# test_latency_mem.py
import cocotb
from cocotb.clock import Clock
from cocotb.triggers import RisingEdge, Timer

LAT_BITS = 9
TRIG_ID_BITS = 8  # Adjust `TRIG_ID_BITS define
DEPTH = 8 # number of Tot / Lat Mem Cells
CLK_PERIOD_NS = 25  # 40 MHz


def bin_to_gray(x: int, bits: int = LAT_BITS) -> int:
    return ((x >> 1) ^ x) & ((1 << bits) - 1)


def onehot(i: int) -> int:
    return 1 << i


async def reset_dut(dut, cycles=4):
    """Apply synchronous reset for a number of clock cycles."""
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
    for _ in range(cycles):
        await RisingEdge(dut.Clk)
    dut.Reset_b.value = 1
    await RisingEdge(dut.Clk)

@cocotb.test()
async def test_write_trigger_read(dut):
    """
    Write a hit into slot 0, let it reach trigger-latency match,
    trigger it, then read it out.

    Register several hits, read out each one after a latency
    check that w/r ptrs are updated
    """
    # SETUP
    cocotb.start_soon(Clock(dut.Clk, CLK_PERIOD_NS, units="ns").start())
    await reset_dut(dut)

    latency = 20 # programmed trigger latency (BCID counts)
    hit_bx_arr = [10, 18, 40] # for future, stagger hits

    hit_bx = hit_bx_arr[0]
    hit_bx2 = hit_bx_arr[1]

    # Phase 1: WRITE — assert HitLeAny for exactly 1 cycle
    bx = hit_bx

    # setup for bx1
    dut.LatCnt.value = bin_to_gray(bx)
    dut.LatCntReq.value = bin_to_gray(bx - latency)  # not relevant now, just realistic
    dut.HitLeAny.value = 1 # first hit
    await RisingEdge(dut.Clk) 

    # Check that slot 0 was selected for writing
    got_w = int(dut.TotMemWriteAddr.value)
    dut._log.info(f"TotMemWriteAddr = 0x{got_w:02x} (expected 0x01)")
    assert got_w == onehot(0), f"Expected write to slot 0, got 0x{got_w:02x}"

    dut.HitLeAny.value = 0
    await RisingEdge(dut.Clk)
    # after clk edge, inside LatencyMem[0]:
    # FSM => Counting State, counter = Gray(hit BCID)

    # Verify Full[0] is set
    full_val = int(dut.full.value)  # internal signal, may need to use hierarchical path
    dut._log.info(f"full = 0b{full_val:08b}")

    # --------------------do the same for bx2, except we should see slot 1-------------
    bx = hit_bx2
    dut.LatCnt.value = bin_to_gray(bx)
    dut.LatCntReq.value = bin_to_gray(bx - latency)  # not relevant now, just realistic
    dut.HitLeAny.value = 1 # second hit
    await RisingEdge(dut.Clk) 

    # Check that slot 1 was selected for writing
    got_w = int(dut.TotMemWriteAddr.value)
    dut._log.info(f"TotMemWriteAddr = 0x{got_w:02x} (expected 0x01)")
    assert got_w == onehot(1), f"Expected write to slot 1, got 0x{got_w:02x}"

    dut.HitLeAny.value = 0
    await RisingEdge(dut.Clk)
    # after clk edge, inside LatencyMem[0]:
    # FSM => Counting State, counter = Gray(hit BCID)

    # Verify Full[0] is set
    full_val = int(dut.full.value)  # internal signal, may need to use hierarchical path
    dut._log.info(f"full = 0b{full_val:08b}")
    #--------------------------------------------------------------------------------------


    # PHASE 2 Advance time until counter_last matches
    bx = hit_bx + 4  # current BX counter position
    trig_id_val = 0x0A  # arbitrary trigger ID
    target_bx = hit_bx + latency
    cycles_to_wait = target_bx - bx

    dut._log.info(f"Advancing {cycles_to_wait} cycles to reach match at BX={target_bx}")

    for i in range(cycles_to_wait):
        dut.LatCnt.value = bin_to_gray(bx)
        dut.LatCntReq.value = bin_to_gray(bx - latency)
        await RisingEdge(dut.Clk)
        bx += 1

    # Now bx == target_bx == hit_bx + latency
    # LatCntReq = Gray(bx - latency) = Gray(hit_bx) = stored counter value
    # => counter_last should be true this cycle!

    # Assert Trig simultaneously with the match
    dut.LatCnt.value = bin_to_gray(bx)
    dut.LatCntReq.value = bin_to_gray(bx - latency)  # = Gray(hit_bx)
    dut.Trig.value = 1
    dut.TrigId.value = trig_id_val
    await RisingEdge(dut.Clk)
    bx += 1

    dut.Trig.value = 0
    dut._log.info("Trigger asserted at match cycle")

    # After this edge, inside LatencyMemCell[0]:
    # FSM => TRIGGERED state, counter <= LatCnt[upper bits], TrigId

    # ------------------ now advance counter for hit_bx2--------------------------(same logic)
    trig_id_val = 0x0D
    target_bx = hit_bx2 + latency
    cycles_to_wait = target_bx - bx # distance from cur bx to hit_bx2
    dut._log.info(f"Advancing {cycles_to_wait} cycles to reach match at BX={target_bx}")
    for i in range(cycles_to_wait):
        dut.LatCnt.value = bin_to_gray(bx)
        dut.LatCntReq.value = bin_to_gray(bx - latency)
        await RisingEdge(dut.Clk)
        bx += 1

    # same trig for bx2
    # Now bx == target_bx == hit_bx + latency
    # LatCntReq = Gray(bx - latency) = Gray(hit_bx) = stored counter value
    # => counter_last should be true this cycle!


    # Assert Trig simultaneously with the match
    dut.LatCnt.value = bin_to_gray(bx)
    dut.LatCntReq.value = bin_to_gray(bx - latency)  # = Gray(hit_bx)
    dut.Trig.value = 1
    dut.TrigId.value = trig_id_val
    await RisingEdge(dut.Clk)
    bx += 1

    dut.Trig.value = 0
    dut._log.info("Trigger asserted at match cycle")
    # After this edge, inside LatencyMemCell[0]:
    # FSM => TRIGGERED state, counter <= LatCnt[upper bits], TrigId
    # PHASE 2 FOR BOTH DONE-------------------------------------------


    # READOUT FOR SLOT 0:---------------------------------------------
    # PHASE 3:
    # need TrigIdReq = trig_id_val
    # 
    trig_id_val = 0x0A  # set back to trig for slot 0
    dut.LatCnt.value = bin_to_gray(bx)
    dut.LatCntReq.value = bin_to_gray(bx - latency)
    dut.TrigIdReq.value = trig_id_val
    await RisingEdge(dut.Clk)
    bx += 1
    # req_to_read should be true

    # PHASE 4
    # Mem slot 0 ReadyToRead should be high 

    # Give one more cycle for signals to settle through the gated clock domain
    dut.LatCnt.value = bin_to_gray(bx)
    dut.LatCntReq.value = bin_to_gray(bx - latency)
    await RisingEdge(dut.Clk)
    bx += 1

    # TODO: need to figure out and re-en
    # data_en = int(dut.DataToCoreEn.value)
    # dut._log.info(f"DataToCoreEn = {data_en}")
    # assert data_en == 1, f"Expected DataToCoreEn=1, got {data_en}"

    # tok_out = int(dut.TokOut.value)
    # dut._log.info(f"TokOut = {tok_out}")
    # assert tok_out == 1, "Expected TokOut=1 (token_rise should be high)"

    # read_addr = int(dut.TotMemReadAddr.value)
    # dut._log.info(f"TotMemReadAddr = 0x{read_addr:02x}")
    # assert read_addr == onehot(0), f"Expected read addr slot 0, got 0x{read_addr:02x}"

    # Phase 5: Issue Read to complete readout
    dut.Read.value = 1
    await RisingEdge(dut.Clk)
    dut.Read.value = 0
    bx += 1

    # After Read, the ReadLatMem signal enables the gated clock,
    # and on the next edge the TOREAD state should clear
    # (trig_state goes to 0 because neither triggered nor req_to_read)
    await RisingEdge(dut.Clk)
    bx += 1
    await RisingEdge(dut.Clk)
    bx += 1

    # # Slot should now be free
    # data_en = int(dut.DataToCoreEn.value)
    # dut._log.info(f"DataToCoreEn after read = {data_en}")
    # assert data_en == 0, f"Expected DataToCoreEn=0 after readout, got {data_en}"

    # dut._log.info("TEST PASSED: Write → Trigger → Read cycle completed successfully")
    # -----------slot 0 readout done-------------------------------------------------

    
    # now try to readout for slot1 (BX2)--------------------------------------
    # READOUT FOR SLOT 1:
    # PHASE 3:
    # need TrigIdReq = trig_id_val
    # 
    trig_id_val = 0x0D  # set back to trig for slot 1
    dut.LatCnt.value = bin_to_gray(bx)
    dut.LatCntReq.value = bin_to_gray(bx - latency)
    dut.TrigIdReq.value = trig_id_val
    await RisingEdge(dut.Clk)
    bx += 1
    # req_to_read should be true

    # PHASE 4
    # Mem slot 1 ReadyToRead should be high 

    # Give one more cycle for signals to settle through the gated clock domain
    dut.LatCnt.value = bin_to_gray(bx)
    dut.LatCntReq.value = bin_to_gray(bx - latency)
    await RisingEdge(dut.Clk)
    bx += 1

    # TODO: need to figure out and re-en
    # data_en = int(dut.DataToCoreEn.value)
    # dut._log.info(f"DataToCoreEn = {data_en}")
    # assert data_en == 1, f"Expected DataToCoreEn=1, got {data_en}"

    # tok_out = int(dut.TokOut.value)
    # dut._log.info(f"TokOut = {tok_out}")
    # assert tok_out == 1, "Expected TokOut=1 (token_rise should be high)"

    # read_addr = int(dut.TotMemReadAddr.value)
    # dut._log.info(f"TotMemReadAddr = 0x{read_addr:02x}")
    # assert read_addr == onehot(1), f"Expected read addr slot 1, got 0x{read_addr:02x}"

    # Phase 5: Issue Read to complete readout
    dut.Read.value = 1
    await RisingEdge(dut.Clk)
    dut.Read.value = 0
    bx += 1

    # After Read, the ReadLatMem signal enables the gated clock,
    # and on the next edge the TOREAD state should clear
    # (trig_state goes to 0 because neither triggered nor req_to_read)
    await RisingEdge(dut.Clk)
    bx += 1
    await RisingEdge(dut.Clk)
    bx += 1

    # # Slot should now be free
    # data_en = int(dut.DataToCoreEn.value)
    # dut._log.info(f"DataToCoreEn after read = {data_en}")
    # assert data_en == 0, f"Expected DataToCoreEn=0 after readout, got {data_en}"

    # dut._log.info("TEST PASSED: Write → Trigger → Read cycle completed successfully")