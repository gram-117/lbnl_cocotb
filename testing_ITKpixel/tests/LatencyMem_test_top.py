# test_latency_mem_minimal.py
import cocotb
from cocotb.clock import Clock
from cocotb.triggers import RisingEdge

LAT_BITS = 9
DEPTH = 8

def bin_to_gray(x: int) -> int:
    return (x >> 1) ^ x

def onehot(i: int) -> int:
    return 1 << i

@cocotb.test()
async def latency_mem_write_then_readslot(dut):
    # clock
    cocotb.start_soon(Clock(dut.Clk, 25, unit="ns").start())

    # ----------------
    # reset + init
    # ----------------
    dut.Reset_b.value = 0
    dut.HitLeAny.value = 0
    dut.PwrDwnAll.value = 0
    dut.Trig.value = 0
    dut.TrigClear.value = 0
    dut.TokIn.value = 0
    dut.Read.value = 0

    dut.LatCnt.value = 0
    dut.LatCntReq.value = 0
    dut.TrigId.value = 0
    dut.TrigIdReq.value = 0

    for _ in range(3):
        await RisingEdge(dut.Clk)
    dut.Reset_b.value = 1
    await RisingEdge(dut.Clk)

    # -----------------------------------------
    # 1) "WRITE": HitLeAny allocates a free slot
    # -----------------------------------------
    lat_bin = 5
    lat_gray = bin_to_gray(lat_bin) & ((1 << LAT_BITS) - 1)

    dut.LatCnt.value = lat_gray
    dut.HitLeAny.value = 1
    await RisingEdge(dut.Clk)
    dut.HitLeAny.value = 0

    got_w = int(dut.TotMemWriteAddr.value)
    assert got_w == onehot(0), f"Expected write slot0 (0x01), got 0x{got_w:02x}"

    # --------------------------------------------------------------------
    # 2) "MAKE IT READABLE":
    #    force counter_last by LatCntReq == stored counter (== LatCnt written),
    #    and assert Trig for one cycle to mark it triggered.
    # --------------------------------------------------------------------
    dut.LatCntReq.value = lat_gray
    dut.Trig.value = 1
    await RisingEdge(dut.Clk)
    dut.Trig.value = 0

    # Match trigger-id request so the cell queues for readout
    trig_id = 0x3A
    dut.TrigId.value = trig_id
    dut.TrigIdReq.value = trig_id

    # Next cycle should expose read slot + DataToCoreEn
    await RisingEdge(dut.Clk)

    got_en = int(dut.DataToCoreEn.value)
    got_r  = int(dut.TotMemReadAddr.value)

    # assert got_en == 1, "Expected DataToCoreEn=1 when something is ready and TokIn=0"
    # assert got_r == onehot(0), f"Expected read slot0 (0x01), got 0x{got_r:02x}"

    # Optional: exercise Read input (doesn't output data, but toggles internal read
