# test_my_design.py (extended)

import cocotb
from cocotb.triggers import RisingEdge, FallingEdge, Timer
from cocotb.types    import LogicArray
from cocotb.utils import get_sim_time

from pixel_logic     import PixelGolden


TOT6TO4 = True
TOT_DUAL_EDGE = True
CLK_PERIOD = 400


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


@cocotb.test()
async def pixel_test(dut):
    """Try accessing the design."""

    cocotb.start_soon(generate_clock(dut))  # run the clock "in the background"
    dut.Reset_b.value = 1
    dut.TotMemWriteAddr.value = LogicArray.from_unsigned(0, 8)
    dut.TotMemReadAddr.value = LogicArray.from_unsigned(0, 8) # pretty sure this is one-hot encoded
    dut.Tot6to4Mapping.value = 1 if TOT6TO4 else 0
    dut.TotDualEdgeCount.value = 1 if TOT_DUAL_EDGE else 0
    dut.HitLeAny.value = 0

    # init golden model: clk_period, dual_edge, map_6to4, (depth=8)
    golden_model = PixelGolden(400, (1 if TOT_DUAL_EDGE else 0), 
        (1 if TOT6TO4 else 0))

    await Timer(1, unit="ns")
    dut.Reset_b.value = 0
    await Timer(1, unit="ns")  # wait a bit
    dut.Reset_b.value = 1
    await Timer(1, unit="ns")

    
    # for Tot mem latch condition: 
    # ^^1 needs TE to go high (sync) then clock to go low (falling edge) (large delay :( )
    # worst case 1.5x clk period
    region_hit = False
    for i in range(0, 7, 1):
        if (i % 3 != 0):
            region_hit = True
        else:
            region_hit = False

        one_hot = 1 << i
        dut.TotMemWriteAddr.value = LogicArray.from_unsigned(one_hot, 8)
        dut.TotMemReadAddr.value = LogicArray.from_unsigned(one_hot, 8)
        if region_hit:
            dut.Hit.value = 1 # simulate hit
        else:
            dut.HitLeAny.value = 1 # region hit 
        t0 = get_sim_time(unit="ps")

        # do whatever
        await Timer((1.5 * CLK_PERIOD + (4 * i * CLK_PERIOD)), unit="ps") # varying value of hit length
        t1 = get_sim_time(unit="ps")
        hit_len = t1 - t0
        if region_hit:
            dut.Hit.value = 0
        else:
            dut.HitLeAny.value = 0

        await Timer(610, unit="ps")   # ^^1 reason for long ish time
        
        if region_hit:
            golden_model.register_hit(i, hit_len) # use index directly instead of one hot
            golden_value = golden_model.read(i)
            cocotb.log.info("""wrote to mem[%s]: dut: %s (%s) golden: %s""",  
            i, dut.TotMemDataOut.value, int(dut.TotMemDataOut.value), golden_value) # i instead of one hot
            raw_cnt = int(hit_len * 2 / CLK_PERIOD if TOT_DUAL_EDGE else hit_len / CLK_PERIOD)
            enc_cnt = dec_to_enc(raw_cnt)
            cocotb.log.info("raw cnt: %s encoded: %s", raw_cnt, enc_cnt)
        else:
            golden_model.register_region_hit(i)
            golden_value = golden_model.read(i)
            cocotb.log.info("wrote to mem[%s]: dut: %s golden: %s REGION HIT!(code 15)",
            i, int(dut.TotMemDataOut.value), golden_value)
        await Timer(1, unit="ns")


def enc_to_dec_range(enc: int) -> tuple[int, int]:
    """Assume mapping-enabled high region (enc should be 8..14). Returns inclusive dec bucket [lo, hi]."""
    if not (8 <= enc <= 14):
        raise ValueError("enc must be 8..14 for high-region mapping")
    lo = 4 * (enc - 6)
    return lo, lo + 3

def dec_to_enc(dec: int) -> int:
    """Assume dec >= 8. Implements: enc = {1, ((dec>>2)-2)[2:0]}, then clamp 15->14."""

    code = 8 + ((dec - 8) // 4)
    if code > 15:
        code = 15
    code4 = code & 0xE # 0xF reserved for false hits (other px in region)
    return code4