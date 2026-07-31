# pixel_logic.py
#
# Simple behavioural model of PixelLogic.
#
# Timing semantics implemented here:
#   * The ToT counter is a NEGEDGE counter.  It arms when Hit asserts, then the
#     FIRST falling edge the pixel actually sees becomes the zero reference of
#     the count; every further falling edge while Hit is high increments it.
#   * The LSB of the ToT word is not counted, it is *sampled*: when the hit
#     ends, if the pixel clock is HIGH at that moment the LSB is 1, else 0.
#     (dual-edge mode only; in single-edge mode the LSB is simply dropped.)
#         tot_dual   = 2 * negedge_count + clk_high_at_end
#         tot_single =     negedge_count
#   * Clock gating matters.  The region gate samples the enable at the rising
#     edge and only then lets the high phase through, so a hit that starts
#     *during* a high phase does NOT get a falling edge at the end of that
#     phase -- the first falling edge it sees is one full period later.  This
#     is the main reason the start alignment of a hit changes the result.
#
# Clock convention (matches the testbench clock generator):
#     falling edges at  t % clk == neg_phase          (default 0)
#     rising  edges at  t % clk == neg_phase + clk/2
#     clock is HIGH on  [posedge, negedge)

def onehot_index(x, depth=8):
    # returns 0..depth-1 or None
    if x <= 0 or (x & (x - 1)) != 0:
        return None
    i = x.bit_length() - 1
    return i if i < depth else None


class PixelGolden:

    EPS = 1e-6          # ps, numerical guard for float edge compares

    def __init__(self, clk_period, dual_edge, map_6to4, depth=8,
                 neg_phase=0.0, gated=True):
        self.clk = float(clk_period)
        if self.clk <= 0:
            raise ValueError("clk_period must be > 0")
        self.dual      = 1 if dual_edge else 0
        self.map       = 1 if map_6to4 else 0
        self.depth     = depth
        self.gated     = bool(gated)
        self.neg_phase = float(neg_phase) % self.clk
        self.pos_phase = (self.neg_phase + self.clk / 2.0) % self.clk
        self.mem       = [0] * depth
        self.last      = {}          # diagnostics from the most recent write

    def reset(self):
        self.mem  = [0] * self.depth
        self.last = {}

    # ------------------------------------------------------------------ clock
    def _next_edge(self, t, phase):
        """First edge of the given phase at or after t (coincident == this one)."""
        d = (phase - t) % self.clk
        if d > self.clk - self.EPS:      # float noise around "exactly on it"
            d = 0.0
        return t + d

    def _first_negedge(self, t_start):
        """First falling edge the pixel actually sees after the hit arrives."""
        if not self.gated:
            return self._next_edge(t_start, self.neg_phase)
        # gated: the enable is sampled at a rising edge, the pulse that follows
        # is the first one that can fall.
        t_pos = self._next_edge(t_start, self.pos_phase)
        return t_pos + self.clk / 2.0

    def _clk_high(self, t, t_enable_from=None):
        """Is the (gated) pixel clock high at time t?"""
        ph   = (t - self.pos_phase) % self.clk     # 0 at posedge, clk/2 at negedge
        high = ph < self.clk / 2.0 - self.EPS
        if high and self.gated and t_enable_from is not None:
            t_pos = t - ph                          # posedge that opened this phase
            if t_pos < t_enable_from - self.EPS:    # gate was shut back then
                high = False
        return high

    # -------------------------------------------------------------- measuring
    def measure(self, t_start, t_end):
        """Absolute-time hit -> (tot, info).  info carries the intermediates."""
        t_n0 = self._first_negedge(t_start)

        if t_n0 > t_end + self.EPS:
            # hit was over before the pixel ever saw a falling edge
            return 0, dict(counted=False, t_first_neg=t_n0, full=0, lsb=0,
                           tot=0, tot_clamped=0)

        dt   = t_end - t_n0
        full = int((dt + self.EPS) // self.clk)             # negedges counted
        lsb  = 1 if self._clk_high(t_end, t_start) else 0   # phase at hit end

        tot = 2 * full + lsb if self.dual else full
        clamped = max(0, min(63, tot))

        return clamped, dict(counted=True, t_first_neg=t_n0, full=full,
                             lsb=lsb, tot=tot, tot_clamped=clamped)

    def _encode(self, tot6):
        """6-bit ToT count -> 4-bit memory word."""
        tot6 = max(0, min(63, int(tot6)))

        if self.map and tot6 >= 8:
            code = 8 + ((tot6 - 8) // 4)
            if code > 15:
                code = 15
            code4 = code & 0xF
        else:
            code4 = tot6 & 0xF

        if code4 == 0xF:            # 0xF reserved for "hit elsewhere in region"
            code4 = 0xE
        return code4

    def predict(self, t_start, t_end, delta=None):
        """Nominal code plus every code reachable if the hit edges race with a
        clock edge.  Perturbing by +/-delta only changes the answer when an
        edge is genuinely coincident, so a singleton list means 'no race'."""
        if delta is None:
            delta = self.clk / 1000.0

        nominal, info = self.measure(t_start, t_end)
        codes = {self._encode(nominal)}
        for ds in (-delta, +delta):
            for de in (-delta, +delta):
                tot, _ = self.measure(t_start + ds, t_end + de)
                codes.add(self._encode(tot))

        info["code"]     = self._encode(nominal)
        info["possible"] = sorted(codes)
        info["race"]     = len(codes) > 1
        return info["code"], info["possible"], info

    # ------------------------------------------------------------------- regs
    def register_hit(self, wr_i, t_start, t_end):
        """Record a real hit given ABSOLUTE start/end times (ps)."""
        i = wr_i
        if i is None or not (0 <= i < self.depth):
            return None
        code, _possible, info = self.predict(t_start, t_end)
        self.mem[i] = code
        self.last   = info
        return code

    def register_hit_duration(self, wr_i, hit_time, t_start=None):
        """Back-compat shim for length-only callers.  Without a start time the
        phase is unknown, so assume the hit starts on a rising edge."""
        if t_start is None:
            t_start = self.pos_phase
        return self.register_hit(wr_i, t_start, t_start + hit_time)

    def register_region_hit(self, wr_i):
        """Hit from another pixel in the region -> pad with the reserved code."""
        i = wr_i
        if i is None or not (0 <= i < self.depth):
            return None
        self.mem[i] = 0xF
        self.last   = dict(counted=False, full=0, lsb=0, tot=None,
                           code=0xF, possible=[0xF], race=False,
                           t_first_neg=None)
        return 0xF

    def read(self, rd_i):
        i = rd_i
        if i is None or not (0 <= i < self.depth):
            return 0
        return self.mem[i] & 0xF