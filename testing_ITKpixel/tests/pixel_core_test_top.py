import cocotb
from cocotb.triggers import RisingEdge, Timer


# ============================================================
# Test config
# ============================================================

TOT6TO4 = True
TOT_DUAL_EDGE = False

CLK_PERIOD = 400  # ps; fast sim clock. Real 40 MHz would be 25000 ps.

LATENCY_COUNTER_BITS = 9

# For clean single-hit test, use one pixel only.
# Old value 63 = 0b111111, which fires pixels 0 through 5.
PIXEL_MASK = 0x1


# ============================================================
# Clock / counter helpers
# ============================================================

async def generate_clock(dut):
    """Free-running clock on DigitalCore ClkIn."""
    while True:
        dut.ClkIn.value = 0
        await Timer(CLK_PERIOD // 2, unit="ps")
        dut.ClkIn.value = 1
        await Timer(CLK_PERIOD // 2, unit="ps")


def bin_to_gray(val, bits):
    """Convert binary integer to Gray code."""
    g = val ^ (val >> 1)
    return g & ((1 << bits) - 1)


async def generate_latency_counters(dut):
    """
    Drive LatCntIn and LatCntReqIn as Gray-encoded counters.
    Trigger latency = 20 BX.
    """
    bx = 20
    while True:
        await RisingEdge(dut.ClkIn)
        bx = bx + 1
        dut.LatCntIn.value = bin_to_gray(bx, LATENCY_COUNTER_BITS)
        bx_req = bx - 20
        dut.LatCntReqIn.value = bin_to_gray(bx_req, LATENCY_COUNTER_BITS)


async def wait_clks(dut, n):
    """Wait for n rising edges of ClkIn."""
    for _ in range(n):
        await RisingEdge(dut.ClkIn)


async def reset_dut(dut):
    """Apply synchronous active-low reset."""
    dut.ResetIn_b.value = 1
    await RisingEdge(dut.ClkIn)

    dut.ResetIn_b.value = 0
    await wait_clks(dut, 5)

    dut.ResetIn_b.value = 1
    await wait_clks(dut, 2)


def decode_region_data(data_16bit):
    """Extract 4 x 4-bit ToT values from the 16-bit region data bus."""
    tots = []
    for p in range(4):
        tot = (data_16bit >> (4 * p)) & 0xF
        tots.append(tot)
    return tots


# ============================================================
# Safe value helpers
# ============================================================

def val_has_unknown(obj):
    """Detect X/Z/U/? in a signal value string."""
    s = str(obj.value).lower()
    return ("x" in s) or ("z" in s) or ("u" in s) or ("?" in s)


def val_unsigned(obj):
    """
    Convert cocotb signal value to unsigned int.
    Returns None if signal contains X/Z or cannot be converted.
    """
    try:
        if val_has_unknown(obj):
            return None

        v = obj.value

        if hasattr(v, "to_unsigned"):
            return v.to_unsigned()

        s = str(v)
        if all(c in "01" for c in s):
            return int(s, 2)

        return int(s)

    except Exception:
        return None



# ============================================================
# Main test
# ============================================================

@cocotb.test()
async def pixel_test(dut):
    """Inject one hit into DigitalCore and check readout propagation to RegionDataOut."""

    clk_task = cocotb.start_soon(generate_clock(dut))
    cnt_task = None

    try:
        # ------------------------------------------------------------
        # Static signal initialization
        # ------------------------------------------------------------
        dut.ResetIn_b.value          = 1
        dut.AnaHit.value             = 0

        dut.CoreRowAddrIn.value      = 0

        dut.AnaInjectionModeIn.value = 0
        dut.EnDigHitIn.value         = 0
        dut.Tot6to4MappingIn.value   = 1 if TOT6TO4 else 0
        dut.TotDualEdgeCountIn.value = 1 if TOT_DUAL_EDGE else 0
        dut.HitSampleModeIn.value    = 0
        dut.EnSeuCountIn.value       = 0

        dut.PixelConfDefaultIn.value = 1
        dut.PixelConfAddrIn.value    = 0
        dut.PixelConfWr5bitIn.value  = 0
        dut.PixelConfWr3bitIn.value  = 0
        dut.PixelConfDataWrIn.value  = 0
        dut.PixelConfDataRdIn.value  = 0

        dut.CalEdgeIn.value          = 0
        dut.CalAuxIn.value           = 0

        dut.HitOrIn.value            = 0

        dut.LatCntIn.value           = 0
        dut.LatCntReqIn.value        = 0

        dut.TrigIn.value             = 0
        dut.TrigClearIn.value        = 0
        dut.TrigIdIn.value           = 0
        dut.TrigIdReqIn.value        = 0

        dut.TokIn.value              = 0
        dut.ReadIn.value             = 0

        dut.RegionAddrIn.value       = 0
        dut.RegionDataIn.value       = 0

        # Optional analog front-end bias inout pins.
        # These only exist depending on ATLAS_CHIP / CMS_CHIP compile defines.
        # Drive to 0 for deterministic simulation unless another model drives them.
        def set_if_exists(sig_name, value=0):
            try:
                getattr(dut, sig_name).value = value
            except AttributeError:
                pass

        # ATLAS_CHIP analog inouts
        set_if_exists("VBP_PREAMP_A", 0)
        set_if_exists("VBP_PREAMP_B", 0)
        set_if_exists("VBN_COMP", 0)
        set_if_exists("VBN_PRECOMP", 0)
        set_if_exists("VTH1", 0)
        set_if_exists("VTH2", 0)
        set_if_exists("VBN_LCC", 0)
        set_if_exists("VBP_VFF", 0)
        set_if_exists("VCTRL_CF0", 0)
        set_if_exists("VCTRL_LCC", 0)
        set_if_exists("VCAL_HI", 0)
        set_if_exists("VCAL_MI", 0)

        # CMS_CHIP analog inouts
        set_if_exists("IPA_A", 0)
        set_if_exists("IPA_B", 0)
        set_if_exists("ICOMP", 0)
        set_if_exists("ICOMP_STAR", 0)
        set_if_exists("VTH", 0)
        set_if_exists("VREF_KRUM", 0)
        set_if_exists("IHU_KRUM", 0)
        set_if_exists("IHD_KRUM", 0)
        set_if_exists("IFC", 0)
        set_if_exists("ILDAC_MIR", 0)

        await Timer(1, unit="ns")

        # ------------------------------------------------------------
        # Reset
        # ------------------------------------------------------------
        await reset_dut(dut)
        dut._log.info("Reset complete")

        # ------------------------------------------------------------
        # Start latency counters
        # ------------------------------------------------------------
        cnt_task = cocotb.start_soon(generate_latency_counters(dut))
        await wait_clks(dut, 3)

        # ------------------------------------------------------------
        # TEST 1: Single hit into pixel 0 / region 0
        # ------------------------------------------------------------
        dut._log.info("=" * 60)
        dut._log.info("TEST 1: Hit pixel 0 for 4 BX clocks")
        dut._log.info("=" * 60)

        hit_duration = 4
        pixel_mask = PIXEL_MASK

        # Keep trigger ID stable for this simple test.
        dut.TrigIdIn.value = 1
        dut.TrigIdReqIn.value = 0

        # Inject hit.
        dut.AnaHit.value = pixel_mask
        await wait_clks(dut, 1)

        latcnt_time = str(dut.LatCntIn.value)
        dut._log.info(f"Injected AnaHit mask = {pixel_mask:#x}")
        dut._log.info(f"Captured hit LatCntIn = {latcnt_time}")

        await wait_clks(dut, hit_duration - 1)
        dut.AnaHit.value = 0

        # ------------------------------------------------------------
        # Wait until LatCntReqIn matches captured LatCntIn
        # ------------------------------------------------------------
        matched = False
        for _ in range(200):
            if str(dut.LatCntReqIn.value) == latcnt_time:
                matched = True
                break
            await RisingEdge(dut.ClkIn)

        if not matched:
            dut._log.error(f"Timeout waiting for LatCntReqIn to match captured LatCntIn={latcnt_time}")
            assert False, "Latency counter request never matched captured hit timestamp"

        dut._log.info(f"Latency match found: LatCntReqIn={dut.LatCntReqIn.value}")

        # ------------------------------------------------------------
        # Issue trigger
        # ------------------------------------------------------------
        dut.TrigIdIn.value = 1
        dut.TrigIn.value = 1
        await RisingEdge(dut.ClkIn)
        dut.TrigIn.value = 0

        dut._log.info("Trigger issued")

        # Wait for region data to become valid.
        await wait_clks(dut, 8)

        # ------------------------------------------------------------
        # Select trigger ID and assert ReadIn
        # ------------------------------------------------------------
        dut.TrigIdReqIn.value = 1
        dut.ReadIn.value = 1
        # Clock read through.
        await RisingEdge(dut.ClkIn)
        await RisingEdge(dut.ClkIn)

        if val_has_unknown(dut.RegionDataOut):
            dut._log.error(f"RegionDataOut has X/Z values: {dut.RegionDataOut.value}")
            assert False, "RegionDataOut has X/Z values"

        data = dut.RegionDataOut.value.to_unsigned()
        tots = decode_region_data(data)

        # Region 0 child output is useful as a compact sanity check, but
        # this test primarily reports the top-level RegionDataOut result.
        region0_data = dut.PixelRegion[0].PixelRegionLogic.DataToCore.value

        dut._log.info("-" * 60)
        dut._log.info("RESULT")
        dut._log.info(f"Region 0 DataToCore = {region0_data}")
        dut._log.info(f"RegionDataOut       = {dut.RegionDataOut.value}")
        dut._log.info(f"Decoded RegionDataOut = 0x{data:04x}")
        dut._log.info(f"ToT values [pixel0, pixel1, pixel2, pixel3] = {tots}")
        dut._log.info("-" * 60)

        if tots[0] == 0:
            dut._log.error("Pixel 0 ToT was zero after injecting a hit into pixel 0")
            assert False, "Expected nonzero ToT for pixel 0"

        if any(tot != 15 for tot in tots[1:]):
            dut._log.error("Unexpected nonzero ToT in pixels 1-3 (code F/15 means 0)")
            assert False, "Expected only pixel 0 to have nonzero ToT"

        # ------------------------------------------------------------
        # Deassert read/request
        # ------------------------------------------------------------
        dut.ReadIn.value = 0
        dut.TrigIdReqIn.value = 0

        await wait_clks(dut, 5)

        dut._log.info("PASS: Single pixel hit propagated to top-level RegionDataOut.")

    # CHAT CAME UP WITH THIS WITHOUT IT I CAN"T GET THE WAVEFORM OUT ICARUS WILL CRASH ON EXIT!!!!!!!!!
    finally:
        # Kill background tasks to reduce simulator exit weirdness.
        if cnt_task is not None:
            cnt_task.kill()
        if clk_task is not None:
            clk_task.kill()
