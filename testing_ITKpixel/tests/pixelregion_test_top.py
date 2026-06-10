import cocotb
from cocotb.triggers import RisingEdge, FallingEdge, Timer
from cocotb.types import LogicArray
from cocotb.utils import get_sim_time
from collections import deque

TOT6TO4 = True
TOT_DUAL_EDGE = False

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



# use 120 wrap around for simplicy, read bcid wrap around takes alot longer
async def generate_latency_counters(dut):
    """
    Drive LatCnt and LatCntReq as Gray-encoded counters.
    Trigger latency = 20 BX.
    BX counter wraps at 120.
    """
    bx = 20 # start at 20 to make things easier
    while True:
        await RisingEdge(dut.Clk)
        bx = (bx + 1)
        dut.LatCnt.value = bin_to_gray(bx, LATENCY_COUNTER_BITS)
        # 20 BX latency
        bx_req = (bx - 20)
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
    dut.TrigId.value = 0
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
    dut.TrigId.value = 1
    await RisingEdge(dut.Clk)  # clock 3: counter_last fires, Trig is high -> triggered!
    dut.Trig.value = 0
    dut.TrigIdReq.value = 0 # change to different 

    # Wait a bit (prolly don't need long 0-2)
    await wait_clks(dut, 8)

    dut.TrigIdReq.value = 1 # now match the TrigId request
    await wait_clks(dut, 1) 
    dut.TrigIdReq.value = 0 # then switch off
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

  
  # =========================================================================
    # TEST 2: Two overlapping hits, then read out one by one
    #   Hit 1: mask 1010, 8 BX total, TrigId = 1
    #   Hit 2: mask 1111, starts 3 BX later, ends same time as Hit 1 (5 BX), TrigId = 2
    # =========================================================================
    dut._log.info("="*60)
    dut._log.info("TEST 2: Overlapping hits (1010 for 8BX, 1111 for 5BX), read 1-by-1")
    dut._log.info("="*60)

    # ---------- Drive both overlapping hits on the shared Hit bus ----------
    mask1 = 0b1010
    mask2 = 0b0101

    # Start Hit 1 (TrigId latched for entry 1)
    dut.TrigId.value = 1
    dut.Hit.value = mask1
    await wait_clks(dut, 1)          # wait one then start counting (entry 1)
    LatCntTime1 = dut.LatCnt.value   # timestamp for hit 1

    # 3 cycles after hit 1 started, hit 2 begins -> bus carries mask1 | mask2
    await wait_clks(dut, 2)          # already advanced 1, need 2 more to reach +3
    dut.TrigId.value = 2
    dut.Hit.value = mask1 | mask2    # overlap region
    await wait_clks(dut, 1)          # wait one then start counting (entry 2)
    LatCntTime2 = dut.LatCnt.value   # timestamp for hit 2

    # Both end together: hit 1 total = 8 BX. We've used 1 + 2 + 1 = 4 BX so far.
    await wait_clks(dut, 8 - 4)      # remaining BX until both fall
    dut.Hit.value = 0

    # ---------- Trigger entry 1 at its latency match ----------
    while LatCntTime1 != dut.LatCntReq.value:
        await RisingEdge(dut.Clk)    # wait until bcid matches after latency
    dut.TrigId.value = 1
    dut.Trig.value = 1
    await RisingEdge(dut.Clk)        # counter_last fires, Trig high -> triggered (entry 1)
    dut.Trig.value = 0

    # ---------- Trigger entry 2 at its latency match (3 BX later) ----------
    while LatCntTime2 != dut.LatCntReq.value:
        await RisingEdge(dut.Clk)    # wait until bcid matches after latency
    dut.TrigId.value = 2
    dut.Trig.value = 1
    await RisingEdge(dut.Clk)        # counter_last fires, Trig high -> triggered (entry 2)
    dut.Trig.value = 0
    dut.TrigIdReq.value = 0          # parked at non-matching

    await wait_clks(dut, 8)

    # ---------- READ 1: select TrigId = 1 ----------
    dut.TrigIdReq.value = 1          # match first TrigId
    await wait_clks(dut, 1)
    dut.TrigIdReq.value = 0          # switch off
    await wait_clks(dut, 5)

    try:
        dut._log.info(f"DataToCoreEn = {dut.LatencyMem.DataToCoreEn.value}")
    except AttributeError:
        pass
    try:
        dut._log.info(f"ready_to_read = {dut.LatencyMem.ready_to_read.value}")
    except AttributeError:
        pass

    dut.Read.value = 1
    await RisingEdge(dut.Clk)
    try:
        data = dut.DataToCore.value.integer
    except ValueError:
        data = 0
        dut._log.warning("DataToCore has X/Z values")
    dut.Read.value = 0

    tots = decode_region_data(data)
    dut._log.info(f"READ 1 (TrigId=1, mask 1010): DataToCore = 0x{data:04x}, ToT values = {tots}")
    dut._log.info(f"  Pixel 0 ToT = {tots[0]} (expect 15 = no hit)")
    dut._log.info(f"  Pixel 1 ToT = {tots[1]} (HIT)")
    dut._log.info(f"  Pixel 2 ToT = {tots[2]} (expect 15 = no hit)")
    dut._log.info(f"  Pixel 3 ToT = {tots[3]} (HIT)")

    await wait_clks(dut, 5)

    # ---------- READ 2: select TrigId = 2 ----------
    dut.TrigIdReq.value = 2          # match second TrigId
    await wait_clks(dut, 1)
    dut.TrigIdReq.value = 0          # switch off
    await wait_clks(dut, 5)

    try:
        dut._log.info(f"DataToCoreEn = {dut.LatencyMem.DataToCoreEn.value}")
    except AttributeError:
        pass
    try:
        dut._log.info(f"ready_to_read = {dut.LatencyMem.ready_to_read.value}")
    except AttributeError:
        pass

    dut.Read.value = 1
    await RisingEdge(dut.Clk)
    try:
        data = dut.DataToCore.value.integer
    except ValueError:
        data = 0
        dut._log.warning("DataToCore has X/Z values")
    dut.Read.value = 0

    tots = decode_region_data(data)
    dut._log.info(f"READ 2 (TrigId=2, mask 0101): DataToCore = 0x{data:04x}, ToT values = {tots}")
    dut._log.info(f"  Pixel 0 ToT = {tots[0]} (HIT)")
    dut._log.info(f"  Pixel 1 ToT = {tots[1]} ")
    dut._log.info(f"  Pixel 2 ToT = {tots[2]} (HIT)")
    dut._log.info(f"  Pixel 3 ToT = {tots[3]} ")

    await wait_clks(dut, 5)

    dut._log.info("All tests complete.")