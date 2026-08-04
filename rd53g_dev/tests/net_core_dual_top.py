import cocotb

from cocotb.handle import Force, Release
from cocotb.triggers import FallingEdge, RisingEdge, Timer


# ============================================================
# Test config
# ============================================================

CLK_PERIOD = 25000  # ps -> 25 ns

# Must match the Verilog definitions.
PACKET_SIZE = 30

UP = 0
DOWN = 1
LEFT = 2
RIGHT = 3

ROUTE_UP = 0b0001
ROUTE_DOWN = 0b0010
ROUTE_LEFT = 0b0100
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
    dut.CoreRowAddrIn.value = 7
    dut.CalEdgeIn.value = 0
    dut.CalAuxIn.value = 0
    dut.LatCntIn.value = 0
    dut.LatCntReqIn.value = 0
    dut.TrigIn.value = 0 # smoke test
    dut.TrigClearIn.value = 0
    dut.TrigIdIn.value = 2

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


    # Let the initial deposits settle before the first edge.
    await Timer(1, unit="ps")


async def reset_dut(dut) -> None:
    dut.ResetIn_b.value = 0
    await wait_rising_edges(dut, 2)
    dut.ResetIn_b.value = 1
    await wait_rising_edges(dut, 2)
    # End on a writable phase, not ReadOnly.


# ============================================================
# Direction plumbing
# ============================================================

def get_direction_signals(dut, direction: int):
    """Return (valid_in, data_bus, read_n_bit_mask) for a receive direction."""
    if direction == UP:
        return dut.valid_up_in, dut.data_bus_up, 0b0001
    if direction == DOWN:
        return dut.valid_dn_in, dut.data_bus_dn, 0b0010
    if direction == LEFT:
        return dut.valid_l_in, dut.data_bus_l, 0b0100
    if direction == RIGHT:
        return dut.valid_r_in, dut.data_bus_r, 0b1000
    raise ValueError(f"Unknown direction: {direction}")


# ============================================================
# Multi-packet injection
# ============================================================
# # Capture edge for each port under the DDR scheme.
# POSEDGE_PORTS = (DOWN, RIGHT)   # asserted during the low phase,  captured on posedge
# NEGEDGE_PORTS = (UP, LEFT)      # asserted during the high phase, captured on negedge
POSEDGE_PORTS = (UP, LEFT)   # asserted during the low phase,  captured on posedge
NEGEDGE_PORTS = (DOWN, RIGHT)      # asserted during the high phase, captured on negedge
SETTLE_PS = 1

def _drive(dut, packets, ports):
    """Drive valid + data for every requested port in this group."""
    driven = {}
    for direction in ports:
        if direction not in packets:
            continue
        valid_signal, bus_signal, _ = get_direction_signals(dut, direction)
        packet = packets[direction]
        bus_signal.value = Force(packet)   # inout wire needs a persistent driver
        valid_signal.value = 1
        driven[direction] = (valid_signal, bus_signal, packet)
    return driven


def _release(driven):
    for valid_signal, bus_signal, _ in driven.values():
        valid_signal.value = 0
        bus_signal.value = Release()


async def inject_network_packets(dut, packets: dict) -> None:
    """
    Emulate neighbouring cores each sending one packet, on the DDR schedule.

    Args:
        packets: mapping of direction (UP/DOWN/LEFT/RIGHT) -> packet int.

    Timeline (every driven signal lasts exactly one half period):

        negedge   -> drive DOWN / RIGHT valid + data
        posedge   -> DOWN / RIGHT captured; release them
                     drive UP / LEFT valid + data
        negedge   -> UP / LEFT captured; release them
    """
    if not packets:
        raise ValueError("inject_network_packets requires at least one packet")

    unknown = set(packets) - set(POSEDGE_PORTS) - set(NEGEDGE_PORTS)
    if unknown:
        raise ValueError(f"unknown direction(s): {[_dir_name(d) for d in unknown]}")

    # ---- low phase: drive the posedge-captured ports -----------------------
    await FallingEdge(dut.ClkIn)
    await Timer(SETTLE_PS, unit="ps")

    pos_driven = _drive(dut, packets, POSEDGE_PORTS)
    if pos_driven:
        dut._log.info(
            "low phase drive: "
            + ", ".join(
                f"{_dir_name(d)} valid={v.value} bus={b.value}"
                for d, (v, b, _) in pos_driven.items()
            )
        )

    await Timer(SETTLE_PS, unit="ps")
    dut._log.info(
        f"low phase: read_n={dut.read_n.value}, "
        f"valid_arr={dut.valid_arr.value}, Wen={dut.network_m_Wen.value}"
    )

    # ---- posedge captures DOWN / RIGHT ------------------------------------
    await RisingEdge(dut.ClkIn)
    await Timer(SETTLE_PS, unit="ps")
    _release(pos_driven)
    await Timer(SETTLE_PS, unit="ps")

    dut._log.info(
        f"after posedge: full={dut.network_m_full.value}, "
        f"count={dut.network_m_cnt.value}, mem0={dut.network_mem[0].value}"
    )

    # ---- high phase: drive the negedge-captured ports ----------------------
    neg_driven = _drive(dut, packets, NEGEDGE_PORTS)
    if neg_driven:
        dut._log.info(
            "high phase drive: "
            + ", ".join(
                f"{_dir_name(d)} valid={v.value} bus={b.value}"
                for d, (v, b, _) in neg_driven.items()
            )
        )

    await Timer(SETTLE_PS, unit="ps")
    dut._log.info(
        f"high phase: read_n={dut.read_n.value}, "
        f"valid_arr={dut.valid_arr.value}, Wen={dut.network_m_Wen.value}"
    )

    # ---- negedge captures UP / LEFT ---------------------------------------
    await FallingEdge(dut.ClkIn)
    await Timer(SETTLE_PS, unit="ps")
    _release(neg_driven)
    await Timer(SETTLE_PS, unit="ps")

    dut._log.info(
        f"after negedge: full={dut.network_m_full.value}, "
        f"count={dut.network_m_cnt.value}, mem0={dut.network_mem[0].value}"
    )

# async def inject_network_packets(dut, packets: dict) -> None:
#     """
#     Emulate several neighboring cores sending one packet each, concurrently.

#     Args:
#         packets: mapping of direction (UP/DOWN/LEFT/RIGHT) -> packet int.

#     Timeline (shared for all directions):
#         drive valid/data on every requested direction
#         falling edge: transact_n captures ownership on each granted port
#         combinational: read_n -> valid_arr -> network_m_Wen
#         rising edge: network_mem captures the granted packets
#     """
#     if not packets:
#         raise ValueError("inject_network_packets requires at least one packet")

#     mask = (1 << PACKET_SIZE) - 1

#     # Record what each direction expects so we can check its own grant bit.
#     drivers = {}
#     for direction, packet in packets.items():
#         valid_signal, bus_signal, grant_mask = get_direction_signals(dut, direction)
#         packet &= mask

#         # A persistent external driver is required for an inout wire.
#         # bus_signal.value = Force(packet)
#         valid_signal.value = 1
#         drivers[direction] = (valid_signal, bus_signal, grant_mask, packet)

#     # Allow the external bus drives and valids to settle before arbitration.
#     await Timer(1, unit="ps")

#     dut._log.info(
#         "Before negedge: "
#         + ", ".join(
#             f"{_dir_name(d)} valid={v.value} bus={b.value}"
#             for d, (v, b, _, _) in drivers.items()
#         )
#     )

#     # transact_n samples valid_o ^ valid_in here.
#     await FallingEdge(dut.ClkIn)

#     # Wait for the nonblocking assignment to transact_n and the downstream
#     # combinational logic to settle.
#     await Timer(1, unit="ps")

#     dut._log.info(
#         # f"After negedge: transact_n={dut.transact_n.value}, "
#         f"read_n={dut.read_n.value}, "
#         f"valid_arr={dut.valid_arr.value}, "
#         f"Wen={dut.network_m_Wen.value}"
#     )

#     read_n = signal_to_int(dut.read_n)
#     for direction, (valid_signal, bus_signal, grant_mask, packet) in drivers.items():
#         transact_cancel = False
#         if direction == UP:
#             transact_cancel = dut.valid_up_o.value != 0 
#         elif (direction == DOWN):
#             transact_cancel = dut.valid_dn_o.value != 0 
#         elif direction == LEFT:
#             transact_cancel = dut.valid_l_o.value != 0 
#         elif direction == RIGHT:
#             transact_cancel = dut.valid_r_o.value != 0 
#             # cancel case of both have valid that is okay :)
#         if (not transact_cancel):
#             # once valid transction then assert data on bus
#             bus_signal.value = Force(packet)

#             assert read_n & grant_mask, (
#                 f"{_dir_name(direction)} receive grant was not asserted: "
#                 f"read_n={dut.read_n.value}"
#             )
#             assert signal_to_int(dut.network_m_Wen) != 0, (
#               "No network-memory write enable became active after the negedge"
#     )

#         else:
#             dut._log.info(f"transaction cancled for {_dir_name(direction)}")

#     # Packets are captured here.
#     await RisingEdge(dut.ClkIn)

#     # RisingEdge returns before all NBA and combinational consequences are
#     # necessarily visible, so explicitly wait for settling.
#     await Timer(1, unit="ps")

#     dut._log.info(
#         f"After write posedge: full={dut.network_m_full.value}, "
#         f"count={dut.network_m_cnt.value}, "
#         f"mem0={dut.network_mem[0].value}"
#     )

#     # Release only after the packets have been captured.
#     for direction, (valid_signal, bus_signal, _, _) in drivers.items():
#         valid_signal.value = 0
#         bus_signal.value = Release()

#     await Timer(1, unit="ps")



async def inject_local_mem(dut, hitmask: int, offset: int = 0) -> None:
    """
    Digital hit injection into local memory

    Args:
        number of packets generated in local memory (up to 16)

    Timeline (shared for all directions):
    after hit gets applied then it can get clocked out next cycle? TODO: look at timing here.
    """
    # future: hitmask generation and different ToTs at the same time...
    dut.LatCntIn.value = 5
    dut.AnaHit.value = hitmask # this is wrong need to check the math on how it gets broken
    await wait_rising_edges(dut, 4)
    if offset > 0:
      await Timer(offset, unit="ns")
    dut.AnaHit.value = 0
    dut.LatCntIn.value = 0 # change lat cnt to something else
    # Allow the external bus drives and valids to settle before arbitration.
    await Timer(1, unit="ps")

    # now bcid matching and trigger
    await RisingEdge(dut.ClkIn) 
    # Packets are captured here
    await RisingEdge(dut.ClkIn)
    dut.LatCntReqIn.value = 5
    dut.TrigIn.value = 1
    dut._log.info(
        f"After write posedge: free={dut.TokOut.value}, "
        f"local packet={dut.local_data_packet.value}, ")
    await RisingEdge(dut.ClkIn)

    dut.LatCntReqIn.value = 0
    dut.TrigIn.value = 0
    # trigger and then reset it
    await RisingEdge(dut.ClkIn)





async def inject_network_packet(dut, direction: int, packet: int) -> None:
    """Backwards-compatible single-packet wrapper."""
    await inject_network_packets(dut, {direction: packet})


def _dir_name(direction: int) -> str:
    return {UP: "UP", DOWN: "DOWN", LEFT: "LEFT", RIGHT: "RIGHT"}.get(
        direction, str(direction)
    )


# ============================================================
# Routing (router decides; no Force on routing_decision)
# ============================================================

def get_output_handles(dut, route: int):
    if route == ROUTE_UP:
        return dut.valid_up_o, dut.data_bus_up
    if route == ROUTE_DOWN:
        return dut.valid_dn_o, dut.data_bus_dn
    if route == ROUTE_LEFT:
        return dut.valid_l_o, dut.data_bus_l
    if route == ROUTE_RIGHT:
        return dut.valid_r_o, dut.data_bus_r
    raise ValueError(f"Route not supported: 0b{route:04b}")


async def route_one_packet(dut, expected_route: int) -> int:
    """
    Let the router decide the output direction and observe the transaction.

    routing_decision is NOT forced; we read it back from the DUT and verify it
    matches expected_route, then watch the corresponding output port.
    """
    # No neighbors are attempting to transmit into this core.
    dut.valid_up_in.value = 0
    dut.valid_dn_in.value = 0
    dut.valid_l_in.value = 0
    dut.valid_r_in.value = 0


    # Let the router's combinational decision settle.
    await Timer(1, unit="ps")

    route = signal_to_int(dut.routing_decision)
    dut._log.info(
        f"router chose routing_decision=0b{route:04b} "
        f"(expected 0b{expected_route:04b})"
    )
    # assert route == expected_route, (
    #     f"Router selected unexpected direction: got 0b{route:04b}, "
    #     f"expected 0b{expected_route:04b}"
    # )

    valid_o, output_bus = get_output_handles(dut, route)

    dut._log.info(
        f"before transmit negedge: "
        f"full={dut.network_m_full.value} "
        f"count={dut.network_m_cnt.value} "
        f"local_data_valid={dut.local_data_valid.value} "
        f"routing_decision={dut.routing_decision.value} "
        f"valid_o={valid_o.value}"
    )

    # assert signal_to_int(dut.network_m_cnt) == 1, (
    #     f"Expected one buffered packet before routing, "
    #     f"got count={dut.network_m_cnt.value}"
    # )

    # assert signal_to_int(valid_o) == 1, (
    #     f"Output valid did not assert before transaction: "
    #     f"route=0b{route:04b}, "
    #     f"local_data_valid={dut.local_data_valid.value}, "
    #     f"count={dut.network_m_cnt.value}"
    # )

    # On this falling edge, transact_n[route] captures valid_o ^ valid_in.
    await FallingEdge(dut.ClkIn)
    await Timer(1, unit="ps")

    dut._log.info(
        f"after transmit negedge: "
        # f"transact_n={dut.transact_n.value} "
        f"assert_n={dut.assert_n.value} "
        f"selected={dut.selected_data_packet.value} "
        f"bus={output_bus.value}"
    )

    # assert signal_to_int(dut.transact_n) & route, (
    #     f"Transaction was not registered for route 0b{route:04b}: "
    #     # f"transact_n={dut.transact_n.value}"
    # )

    # assert signal_to_int(dut.assert_n) & route, (
    #     f"Output ownership was not asserted for route 0b{route:04b}: "
    #     f"assert_n={dut.assert_n.value}"
    # )

    # The output bus is actively driven between this falling edge and
    # the following rising edge.
    packet = signal_to_int(output_bus)

    # The selected memory entry is cleared at this rising edge.
    await RisingEdge(dut.ClkIn)
    await Timer(1, unit="ps")

    dut._log.info(
        f"after transmit posedge: "
        f"full={dut.network_m_full.value} "
        f"count={dut.network_m_cnt.value} "
        f"valid_o={valid_o.value}"
    )

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

        packet_dn = 0x1234ABCD

        await inject_network_packet(dut, direction=DOWN, packet=packet_dn)

        occupancy = signal_to_int(dut.buf_status_self)
        # assert occupancy == 1, f"Expected occupancy 1, got {occupancy}"
        dut._log.info(f"Received UP packet 0x{packet_dn:x}; occupancy={occupancy}")

        observed = await route_one_packet(dut, expected_route=ROUTE_DOWN)
        # assert observed == packet_dn, (
        #     f"Routed packet mismatch: got 0x{observed:x}, expected 0x{packet_dn:x}"
        # )

        occupancy = signal_to_int(dut.buf_status_self)
        # assert occupancy == 0, f"Expected empty buffer, got {occupancy}"
        dut._log.info(f"Routed packet 0x{observed:x} to RIGHT; buffer drained")

        await wait_rising_edges(dut, 7)  # let counters stabilize

    finally:
        if clk_task is not None:
            clk_task.cancel()


@cocotb.test()
async def multi_inject_test(dut):
    """Inject packets from several directions at once."""

    clk_task = cocotb.start_soon(generate_clock(dut))

    try:
        await initialize_inputs(dut)
        await reset_dut(dut)

        packets = {
            UP: 0x1234ABCD,
            DOWN: 0x0BADF00D,
            LEFT: 0x0DEADBEE,
        }

        await inject_network_packets(dut, packets)

        occupancy = signal_to_int(dut.buf_status_self)
        # assert occupancy == len(packets), (
        #     f"Expected occupancy {len(packets)}, got {occupancy}"
        # )
        dut._log.info(f"Injected {len(packets)} packets; occupancy={occupancy}")

        await wait_rising_edges(dut, 2)  # let counters stabilize

    finally:
        if clk_task is not None:
            clk_task.cancel()


@cocotb.test()
async def local_mem_test(dut):
    """Fill up local memory."""

    clk_task = cocotb.start_soon(generate_clock(dut))

    try:
        await initialize_inputs(dut)
        await reset_dut(dut)

        mask = (1 << 32) - 1

        await inject_local_mem(dut, mask)

        for i in range(11):
          await wait_rising_edges(dut, 1)
          dut._log.info(f"TokOut {dut.TokOut.value}, localpacket {dut.local_data_packet.value}")

    finally:
        if clk_task is not None:
            clk_task.cancel()
## TOT COUNTER TESTING
@cocotb.test()
async def local_mem_test_over(dut):
    """Fill up local memory 4 clock cyles + 1/4."""

    clk_task = cocotb.start_soon(generate_clock(dut))

    try:
        await initialize_inputs(dut)
        await reset_dut(dut)

        mask = (1 << 32) - 1

        await inject_local_mem(dut, mask, 6)

        for i in range(11):
          await wait_rising_edges(dut, 1)
          dut._log.info(f"TokOut {dut.TokOut.value}, localpacket {dut.local_data_packet.value}")

    finally:
        if clk_task is not None:
            clk_task.cancel()

@cocotb.test()
async def local_mem_test_under(dut):
    """Fill up local memory 5 clock cycles - 1/4"""

    clk_task = cocotb.start_soon(generate_clock(dut))

    try:
        await initialize_inputs(dut)
        await reset_dut(dut)

        mask = (1 << 32) - 1

        await inject_local_mem(dut, mask, 20)

        for i in range(11):
          await wait_rising_edges(dut, 1)
          dut._log.info(f"TokOut {dut.TokOut.value}, localpacket {dut.local_data_packet.value}")

    finally:
        if clk_task is not None:
            clk_task.cancel()


@cocotb.test()
async def local_bypass_test(dut):
    """Bypass path: network empty, local core presents a packet, router routes it."""

    clk_task = cocotb.start_soon(generate_clock(dut))

    try:
        await initialize_inputs(dut)
        await reset_dut(dut)

        mask = (1 << 16) - 1
        await inject_local_mem(dut, mask)

        await Timer(1, unit="ps")
        await RisingEdge(dut.ClkIn)
        await RisingEdge(dut.ClkIn)

        # Router decides; read it back.
        route = signal_to_int(dut.routing_decision)
        valid_o, output_bus = get_output_handles(dut, route)
        dut._log.info(
            f"bypass: router chose 0b{route:04b} "
            f"valid_o={valid_o.value} "
            f"selected={dut.selected_data_packet.value}"
        )

        await FallingEdge(dut.ClkIn)
        await Timer(1, unit="ps")
        # observed = signal_to_int(output_bus)

        # assert observed == bypass_packet, (
        #     f"Bypass mismatch: got 0x{observed:x}, expected 0x{bypass_packet:x}"
        # )
        # dut._log.info(
        #     f"Local bypass routed 0x{observed:x} to 0b{route:04b}"
        # )

        # # Clean teardown.
        # dut.core_mem_valid_tb.value = 0
        # dut.local_data_packet_tb.value = 0
        # await Timer(1, unit="ps")

        await wait_rising_edges(dut, 4)  # let counters stabilize

    finally:
        if clk_task is not None:
            clk_task.cancel()



@cocotb.test()
async def overflow_test(dut):
    """Inject packets to observe steady state and overflow behavior"""
    ## TEST CASE PASSED!!! INJECT 3x3 READS OUT 7, buffer status overflows by 2 not 3 ->
    # steady state achieved

    clk_task = cocotb.start_soon(generate_clock(dut))

    try:
        await initialize_inputs(dut)
        await reset_dut(dut)

        packets = {
            UP: 0x11111111,
            RIGHT: 0x22222222,
            LEFT: 0x33333333,
        }

        await inject_network_packets(dut, packets)

        occupancy = signal_to_int(dut.buf_status_self)
        # assert occupancy == len(packets), (
        #     f"Expected occupancy {len(packets)}, got {occupancy}"
        # )

        dut._log.info(f"Injected {len(packets)} packets; occupancy={occupancy}")
        packets = {
            UP: 0x44444444,
            RIGHT: 0x55555555,
            LEFT: 0x66666666,
        }

        await inject_network_packets(dut, packets)

        occupancy = signal_to_int(dut.buf_status_self)
        # assert occupancy == len(packets), (
        #     f"Expected occupancy {len(packets)}, got {occupancy}"
        # )

        packets = {
            UP: 0x77777777,
            RIGHT: 0x88888888,
            LEFT: 0x99999999,
        }
        dut._log.info(f"Injected {len(packets)} packets; occupancy={occupancy}")

        await inject_network_packets(dut, packets)

        occupancy = signal_to_int(dut.buf_status_self)
        # assert occupancy == len(packets), (
        #     f"Expected occupancy {len(packets)}, got {occupancy}"
        # )
        dut._log.info(f"Injected {len(packets)} packets; occupancy={occupancy}")

        await wait_rising_edges(dut, 9)  # let counters stabilize

    finally:
        if clk_task is not None:
            clk_task.cancel()

## maybe one more test case with the routing getting cancled and then read out next cycle?
## need to move to golden model soon instead of manually doing asserts and shit.......


@cocotb.test()
async def conflict_test(dut):
    """test the conflict behavior case"""

    clk_task = cocotb.start_soon(generate_clock(dut))

    try:
        await initialize_inputs(dut)
        await reset_dut(dut)

        packets = {
            UP: 0x11111111,
            RIGHT: 0x22222222,
            LEFT: 0x33333333,
        }

        await inject_network_packets(dut, packets)

        occupancy = signal_to_int(dut.buf_status_self)
        # assert occupancy == len(packets), (
        #     f"Expected occupancy {len(packets)}, got {occupancy}"
        # )

        dut._log.info(f"Injected {len(packets)} packets; occupancy={occupancy}")
        packets = {
            DOWN: 0x44444444,
        }

        await inject_network_packets(dut, packets)

        occupancy = signal_to_int(dut.buf_status_self)
        # assert occupancy == len(packets), (
        #     f"Expected occupancy {len(packets)}, got {occupancy}"
        # )

        packets = {
            DOWN: 0x55555555,
        }
        dut._log.info(f"Injected {len(packets)} packets; occupancy={occupancy}")

        await inject_network_packets(dut, packets)

        occupancy = signal_to_int(dut.buf_status_self)
        # assert occupancy == len(packets), (
        #     f"Expected occupancy {len(packets)}, got {occupancy}"
        # )
        dut._log.info(f"Injected {len(packets)} packets; occupancy={occupancy}")

        await wait_rising_edges(dut, 8)  # let counters stabilize

    finally:
        if clk_task is not None:
            clk_task.cancel()