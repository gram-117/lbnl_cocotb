# test_my_design.py  (edge-alignment hit classification, absolute-time golden)

import cocotb
from cocotb.triggers import Timer
from cocotb.types    import LogicArray
from cocotb.utils    import get_sim_time

from pixel_logic     import PixelGolden


# ----------------------------------------------------------------------- knobs
TOT6TO4            = False
TOT_DUAL_EDGE      = True
CLK_PERIOD         = 400      # ps
EPS                = 20       # ps -- the "right before"/"right after" skew
DEFAULT_CYCLES     = 4        # default hit: posedge -> 4 cycles -> posedge
SAMPLE_DELAY_CYC   = 1        # TotMemDataOut is valid 1 cycle after hit end
SAMPLE_SKEW        = 20       # ps past that point, to dodge the edge itself
MEM_DEPTH          = 7        # usable addresses 0..6 inside the 8b one-hot bus
ASSERT_ON_MISMATCH = False

POS_PHASE = CLK_PERIOD // 2   # clock generator drives low first, high second
NEG_PHASE = 0

ALIGNMENTS = {
    "at_pos":     POS_PHASE,
    "before_pos": (POS_PHASE - EPS) % CLK_PERIOD,
    "after_pos":  (POS_PHASE + EPS) % CLK_PERIOD,
    "at_neg":     NEG_PHASE,
    "before_neg": (NEG_PHASE - EPS) % CLK_PERIOD,
    "after_neg":  (NEG_PHASE + EPS) % CLK_PERIOD,
}
ALIGN_ORDER = ["at_pos", "before_pos", "after_pos",
               "at_neg", "before_neg", "after_neg"]


# ----------------------------------------------------------------------- clock
async def generate_clock(dut):
    """Simulate region gate clock, enable after dut enables PixelClkEn"""
    while True:
        dut.PixelClkGated.value = 0
        await Timer(CLK_PERIOD / 2, unit="ps")
        if dut.PixelClkEn.value == 1:
            dut.PixelClkGated.value = 1
        else:
            dut.PixelClkGated.value = 0
        await Timer(CLK_PERIOD / 2, unit="ps")


# --------------------------------------------------------------------- helpers
async def wait_until_phase(phase, min_wait=0):
    """Advance to the next time with (sim_time % CLK_PERIOD) == phase, after
    waiting at least min_wait ps.  Phase is a pure function of absolute time
    because the gate masks the high phase without shifting it, so this never
    drifts and never deadlocks while the clock is gated off."""
    now    = get_sim_time(unit="ps")
    target = now + min_wait
    total  = min_wait + ((phase - (target % CLK_PERIOD)) % CLK_PERIOD)
    if total == 0:
        total = CLK_PERIOD
    await Timer(total, unit="ps")


def sig_int(sig):
    try:
        return int(sig.value)
    except ValueError:
        return None


def enc_to_dec_range(enc: int) -> tuple[int, int]:
    """Assume mapping-enabled high region (enc 8..14).  Inclusive dec bucket."""
    if not (8 <= enc <= 14):
        raise ValueError("enc must be 8..14 for high-region mapping")
    lo = 4 * (enc - 6)
    return lo, lo + 3


def dec_to_enc(dec: int) -> int:
    """enc = {1, ((dec>>2)-2)[2:0]}, clamped, 0xF reserved."""
    code = 8 + ((dec - 8) // 4)
    if code > 15:
        code = 15
    return code & 0xE


# ------------------------------------------------------------- one transaction
async def run_hit(dut, golden, results, name, addr,
                  start_align, end_align,
                  cycles=DEFAULT_CYCLES, region_hit=False):

    one_hot = 1 << addr
    dut.TotMemWriteAddr.value = LogicArray.from_unsigned(one_hot, 8)
    dut.TotMemReadAddr.value  = LogicArray.from_unsigned(one_hot, 8)

    # --- leading edge of the hit, aligned to the requested clock phase -------
    await wait_until_phase(ALIGNMENTS[start_align])
    if region_hit:
        dut.HitLeAny.value = 1
    else:
        dut.Hit.value = 1
    t0 = get_sim_time(unit="ps")

    # --- trailing edge: at least `cycles` clocks later, on the requested phase
    await wait_until_phase(ALIGNMENTS[end_align], min_wait=cycles * CLK_PERIOD)
    if region_hit:
        dut.HitLeAny.value = 0
    else:
        dut.Hit.value = 0
    t1 = get_sim_time(unit="ps")

    # --- golden prediction from the ABSOLUTE times --------------------------
    if region_hit:
        golden.register_region_hit(addr)
    else:
        golden.register_hit(addr, t0, t1)
    info       = golden.last
    golden_val = golden.read(addr)
    possible   = info.get("possible", [golden_val])

    # --- sample TotMemDataOut one cycle after the hit ended -----------------
    await Timer(SAMPLE_DELAY_CYC * CLK_PERIOD + SAMPLE_SKEW, unit="ps")
    dut_val = sig_int(dut.TotMemDataOut)

    if dut_val == golden_val:
        verdict = "OK"
    elif dut_val in possible:
        verdict = "RACE-OK"
    else:
        verdict = "<<< MISMATCH"

    if region_hit:
        cocotb.log.info(
            "%-24s mem[%d] t0=%6d t1=%6d len=%5d | REGION HIT (0xF) | "
            "dut=%s golden=%s %s",
            name, addr, t0, t1, t1 - t0, dut_val, golden_val, verdict)
    else:
        cocotb.log.info(
            "%-24s mem[%d] t0=%6d t1=%6d len=%5d | neg0=%s full=%d lsb=%d "
            "tot=%s | dut=%s (0x%s) golden=%d (0x%X) %s%s",
            name, addr, t0, t1, t1 - t0,
            info.get("t_first_neg"), info.get("full", 0), info.get("lsb", 0),
            info.get("tot"),
            dut_val, "-" if dut_val is None else format(dut_val, "X"),
            golden_val, golden_val, verdict,
            "  possible=%s" % [hex(p) for p in possible] if info.get("race") else "")

    results.append(dict(name=name, addr=addr, t0=t0, t1=t1,
                        dut=dut_val, golden=golden_val,
                        possible=possible, race=info.get("race", False),
                        verdict=verdict))

    if ASSERT_ON_MISMATCH and verdict.startswith("<<<"):
        raise AssertionError(f"{name}: dut={dut_val} golden={golden_val}")

    await Timer(2 * CLK_PERIOD, unit="ps")     # idle between transactions


# ------------------------------------------------------------------------ test
@cocotb.test()
async def pixel_test(dut):
    """Sweep every combination of hit-start / hit-end clock alignment."""

    cocotb.start_soon(generate_clock(dut))

    dut.Reset_b.value          = 1
    dut.Hit.value              = 0
    dut.HitLeAny.value         = 0
    dut.TotMemWriteAddr.value  = LogicArray.from_unsigned(0, 8)
    dut.TotMemReadAddr.value   = LogicArray.from_unsigned(0, 8)
    dut.Tot6to4Mapping.value   = 1 if TOT6TO4 else 0
    dut.TotDualEdgeCount.value = 1 if TOT_DUAL_EDGE else 0

    golden = PixelGolden(CLK_PERIOD,
                         (1 if TOT_DUAL_EDGE else 0),
                         (1 if TOT6TO4 else 0),
                         depth=8, neg_phase=NEG_PHASE, gated=True)

    await Timer(1, unit="ns")
    dut.Reset_b.value = 0
    await Timer(1, unit="ns")
    dut.Reset_b.value = 1
    await Timer(1, unit="ns")

    results, idx = [], 0

    cocotb.log.info("=== DEFAULT CASE: posedge -> %d cycles -> posedge ===",
                    DEFAULT_CYCLES)
    await run_hit(dut, golden, results, "DEFAULT at_pos->at_pos",
                  idx % MEM_DEPTH, "at_pos", "at_pos", DEFAULT_CYCLES)
    idx += 1

    cocotb.log.info("=== ALIGNMENT SWEEP (%d cases) ===", len(ALIGN_ORDER) ** 2)
    for start_align in ALIGN_ORDER:
        for end_align in ALIGN_ORDER:
            await run_hit(dut, golden, results,
                          f"{start_align}->{end_align}",
                          idx % MEM_DEPTH, start_align, end_align,
                          DEFAULT_CYCLES)
            idx += 1

    bad   = [r for r in results if r["verdict"].startswith("<<<")]
    racy  = [r for r in results if r["race"]]
    cocotb.log.info("=== SUMMARY: %d cases, %d racy, %d mismatches ===",
                    len(results), len(racy), len(bad))
    for r in bad:
        cocotb.log.warning("MISMATCH %-24s mem[%d] dut=%s golden=%s possible=%s",
                           r["name"], r["addr"], r["dut"], r["golden"],
                           [hex(p) for p in r["possible"]])