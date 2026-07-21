# simple model of PixelLogic module
# focuses on top level hit read/writes, less on lower level implementation. 

def onehot_index(x, depth=8):
    # returns 0..depth-1 or None
    if x <= 0 or (x & (x - 1)) != 0:
        return None
    i = x.bit_length() - 1
    return i if i < depth else None


class PixelGolden:
    def __init__(self, clk_period, dual_edge, map_6to4, depth=8):
        self.clk = float(clk_period)
        self.dual = 1 if dual_edge else 0
        self.map = 1 if map_6to4 else 0
        self.depth = depth
        self.mem = [0] * depth  # you said init {0}; change to [0xF]*depth if you want

    def reset(self):
        self.mem = [0] * self.depth

    def _encode(self, hit_time):
        # Convert hit_time -> tot count
        if self.clk <= 0:
            raise ValueError("clk_period must be > 0")

        tot = hit_time / self.clk # not floor div bcs *2 later
        if self.dual:
            tot *= 2.0

        # quantize + clamp to 6-bit range
        tot6 = int(tot)
        if tot6 < 0:
            tot6 = 0
        if tot6 > 63:
            tot6 = 63

        # optional simplified 6->4 mapping
        if self.map:
            # "if >=8 apply mapping", simple bucket compression
            if tot6 >= 8:
                code = 8 + ((tot6 - 8) // 4)
                if code > 15:
                    code = 15
                code4 = code & 0xF
            else:
                code4 = tot6 & 0xF
        else:
            code4 = tot6 & 0xF

        # reserve 0xF for empty-hit; map real 0xF -> 0xE
        if code4 == 0xF:
            code4 = 0xE
        return code4

    def register_hit(self, wr_i, hit_time):
        # i = onehot_index(wr_onehot, self.depth) just pass in index
        i = wr_i
        if i is None:
            return None
        code = self._encode(hit_time)
        self.mem[i] = code
        return code

    def register_region_hit(self, wr_i): # hit from other region need to pad
        # i = onehot_index(wr_onehot, self.depth) just pass index
        i = wr_i
        if i is None:
            return None
        self.mem[i] = 0xF
        return 0xF

    def read(self, rd_i):
        # i = onehot_index(rd_onehot, self.depth) just pass in index
        i = rd_i
        if i is None:
            return 0
        return self.mem[i] & 0xF
