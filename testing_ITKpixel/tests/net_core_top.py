import cocotb

from cocotb.handle import Force, Release
from cocotb.triggers import FallingEdge, RisingEdge, Timer


# ============================================================
# Test config
# ============================================================

CLK_PERIOD = 400  # ps

# Must match the Verilog definitions.
PACKET_SIZE = 30

UP = 0
RIGHT = 3

ROUTE_RIGHT = 0b1000

# NETWORK_MEM_DEPTH-dependent: a neighbor buf_status that RouterStub reads as
# "full enough to prefer routing there". Adjust to match RouterStub's policy.
NEIGHBOR_EMPTY = 0
NEIGHBOR_HAS_ROOM = 1


# ============================================================
# Clock / wait helpers
# ============================================================

async def generate_clock(dut):
    """Free-running clock on ClkIn."""
    while True:
        dut.ClkIn.value = 0
        await Timer(CLK_PERIOD // 2, unit="ps")
        dut.ClkIn.value = 1
        await Timer(CLK_PERIOD // 2, unit="ps")


async def wait_rising_edges(dut, n: int) -> None:
    """Wait for n rising edges of ClkIn."""
    for _ in range(n):
        await RisingEdge(dut.ClkIn)


async def initialize_inputs(dut) -> None:
    # --- digital-core / config side: hold quiescent ---
    dut.ResetIn_b.value = 1
    dut.AnaHit.value = 0
    dut.CoreRowAddrIn.value = 0
    dut.CalEdgeIn.value = 0
    dut.CalAuxIn.value = 0
    dut.LatCntIn.value = 0
    dut.LatCntReqIn.value = 0
    dut.TrigIn.value = 0
    dut.TrigClearIn.value = 0

    # --- neighbor buffer status (RouterStub inputs) ---
    dut.buf_status_up.value = 0
    dut.buf_status_dn.value = 0
    dut.buf_status_l.value = 0
    dut.buf_status_r.value = 0

    # --- neighbor valid handshake inputs ---
    dut.valid_up_in.value = 0
    dut.valid_dn_in.value = 0
    dut.valid_l_in.value = 0
    dut.valid_r_in.value = 0

    # --- NETWORKSIM local-core source hooks: MUST be driven ---
    # Undriven, core_mem_valid_tb would be X and poison the write-decision
    # always_comb (valid_arr[0], network_m_Wen, slot_src).
    dut.core_mem_valid_tb.value = 0
    dut.local_data_packet_tb.value = 0

    # Let the initial deposits settle before the first edge.
    await Timer(1, unit="ps")


async def reset_dut(dut) -> None:
    dut.ResetIn_b.value = 0
    await wait_rising_edges(dut, 3)
    dut.ResetIn_b.value = 1
    await wait_rising_edges(dut, 2)
    # End on a writable phase, not ReadOnly.


def get_direction_signals(dut, direction: int):
    if direction == UP:
        return dut.valid_up_in, dut.data_bus_up
    raise ValueError(f"Direction not supported in base test: {direction}")


async def inject_network_packet(dut, direction: int, packet: int) -> None:
    """
    Emulate a neighboring core sending one packet.

    Timeline:
        drive valid/data
        falling edge: transact_n captures ownership
        combinational: read_n -> valid_arr -> network_m_Wen
        rising edge: network_mem captures packet
    """
    valid_signal, bus_signal = get_direction_signals(dut, direction)

    packet &= (1 << PACKET_SIZE) - 1

    # A persistent external driver is required for an inout wire.
    bus_signal.value = Force(packet)
    valid_signal.value = 1

    # Allow the external bus drive and valid to settle before arbitration.
    await Timer(1, unit="ps")

    dut._log.info(
        f"Before negedge: valid_in={valid_signal.value}, "
        f"bus={bus_signal.value}"
    )

    # transact_n samples valid_o ^ valid_in here.
    await FallingEdge(dut.ClkIn)

    # Wait for the nonblocking assignment to transact_n and the downstream
    # combinational logic to settle.
    await Timer(1, unit="ps")

    dut._log.info(
        f"After negedge: transact_n={dut.transact_n.value}, "
        f"read_n={dut.read_n.value}, "
        f"valid_arr={dut.valid_arr.value}, "
        f"Wen={dut.network_m_Wen.value}, "
        f"slot_src[0]={dut.slot_src[0].value}, "
        f"bus_in={dut.data_bus_up_in.value}"
    )

    assert signal_to_int(dut.read_n) & 0b0001, (
        f"UP receive grant was not asserted: read_n={dut.read_n.value}"
    )

    assert signal_to_int(dut.network_m_Wen) != 0, (
        "No network-memory write enable became active after the negedge"
    )

    # Packet is captured here.
    await RisingEdge(dut.ClkIn)

    # RisingEdge returns before all NBA and combinational consequences are
    # necessarily visible, so explicitly wait for settling.
    await Timer(1, unit="ps")

    dut._log.info(
        f"After write posedge: free={dut.network_m_free.value}, "
        f"count={dut.network_m_cnt.value}, "
        f"mem0={dut.network_mem[0].value}"
    )

    # Release only after the packet has been captured.
    valid_signal.value = 0
    bus_signal.value = Release()

    await Timer(1, unit="ps")


def get_output_handles(dut, route: int):
    if route == ROUTE_RIGHT:
        return dut.valid_r_o, dut.data_bus_r
    raise ValueError(f"Route not supported in base test: 0b{route:04b}")


async def route_one_packet(dut, route: int) -> int:
    valid_o, output_bus = get_output_handles(dut, route)

    # No neighbors are attempting to transmit into this core.
    dut.valid_up_in.value = 0
    dut.valid_dn_in.value = 0
    dut.valid_l_in.value = 0
    dut.valid_r_in.value = 0

    dut.core_mem_valid_tb.value = 0
    dut.local_data_packet_tb.value = 0

    # Select the output direction.
    dut.routing_decision.value = Force(route)
    await Timer(1, unit="ps")

    # valid_r_o is combinational. It should assert immediately because
    # network_m_cnt is already 1.
    dut._log.info(
        f"before transmit negedge: "
        f"free={dut.network_m_free.value} "
        f"count={dut.network_m_cnt.value} "
        f"local_data_valid={dut.local_data_valid.value} "
        f"routing_decision={dut.routing_decision.value} "
        f"valid_r_o={valid_o.value}"
    )

    assert signal_to_int(dut.network_m_cnt) == 1, (
        f"Expected one buffered packet before routing, "
        f"got count={dut.network_m_cnt.value}"
    )

    assert signal_to_int(valid_o) == 1, (
        f"Output valid did not assert before transaction: "
        f"route={dut.routing_decision.value}, "
        f"local_data_valid={dut.local_data_valid.value}, "
        f"count={dut.network_m_cnt.value}"
    )

    # On this falling edge, transact_n[RIGHT] captures:
    # valid_r_o ^ valid_r_in = 1 ^ 0 = 1.
    await FallingEdge(dut.ClkIn)
    await Timer(1, unit="ps")

    dut._log.info(
        f"after transmit negedge: "
        f"transact_n={dut.transact_n.value} "
        f"assert_n={dut.assert_n.value} "
        f"selected={dut.selected_data_packet.value} "
        f"bus={output_bus.value}"
    )

    assert signal_to_int(dut.transact_n) & ROUTE_RIGHT, (
        f"RIGHT transaction was not registered: "
        f"transact_n={dut.transact_n.value}"
    )

    assert signal_to_int(dut.assert_n) & ROUTE_RIGHT, (
        f"RIGHT output ownership was not asserted: "
        f"assert_n={dut.assert_n.value}"
    )

    # The output bus is actively driven between this falling edge and
    # the following rising edge.
    packet = signal_to_int(output_bus)

    # The selected memory entry is cleared at this rising edge.
    await RisingEdge(dut.ClkIn)
    await Timer(1, unit="ps")

    dut._log.info(
        f"after transmit posedge: "
        f"free={dut.network_m_free.value} "
        f"count={dut.network_m_cnt.value} "
        f"valid_r_o={valid_o.value}"
    )

    dut.routing_decision.value = Release()
    await Timer(1, unit="ps")

    return packet


def signal_to_int(signal) -> int:
    """Convert a resolvable signal to an unsigned integer."""
    val = signal.value
    if not val.is_resolvable:
        raise AssertionError(f"{signal._path} contains X/Z: {val}")
    # LogicArray (multi-bit) path
    if hasattr(val, "to_unsigned"):
        return val.to_unsigned()
    # Logic (scalar) path
    return int(val)


# ============================================================
# Tests
# ============================================================

@cocotb.test()
async def network_layer_base_test(dut):
    """Base case: reset, receive one packet from UP, route it to RIGHT."""

    clk_task = cocotb.start_soon(generate_clock(dut))

    try:
        await initialize_inputs(dut)
        await reset_dut(dut)

        packet_up = 0x1234ABCD

        await inject_network_packet(dut, direction=UP, packet=packet_up)

        occupancy = signal_to_int(dut.buf_status_self)
        assert occupancy == 1, f"Expected occupancy 1, got {occupancy}"
        dut._log.info(f"Received UP packet 0x{packet_up:x}; occupancy={occupancy}")

        observed = await route_one_packet(dut, route=ROUTE_RIGHT)
        assert observed == packet_up, (
            f"Routed packet mismatch: got 0x{observed:x}, expected 0x{packet_up:x}"
        )

        occupancy = signal_to_int(dut.buf_status_self)
        assert occupancy == 0, f"Expected empty buffer, got {occupancy}"
        dut._log.info(f"Routed packet 0x{observed:x} to RIGHT; buffer drained")

        await wait_rising_edges(dut, 3)  # let counters stabilize

    finally:
        # Kill background tasks to reduce simulator exit weirdness.
        if clk_task is not None:
            clk_task.cancel()


@cocotb.test()
async def local_bypass_test(dut):
    """Bypass path: network empty, local core presents a packet, route to RIGHT."""

    clk_task = cocotb.start_soon(generate_clock(dut))

    try:
        await initialize_inputs(dut)
        await reset_dut(dut)

        # Network is empty after reset -> core_mem_passthrough = &network_m_free = 1.
        bypass_packet = 0x0BADF00D & ((1 << PACKET_SIZE) - 1)

        dut.local_data_packet_tb.value = bypass_packet
        dut.core_mem_valid_tb.value = 1
        dut.routing_decision.value = Force(ROUTE_RIGHT)

        dut.valid_up_in.value = 0
        dut.valid_dn_in.value = 0
        dut.valid_l_in.value = 0
        dut.valid_r_in.value = 0

        await Timer(1, unit="ps")
        dut._log.info(
            f"bypass: valid_r_o={dut.valid_r_o.value} "
            f"selected={dut.selected_data_packet.value}"
        )

        await FallingEdge(dut.ClkIn)
        await Timer(1, unit="ps")
        observed = signal_to_int(dut.data_bus_r)

        assert observed == bypass_packet, (
            f"Bypass mismatch: got 0x{observed:x}, expected 0x{bypass_packet:x}"
        )
        dut._log.info(f"Local bypass routed 0x{observed:x} to RIGHT")

        # Clean teardown so nothing is left forced/asserted.
        dut.core_mem_valid_tb.value = 0
        dut.local_data_packet_tb.value = 0
        dut.routing_decision.value = Release()
        await Timer(1, unit="ps")

        await wait_rising_edges(dut, 3)  # let counters stabilize

    finally:
        # Kill background tasks to reduce simulator exit weirdness.
        if clk_task is not None:
            clk_task.cancel()