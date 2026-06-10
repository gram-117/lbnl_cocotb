import cocotb
from cocotb.triggers import RisingEdge, FallingEdge, Timer
from cocotb.types import LogicArray
from cocotb.utils import get_sim_time
from collections import deque

TOT6TO4 = True
TOT_DUAL_EDGE = True
CLK_PERIOD = 400  # ps  (this is NOT 40MHz; it's fast for sim speed. Real would be 25000 ps)
LATENCY_MEM_DEPTH = 8
LATENCY_COUNTER_BITS = 9
TRIG_ID_BITS = 5  # assumed from typical RD53 config


async def generate_clock(dut):
    """Free-running 40 MHz BX clock."""
    while True:
        dut.Clk.value = 0
        await Timer(CLK_PERIOD // 2, unit="ps")
        dut.Clk.value = 1
        await Timer(CLK_PERIOD // 2, unit="ps")


def bin_to_gray(val, bits):
    """Convert binary integer to Gray code."""
    g = val ^ (val >> 1)
    return g & ((1 << bits) - 1)



# attempting to solve issue where bcid gets immidtley overriden by trigid
async def generate_latency_counters(dut):
    """
    Drive LatCnt and LatCntReq as Gray-encoded counters.
    Trigger latency = 20 BX.
    BX counter wraps at 120.
    """
    bx = 0
    while True:
        await RisingEdge(dut.Clk)
        bx = (bx + 1) % 120
        dut.LatCnt.value = bin_to_gray(bx, LATENCY_COUNTER_BITS)
        # 20 BX latency
        bx_req = (bx - 20) % 120
        dut.LatCntReq.value = bin_to_gray(bx_req, LATENCY_COUNTER_BITS)


async def wait_clks(dut, n):
    """Wait for n rising edges of Clk."""
    for _ in range(n):
        await RisingEdge(dut.Clk)


async def reset_dut(dut):
    """Apply synchronous active-low reset."""
    dut.Reset_b.value = 1
    await RisingEdge(dut.Clk)
    dut.Reset_b.value = 0
    # Hold reset for a few clocks so all gated-clock domains see it
    await wait_clks(dut, 5)
    dut.Reset_b.value = 1
    await wait_clks(dut, 2)


def decode_region_data(data_16bit):
    """Extract 4 x 4-bit ToT values from the 16-bit DataToCore bus."""
    tots = []
    for p in range(4):
        tot = (data_16bit >> (4 * p)) & 0xF
        tots.append(tot)
    return tots


@cocotb.test()
async def pixel_test(dut):
    """Inject hits into PixelRegionLogic and read back ToT values."""

    cocotb.start_soon(generate_clock(dut))

    # --- Static signal initialization ---
    dut.Reset_b.value = 1
    dut.Tot6to4Mapping.value = 1 if TOT6TO4 else 0
    dut.TotDualEdgeCount.value = 1 if TOT_DUAL_EDGE else 0
    dut.Hit.value = 0
    dut.PwrDwn.value = 0
    dut.LatCnt.value = 0
    dut.LatCntReq.value = 0
    dut.Trig.value = 0
    dut.TrigClear.value = 0
    dut.TrigId.value = 1
    dut.TrigIdReq.value = 1
    dut.TokIn.value = 0
    dut.Read.value = 0

    await Timer(1, unit="ns")


    # --- Reset ---
    await reset_dut(dut)
    dut._log.info("Reset complete")

    # --- Start latency counters (with latency = 20) ---

    cocotb.start_soon(generate_latency_counters(dut)) # use new one
    await wait_clks(dut, 3)  # let counters stabilize

    # =========================================================================
    # TEST 1: Single pixel hit, short duration (e.g., 3 BX on pixel 0)
    # =========================================================================
    dut._log.info("="*60)
    dut._log.info("TEST 1: Hit pixel 0 for 3 BX clocks")
    dut._log.info("="*60)

    hit_duration = 3  # BX clocks
    pixel_mask = 0b0001  # pixel 0 only

    # Inject hit
    dut.Hit.value = pixel_mask
    await wait_clks(dut, 1) # wait one then start counting
    LatCntTime = dut.LatCnt.value
    await wait_clks(dut, hit_duration - 1)
    dut.Hit.value = 0
    
    while LatCntTime != dut.LatCntReq.value:
      await RisingEdge(dut.Clk)  # wait until bcid matches after latency
    
    dut.Trig.value = 1
    await RisingEdge(dut.Clk)  # clock 3: counter_last fires, Trig is high -> triggered!
    dut.Trig.value = 0
    dut.TrigIdReq.value = 2 # change to different 

    # Wait for HitTe and latch
    await wait_clks(dut, 15)


    dut.TrigIdReq.value = 1 # now match the TrigId request
    await wait_clks(dut, 1) 
    dut.TrigIdReq.value = 2 # then switch off
    await wait_clks(dut, 5)  # give plenty of time
    

    # Try to read
    try:
        dtce = dut.LatencyMem.DataToCoreEn.value
        dut._log.info(f"DataToCoreEn = {dtce}")
    except AttributeError:
        pass

    try:
        rtr = dut.LatencyMem.ready_to_read.value
        dut._log.info(f"ready_to_read = {rtr}")
    except AttributeError:
        pass

    # Read out data
    dut.Read.value = 1
    await RisingEdge(dut.Clk)
    
    try:
        data = dut.DataToCore.value.integer
    except ValueError:
        data = 0
        dut._log.warning("DataToCore has X/Z values")

    dut.Read.value = 0
    tots = decode_region_data(data)
    dut._log.info(f"DataToCore = 0x{data:04x}, ToT values = {tots}")
    dut._log.info(f"  Pixel 0 ToT = {tots[0]}")
    dut._log.info(f"  Pixel 1 ToT = {tots[1]} (expect 15 = no hit)")
    dut._log.info(f"  Pixel 2 ToT = {tots[2]} (expect 15 = no hit)")
    dut._log.info(f"  Pixel 3 ToT = {tots[3]} (expect 15 = no hit)")

    await wait_clks(dut, 5)

    # # =========================================================================
    # # TEST 2: Multiple pixels hit simultaneously with known duration
    # # =========================================================================
    # dut._log.info("="*60)
    # dut._log.info("TEST 2: Hit pixels 0,1,2,3 for 5 BX clocks")
    # dut._log.info("="*60)

    # # Inject hit on all 4 pixels
    # dut.Hit.value = 0b1111
    # await RisingEdge(dut.Clk)  # hit_rise captured
    # await RisingEdge(dut.Clk)  # HitLe fires, WriteLatMem fires

    # # Assert trigger at the right moment
    # dut.Trig.value = 1
    # await RisingEdge(dut.Clk)  # counter_last fires with Trig high
    # dut.Trig.value = 0

    # # Keep hit high for more ToT
    # await wait_clks(dut, 4)  # total ~7 BX of hit
    # dut.Hit.value = 0

    # # Wait for trailing edge + latch + FSM propagation
    # await wait_clks(dut, 10)

    # # Read
    # dut.Read.value = 1
    # await RisingEdge(dut.Clk)
    # try:
    #     data2 = dut.DataToCore.value.integer
    # except ValueError:
    #     data2 = 0
    #     dut._log.warning("DataToCore has X/Z values (test 2)")
    # dut.Read.value = 0

    # tots2 = decode_region_data(data2)
    # dut._log.info(f"DataToCore = 0x{data2:04x}, ToT values = {tots2}")
    # for p in range(4):
    #     dut._log.info(f"  Pixel {p} ToT = {tots2[p]}")

    # await wait_clks(dut, 5)

    # # =========================================================================
    # # TEST 3: Sequential hits with different durations
    # # =========================================================================
    # dut._log.info("="*60)
    # dut._log.info("TEST 3: Sequential hits with varying duration")
    # dut._log.info("="*60)

    # test_durations = [2, 4, 6, 8]  # BX clocks per pixel

    # for px_idx, dur in enumerate(test_durations):
    #     dut._log.info(f"  Hitting pixel {px_idx} for {dur} BX")
    #     px_mask = 1 << px_idx

    #     dut.Hit.value = px_mask
    #     await RisingEdge(dut.Clk)
    #     await RisingEdge(dut.Clk)  # HitLe / WriteLatMem

    #     dut.Trig.value = 1
    #     await RisingEdge(dut.Clk)  # triggered
    #     dut.Trig.value = 0

    #     # Keep hit for remaining duration
    #     remaining = dur - 3  # we already consumed 3 clocks
    #     if remaining > 0:
    #         await wait_clks(dut, remaining)
    #     dut.Hit.value = 0

    #     # Wait for TE + latch + ReadyToRead
    #     await wait_clks(dut, 10)

    #     # Read
    #     dut.Read.value = 1
    #     await RisingEdge(dut.Clk)
    #     try:
    #         data_px = dut.DataToCore.value.integer
    #     except ValueError:
    #         data_px = 0
    #     dut.Read.value = 0

    #     tots_px = decode_region_data(data_px)
    #     dut._log.info(f"    DataToCore = 0x{data_px:04x}, pixel {px_idx} ToT = {tots_px[px_idx]}")

    #     await wait_clks(dut, 5)

    # # =========================================================================
    # # TEST 4: Verify no-hit pixels report code 15
    # # =========================================================================
    # dut._log.info("="*60)
    # dut._log.info("TEST 4: Single pixel 2 hit, verify others get code 15")
    # dut._log.info("="*60)

    # dut.Hit.value = 0b0100  # pixel 2 only
    # await RisingEdge(dut.Clk)
    # await RisingEdge(dut.Clk)

    # dut.Trig.value = 1
    # await RisingEdge(dut.Clk)
    # dut.Trig.value = 0

    # await wait_clks(dut, 5)  # ~8 BX total hit
    # dut.Hit.value = 0

    # await wait_clks(dut, 10)

    # dut.Read.value = 1
    # await RisingEdge(dut.Clk)
    # try:
    #     data4 = dut.DataToCore.value.integer
    # except ValueError:
    #     data4 = 0
    # dut.Read.value = 0

    # tots4 = decode_region_data(data4)
    # dut._log.info(f"DataToCore = 0x{data4:04x}")
    # for p in range(4):
    #     label = "HIT" if p == 2 else "no-hit (expect 15)"
    #     dut._log.info(f"  Pixel {p} ToT = {tots4[p]}  [{label}]")

    # # Check that non-hit pixels have code 15
    # for p in [0, 1, 3]:
    #     if tots4[p] == 15:
    #         dut._log.info(f"  PASS: Pixel {p} correctly reports 15 (no hit)")
    #     else:
    #         dut._log.warning(f"  UNEXPECTED: Pixel {p} reports {tots4[p]}, expected 15")

    # await wait_clks(dut, 10)
    dut._log.info("All tests complete.")