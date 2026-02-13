# test_my_design.py (extended)

import cocotb
from cocotb.triggers import RisingEdge, FallingEdge, Timer
from cocotb.types    import LogicArray
from cocotb.utils import get_sim_time


TOT6TO4 = True
TOT_DUAL_EDGE = True
CLK_PERIOD = 400
LATENCY_MEM_DEPTH = 8
LATENCY_COUNTER_BITS = 9

async def generate_clock(dut):
    """Simulate region gate clock, enable after dut enables PixelClkEn""" 
    while True:
        dut.Clk.value = 0
        await Timer(CLK_PERIOD / 2, unit="ps")
        dut.Clk.value = 1
        await Timer(CLK_PERIOD / 2, unit="ps")

    # need to actually drive these in order to give timestamp. 
    # input  wire [`LATENCY_COUNTER_BITS-1:0] LatCnt,         // Gray-encoded BX timestamp
    # input  wire [`LATENCY_COUNTER_BITS-1:0] LatCntReq,      // Gray-encoded BX timestamp shifted back in time by programmable trigger latency

# LATENCY_MEM_DEPTH = 8
async def generate_lat(dut):
    """Simulate region gate clock, enable after dut enables PixelClkEn""" 
    while True:
        await Timer(CLK_PERIOD, unit="ps")
        # Gray-encoded BX timestamp (from chip periphery)
        val = int(dut.LatCnt.value)
        val += 1
        if (val == 1 << LATENCY_COUNTER_BITS):
            val = LogicArray.from_unsigned(0, LATENCY_COUNTER_BITS)
        val = LogicArray.from_unsigned(val, LATENCY_COUNTER_BITS)
        dut.LatCnt.value = val
        # BX timestamp shifted by programmable trigger latency (from chip periphery)
        # shifted assumed to be 0 for simulation 
        dut.LatCntReq.value = val

def incr(signal):
    width = len(signal)
    mask = (1 << width) - 1

    if signal.value.is_resolvable:
        val = signal.value.integer
    else:
        val = 0

    signal.value = (val + 1) & mask

@cocotb.test()
async def pixel_test(dut):
    """Try accessing the design."""

    cocotb.start_soon(generate_clock(dut))  # run the clock "in the background"
    cocotb.start_soon(generate_lat(dut))
    dut.Reset_b.value = 1
    dut.Tot6to4Mapping.value = 1 if TOT6TO4 else 0
    dut.TotDualEdgeCount.value = 1 if TOT_DUAL_EDGE else 0
    dut.PwrDwn.value = 0
    dut.LatCnt.value = 0 # Gray-encoded BX timestamp (from chip periphery)
    dut.LatCntReq.value = 0 # BX timestamp shifted by programmable trigger latency (from chip periphery)
    dut.Trig.value = 0
    dut.TrigIdReq.value = 0
    dut.TokIn.value = 0
    dut.Read.value = 1

    # init golden model: clk_period, dual_edge, map_6to4, (depth=8)
    # dont need just confirm functionality initially

    await Timer(1, unit="ns")
    dut.Reset_b.value = 0
    await Timer(1, unit="ns")  # wait a bit
    dut.Reset_b.value = 1
    await Timer(1, unit="ns")
    
    # for Tot mem latch condition: 
    # ^^1 needs TE to go high (sync) then clock to go low (falling edge) (large delay :( )
    # worst case 1.5x clk period

    for i in range(1, 5, 1):
        dut.Hit.value = LogicArray.from_unsigned(i, 4)
        await Timer(i * CLK_PERIOD, unit="ps")
        dut.Hit.value = LogicArray.from_unsigned(0, 4) # just 0 work?
        await Timer(1, unit="ns")
        dut.Trig.value = 1
        await Timer(500, unit="ps")
        dut.Trig.value = 0
        await Timer(1200, unit = "ps")



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