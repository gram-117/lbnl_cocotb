import cocotb
from cocotb.triggers import RisingEdge, FallingEdge, Timer
from cocotb.types import LogicArray
from cocotb.utils import get_sim_time

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


async def generate_latency_counters(dut):
    """
    Drive LatCnt and LatCntReq as Gray-encoded counters.
    With zero trigger latency: LatCntReq == LatCnt at all times.
    This means counter_last fires the cycle after WriteLatMem.
    """
    bx = 0
    while True:
        await RisingEdge(dut.Clk)
        bx = (bx + 1) & ((1 << LATENCY_COUNTER_BITS) - 1)
        gray = bin_to_gray(bx, LATENCY_COUNTER_BITS)
        dut.LatCnt.value = gray
        # Zero latency: LatCntReq tracks LatCnt
        dut.LatCntReq.value = gray


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


async def inject_hit(dut, pixel_mask, duration_bx):
    """
    Drive Hit[3:0] = pixel_mask for `duration_bx` clock periods,
    then deassert.  Returns after Hit goes low and HitTe has had
    time to propagate (one extra clock).
    
    pixel_mask: e.g. 0b0001 for pixel 0 only
    duration_bx: how many BX clocks the Hit stays high (determines ToT)
    """
    dut.Hit.value = pixel_mask
    await wait_clks(dut, duration_bx)
    dut.Hit.value = 0
    # Wait for trailing edge processing:
    # HitTe appears 1 clock after Hit falls (state_sr goes 10->01)
    # Then latch needs clock to go low => wait a couple more clocks
    await wait_clks(dut, 3)


async def trigger_and_read(dut):
    """
    Assert Trig for one clock, then wait for ReadyToRead to propagate,
    then perform Read handshake and capture DataToCore.
    
    With zero latency, counter_last fires one clock after the LatencyMemCell
    gets written (WriteLatMem). The FSM sequence is:
    
      Cycle N:   WriteLatMem -> start_state=1, counter loaded with LatCnt
      Cycle N+1: counter==LatCntReq (since latency=0, LatCnt has advanced but
                 counter holds old value... actually let's think more carefully)
    
    Actually with zero latency: LatCntReq == LatCnt always. When WriteLatMem fires,
    counter <= LatCnt. Next cycle, LatCntReq has changed (incremented), so
    counter != LatCntReq. The match happens when LatCntReq wraps back around to
    the stored value, which takes 2^9-1 = 511 cycles. That's too long!
    
    Better approach: Set trigger latency to 1 so LatCntReq is one behind LatCnt.
    Then on the cycle after WriteLatMem, counter (loaded with old LatCnt) == new LatCntReq.
    
    OR: Just pulse Trig on the same cycle as WriteLatMem and ensure counter_last is true.
    
    The simplest approach for testbench: drive LatCntReq = LatCnt (no shift) but
    assert Trig at the right moment. Actually, since the counter is loaded with LatCnt
    at WriteLatMem time, and counter_last checks counter == LatCntReq, and both
    LatCnt and LatCntReq change every cycle, the match only happens on the ONE cycle
    where LatCntReq happens to equal the stored value again.
    
    SIMPLEST: Use latency=1. LatCntReq = gray(bx-1) while LatCnt = gray(bx).
    Then counter (loaded at bx=N with gray(N)) matches LatCntReq=gray(N) at bx=N+1.
    So counter_last fires exactly 1 cycle after WriteLatMem.
    
    We need Trig to be high at that moment.
    """
    # Assert trigger - hold for several clocks to ensure we catch the match
    dut.Trig.value = 1
    await wait_clks(dut, 4)
    dut.Trig.value = 0
    await wait_clks(dut, 2)

    # Now check if DataToCoreEn is asserted (TokIn=0 and ReadyToRead=1)
    # Poll for DataToCoreEn
    timeout = 20
    for i in range(timeout):
        await RisingEdge(dut.Clk)
        try:
            if dut.DataToCore.value.integer != 0:
                break
            # Also check the internal signal
            if dut.LatencyMem.DataToCoreEn.value == 1:
                break
        except ValueError:
            pass
    
    # Perform Read
    dut.Read.value = 1
    await RisingEdge(dut.Clk)
    
    # Capture data
    try:
        data = dut.DataToCore.value.integer
    except ValueError:
        data = 0
    
    dut.Read.value = 0
    await wait_clks(dut, 3)
    
    return data


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
    dut.TrigIdReq.value = 0
    dut.TokIn.value = 0
    dut.Read.value = 0

    await Timer(1, unit="ns")

    # --- Reset ---
    await reset_dut(dut)
    dut._log.info("Reset complete")

    # --- Start latency counters (with latency = 1) ---
    # We drive them manually with latency=1 for precise control
    # Instead of the background task, we'll drive them inline for better control

    # Actually, let's use a modified background counter with latency=1
    bx_counter = {"val": 0}

    async def drive_lat_counters():
        """LatCntReq lags LatCnt by 1 BX (trigger latency = 1)."""
        while True:
            await RisingEdge(dut.Clk)
            old_bx = bx_counter["val"]
            bx_counter["val"] = (old_bx + 1) & ((1 << LATENCY_COUNTER_BITS) - 1)
            new_bx = bx_counter["val"]
            dut.LatCnt.value = bin_to_gray(new_bx, LATENCY_COUNTER_BITS)
            # LatCntReq = gray(bx - 1) = gray(old_bx)
            dut.LatCntReq.value = bin_to_gray(old_bx, LATENCY_COUNTER_BITS)

    cocotb.start_soon(drive_lat_counters())
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
    await wait_clks(dut, hit_duration)
    dut.Hit.value = 0

    # After hit goes low, wait for:
    # 1. HitTe to fire (1-2 clocks after Hit falls)
    # 2. ToT latch to capture (needs falling edge of PixelClkGated with HitTe)
    # 3. LatencyMemCell counter_last to match (1 clock after WriteLatMem with latency=1)
    # HitLe fires on the 2nd clock of Hit being high (state_sr goes 00->10->10...)
    # WriteLatMem fires when HitLe is high (= HitLeAny & free_write_addr)
    # counter_last fires 1 clock later (latency=1)
    # We need Trig to be high at that moment

    # The HitLe already fired during the hit. The LatencyMemCell was written then.
    # By now (after hit ends + 0 clocks), counter_last may have already fired.
    # With latency=1, counter_last fires 1 cycle after WriteLatMem.
    # HitLe fires ~1 clock after Hit rises. So counter_last fires ~2 clocks after Hit rises.
    # Our hit was 3 BX long, so counter_last fired around BX 3 of the hit.
    # We need Trig to have been high at that moment!

    # Strategy: Assert Trig BEFORE the hit so it's already high when counter_last fires.
    # Let's redo this properly.

    await wait_clks(dut, 5)  # settle

    dut._log.info("TEST 1 (retry): Hit pixel 0 with Trig asserted at right time")

    # Phase 1: Inject hit on pixel 0
    dut.Hit.value = 0b0001
    # Wait 1 clock for hit_rise to be captured, then HitLe fires
    await RisingEdge(dut.Clk)  # clock 1: hit_rise latched by PixelClkGated
    await RisingEdge(dut.Clk)  # clock 2: state_sr = 10 -> HitLe=1, WriteLatMem fires
    
    # Now assert Trig - counter_last will fire on next clock (latency=1)
    dut.Trig.value = 1
    await RisingEdge(dut.Clk)  # clock 3: counter_last fires, Trig is high -> triggered!
    dut.Trig.value = 0

    # Keep hit high for a few more clocks for ToT counting
    await wait_clks(dut, 3)  # total hit duration ~6 BX
    dut.Hit.value = 0

    # Wait for HitTe and latch
    await wait_clks(dut, 4)

    # Now the LatencyMemCell FSM should be in ReadyToRead state
    # Check DataToCoreEn (needs TokIn=0 and ReadyToRead)
    
    # TrigId matching: after triggered, counter gets overwritten with
    # {LatCnt[8:5], TrigId[4:0]}. Then req_to_read checks counter[4:0] == TrigIdReq[4:0].
    # We set TrigId=0 and TrigIdReq=0, so this should match immediately.
    
    # Wait for the FSM to reach ReadyToRead (trig_state=1, start_state=0)
    # After triggered: start_state<=1, trig_state<=1 (TRIGGERED state)
    # Next cycle: req_to_read = start_state & trig_state & trig_id_match = 1 (if IDs match)
    #   -> trig_state<=1, start_state<=0 (since WriteLatMem=0 and triggered=0)
    # So ReadyToRead = trig_state & !start_state = 1
    
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
    # TEST 2: Multiple pixels hit simultaneously with known duration
    # =========================================================================
    dut._log.info("="*60)
    dut._log.info("TEST 2: Hit pixels 0,1,2,3 for 5 BX clocks")
    dut._log.info("="*60)

    # Inject hit on all 4 pixels
    dut.Hit.value = 0b1111
    await RisingEdge(dut.Clk)  # hit_rise captured
    await RisingEdge(dut.Clk)  # HitLe fires, WriteLatMem fires

    # Assert trigger at the right moment
    dut.Trig.value = 1
    await RisingEdge(dut.Clk)  # counter_last fires with Trig high
    dut.Trig.value = 0

    # Keep hit high for more ToT
    await wait_clks(dut, 4)  # total ~7 BX of hit
    dut.Hit.value = 0

    # Wait for trailing edge + latch + FSM propagation
    await wait_clks(dut, 10)

    # Read
    dut.Read.value = 1
    await RisingEdge(dut.Clk)
    try:
        data2 = dut.DataToCore.value.integer
    except ValueError:
        data2 = 0
        dut._log.warning("DataToCore has X/Z values (test 2)")
    dut.Read.value = 0

    tots2 = decode_region_data(data2)
    dut._log.info(f"DataToCore = 0x{data2:04x}, ToT values = {tots2}")
    for p in range(4):
        dut._log.info(f"  Pixel {p} ToT = {tots2[p]}")

    await wait_clks(dut, 5)

    # =========================================================================
    # TEST 3: Sequential hits with different durations
    # =========================================================================
    dut._log.info("="*60)
    dut._log.info("TEST 3: Sequential hits with varying duration")
    dut._log.info("="*60)

    test_durations = [2, 4, 6, 8]  # BX clocks per pixel

    for px_idx, dur in enumerate(test_durations):
        dut._log.info(f"  Hitting pixel {px_idx} for {dur} BX")
        px_mask = 1 << px_idx

        dut.Hit.value = px_mask
        await RisingEdge(dut.Clk)
        await RisingEdge(dut.Clk)  # HitLe / WriteLatMem

        dut.Trig.value = 1
        await RisingEdge(dut.Clk)  # triggered
        dut.Trig.value = 0

        # Keep hit for remaining duration
        remaining = dur - 3  # we already consumed 3 clocks
        if remaining > 0:
            await wait_clks(dut, remaining)
        dut.Hit.value = 0

        # Wait for TE + latch + ReadyToRead
        await wait_clks(dut, 10)

        # Read
        dut.Read.value = 1
        await RisingEdge(dut.Clk)
        try:
            data_px = dut.DataToCore.value.integer
        except ValueError:
            data_px = 0
        dut.Read.value = 0

        tots_px = decode_region_data(data_px)
        dut._log.info(f"    DataToCore = 0x{data_px:04x}, pixel {px_idx} ToT = {tots_px[px_idx]}")

        await wait_clks(dut, 5)

    # =========================================================================
    # TEST 4: Verify no-hit pixels report code 15
    # =========================================================================
    dut._log.info("="*60)
    dut._log.info("TEST 4: Single pixel 2 hit, verify others get code 15")
    dut._log.info("="*60)

    dut.Hit.value = 0b0100  # pixel 2 only
    await RisingEdge(dut.Clk)
    await RisingEdge(dut.Clk)

    dut.Trig.value = 1
    await RisingEdge(dut.Clk)
    dut.Trig.value = 0

    await wait_clks(dut, 5)  # ~8 BX total hit
    dut.Hit.value = 0

    await wait_clks(dut, 10)

    dut.Read.value = 1
    await RisingEdge(dut.Clk)
    try:
        data4 = dut.DataToCore.value.integer
    except ValueError:
        data4 = 0
    dut.Read.value = 0

    tots4 = decode_region_data(data4)
    dut._log.info(f"DataToCore = 0x{data4:04x}")
    for p in range(4):
        label = "HIT" if p == 2 else "no-hit (expect 15)"
        dut._log.info(f"  Pixel {p} ToT = {tots4[p]}  [{label}]")

    # Check that non-hit pixels have code 15
    for p in [0, 1, 3]:
        if tots4[p] == 15:
            dut._log.info(f"  PASS: Pixel {p} correctly reports 15 (no hit)")
        else:
            dut._log.warning(f"  UNEXPECTED: Pixel {p} reports {tots4[p]}, expected 15")

    await wait_clks(dut, 10)
    dut._log.info("All tests complete.")