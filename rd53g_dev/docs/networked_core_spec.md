# NetworkedCore — Functional Specification for Golden Model
# NEEDS AUDIT, USED FOR FORMATTING/RESTRUCTURING

**Source RTL:** `rd53g_dev/rtl/NetworkedCore.sv`, `DigitalCore.sv`, `PixelRegionLogic.sv`,
`LatencyMem.sv`, `LatencyMemCell.sv`, `PixelLogic.sv`, `RouterStub.sv`, `defines.sv`

---

## 1. Overview

NetworkedCore wraps one DigitalCore (an 8×8 pixel sensor tile) and adds a network transport
layer. Each instantiated core:

1. Detects particle hits, measures their time-over-threshold (ToT), and assembles data packets
2. Buffers packets in a (Configurable but currently assumed to be) 5-slot local network memory
3. Receives packets arriving from up to four cardinal-direction neighbors over shared tristate buses
4. Routes one packet per clock cycle outward toward the chip periphery, selected by a scoring
   router

Many cores tile into a 2D mesh. Each core implements the same logic independently.

---

## 2. Parameters and Constants

| Name | Value | Description |
|---|---|---|
| `PACKET_SIZE` | 30 | Total bits per packet |
| `CORE_ADDR_BITS` | 6 | Bits for the core row address |
| `REGION_ADDR_BITS` | 4 | Bits to address one of 16 pixel regions within a core |
| `TRIG_ID_BITS` | 8 | Bits for the trigger identifier |
| `REGION_DATA_BITS` | 16 | Pixel ToT data: 4 pixels × 4-bit ToT |
| `NETWORK_MEM_DEPTH` | 5 | Number of packet slots in the local network buffer |
| `LATENCY_MEM_DEPTH` | 8 | Timestamp/ToT slots per pixel region |
| `LATENCY_COUNTER_BITS` | 9 | Width of the latency counter / BX timestamp |
| `SCORE_WIDTH` | 4 | Bits per distance score in the router |

---

## 3. Packet Format

Every packet is exactly 30 bits, packed from MSB to LSB as follows:

```
[29:24]  CoreRowAddr  — 6-bit static row address of the core that generated this hit
[23:20]  RegionAddr   — 4-bit index (0–15) of the pixel region within that core
[19:12]  TrigId       — 8-bit trigger identifier associated with this hit
[11:0]   RegionData   — 16-bit pixel data: {ToT[3][3:0], ToT[2][3:0], ToT[1][3:0], ToT[0][3:0]}
```

`RegionData` packs four 4-bit ToT values with pixel index 0 in the least-significant nibble
(`[3:0]`) and pixel index 3 in the most-significant nibble (`[15:12]`). ToT value 15 is
reserved to mean "this pixel did not fire in this region slot."

---

## 4. Interface

### 4.1 Inputs

| Signal | Width | Description |
|---|---|---|
| `ClkIn` | 1 | 40 MHz system clock. All state updates occur on rising edges. |
| `ResetIn_b` | 1 | Synchronous active-low reset. |
| `AnaHit` | 64 | Raw hit bitmap for the 8×8 pixel array. Bit `r*8 + c` corresponds to pixel at row `r`, column `c`. Treated as a level signal: high while the discriminator fires. |
| `CoreRowAddrIn` | 6 | Static row address of this core. Placed in every locally-generated packet. |
| `LatCntIn` | 9 | Current BX timestamp (binary, increments each clock cycle). |
| `LatCntReqIn` | 9 | BX timestamp shifted backward by the trigger latency. When `LatCntReqIn == stored_LatCnt` for a buffered hit, the trigger window for that hit has closed. |
| `TrigIn` | 1 | L1A trigger pulse. High for one clock cycle. |
| `TrigClearIn` | 1 | Double-trigger cancellation. High for one clock cycle; cancels a triggered-but-not-yet-read slot (ATLAS double-trigger support). |
| `TrigIdIn` | 8 | Trigger identifier to embed in packets for the current `TrigIn` pulse. |
| `buf_status_up/dn/l/r` | 3 each | Neighbor buffer occupancy counts (0–5 occupied slots). |
| `valid_up/dn/l/r_in` | 1 each | Neighbor valid signals: neighbor asserts when it wants to send toward this core. |

### 4.2 Outputs

| Signal | Width | Description |
|---|---|---|
| `buf_status_self` | 3 | This core's buffer occupancy (number of occupied slots, 0–5). |
| `valid_up/dn/l/r_o` | 1 each | This core's valid signals: asserted combinationally when this core has data and the router selects that direction. |

### 4.3 Bidirectional (Tristate)

| Signal | Width | Description |
|---|---|---|
| `data_bus_up/dn/l/r` | 30 each | Per-direction shared packet buses. Only the asserting side drives the bus; the non-asserting side is high-Z. |

---

## 5. Pixel Hit Processing

### 5.1 Pixel-to-Region Mapping

The 64 pixels are grouped into 16 one-dimensional four-pixel regions (1 row × 4 columns).
Region index `r` (0–15) maps to:

- Row: `r / 2`
- Columns 0–3 if `r % 2 == 0`; columns 4–7 if `r % 2 == 1`

Within a region, pixel index `p` (0–3) is the `p`-th column of the four-column slice.

| Region | Row | Columns | AnaHit bits |
|---|---|---|---|
| 0 | 0 | 0–3 | [3:0] |
| 1 | 0 | 4–7 | [7:4] |
| 2 | 1 | 0–3 | [11:8] |
| 3 | 1 | 4–7 | [15:12] |
| … | … | … | … |
| 14 | 7 | 0–3 | [59:56] |
| 15 | 7 | 4–7 | [63:60] |

### 5.2 Hit Leading Edge and Timestamp Storage

A hit leading edge (`HitLe`) is detected asynchronously when `AnaHit` for a pixel transitions
from 0 to 1. On the **first rising edge of `ClkIn`** after the leading edge, `HitLe` is
registered as a single-cycle pulse.

When any pixel in a region fires (`HitLeAny` = OR of all four pixels' `HitLe` flags):

- The current value of `LatCntIn` is stored into the **first free slot** (lowest free index) of
  that region's 8-slot timestamp memory. This value becomes the "hit timestamp" for this slot.
- The **same slot index** is recorded as the write address for each pixel's ToT memory.
- All four pixels' ToT memory slots are initialized: pixels that did not fire receive ToT = 15
  immediately; pixels that fired record their actual ToT at the hit trailing edge (see §5.3).

If no free timestamp slot exists when a hit arrives, the hit is silently discarded.

### 5.3 ToT Measurement

ToT is a 4-bit unsigned integer in the range 0–14. Value 15 is reserved for "no hit."

**Counting rule:**

ToT is measured in units of half clock periods (80 MHz effective resolution):

- Bits `[3:1]` count the number of **rising edges of `ClkIn`** that occurred while the pixel's
  discriminator (`AnaHit` bit) was asserted high. This approximates the number of complete 40 MHz
  clock cycles the hit lasted. 

CHECK THIS LOOKS FLIPPED BELOW
- Bit `[0]` (the phase bit) captures the state of `ClkIn` at the **hit trailing edge**:
  - `1` if `ClkIn` was in its high half (ClkIn = 1) when the hit fell low
  - `0` if `ClkIn` was in its low half (ClkIn = 0) when the hit fell low

**Saturation:**

When bits `[3:1]` reach `3'b111` (i.e., 7 or more rising edges counted), the counter saturates
and stops. The phase bit `[0]` is forced to 0, making ToT = 14. This preserves the invariant
that ToT = 15 is never produced by a real hit.

**ToT write timing:**

The ToT value is written into the ToT memory at the hit trailing edge (`HitTe`), which is a
single-cycle pulse on the first rising edge of `ClkIn` after the discriminator goes low. The
memory slot written is the one recorded at the leading edge.

**Non-firing pixels in an active region:**

When `HitLeAny` fires for a region, any pixel within that region that did not fire (`HitLe = 0`)
has ToT = 15 written into the current slot immediately (at the same cycle as the leading edge).

### 5.4 Timestamp Slot FSM

Each of the 8 timestamp slots per region runs an independent 2-bit FSM. Let `start` and `trig`
denote the two state bits:

| State | start | trig | Meaning |
|---|---|---|---|
| IDLE | 0 | 0 | Slot is free |
| COUNTING | 1 | 0 | Timestamp stored; watching for trigger latency expiry |
| TRIGGERED | 1 | 1 | Trigger matched; waiting one cycle for TOREAD |
| TOREAD | 0 | 1 | Ready for readout; asserts ReadyToRead |

**IDLE → COUNTING:** Triggered when this slot is selected as the write target (`WriteLatMem`
high) on a `HitLeAny` event. On the rising edge: `start ← 1`, `counter ← LatCntIn`.

**COUNTING: trigger window check (each rising clock edge):**

Evaluate `counter_last = (counter == LatCntReqIn) AND start AND NOT trig`.

- If `counter_last AND TrigIn AND NOT TrigClearIn`: transition to TRIGGERED.
  On this edge: `start ← 1`, `trig ← 1`, `counter ← {LatCntIn[8:8], TrigIdIn[7:0]}` (TrigId
  stored in the lower 8 bits of the counter register).
- If `counter_last AND (NOT TrigIn OR TrigClearIn)`: hit is discarded. Transition to IDLE.
  On this edge: `start ← 0`, `trig ← 0`.
- Otherwise (counter_last = 0): remain in COUNTING.

**COUNTING → TRIGGERED:** Takes one clock cycle (edge where `triggered` fires).

**TRIGGERED → TOREAD:** One clock cycle later (the following rising edge):

In TRIGGERED state: `req_to_read = start AND trig AND NOT TrigClearIn`. This is high when
TrigClearIn is not asserted. On the next rising edge: `start ← 0`, `trig ← 1` → TOREAD.

If `TrigClearIn` is asserted while in TRIGGERED state: `cleared` fires. On the next edge:
`start ← 0`, `trig ← 0` → IDLE (the trigger is cancelled).

**TOREAD:** `ReadyToRead` is asserted (combinationally: `trig AND NOT start`). The slot remains
in TOREAD until the core reads it out.

**TOREAD → IDLE:** When the core issues a read request to this slot (`ReadLatMem` pulse), on the
next rising edge: `trig ← 0` → IDLE.

**TrigIdOut:** Exposed from the lower 8 bits of the counter register at all times. Valid (equals
the matched TrigId) from the cycle after TRIGGERED through TOREAD.

### 5.5 Region Readout Token Chain

The 16 regions are connected in a priority chain (region 0 is highest priority). Each region
generates `TokOut`:

`TokOut[r] = TokIn[r] OR (any slot in region r is in TOREAD state AND NOT PwrDwnAll)`

The chain is initialized: `TokIn[0] = 0` (pinned low always).

`TokIn[r] = TokOut[r-1]` for r = 1..15.

**DataToCoreEn:** A region drives data to the core only when:
`DataToCoreEn[r] = (TokIn[r] == 0) AND (region r has at least one TOREAD slot) AND NOT PwrDwnAll`

This ensures the lowest-index triggered region (the first one whose `TokIn` is still 0) wins
each cycle. At most one region produces output per clock cycle.

**Outputs when DataToCoreEn[r] is high:**

- `RegionAddr`: the 4-bit region index `r`
- `RegionData`: `{ToT[3][3:0], ToT[2][3:0], ToT[1][3:0], ToT[0][3:0]}` from the TOREAD slot of
  region `r`. ToT = 15 for pixels that did not fire in that slot.
- `RegionTrig`: the `TrigIdOut` from the selected TOREAD slot of region `r`

After the core reads a region (ReadLatMem pulse to that slot), the slot transitions TOREAD → IDLE
on the next rising edge. If region `r` has additional TOREAD slots, it re-asserts TokOut in the
following cycle.

**core_mem_valid:** The OR of all 16 `TokOut` values (equivalently, the final `TokOut[15]`).
High when any region in the core has at least one TOREAD slot. This is the signal that tells the
network layer the core has data to forward.

`PwrDwnAll` is always 0 (all pixels active — pixel masking is not modeled in this spec).

### 5.6 Local Data Packet Assembly

When `core_mem_valid` is high, the core continuously presents a valid packet:

```
local_data_packet = {CoreRowAddrIn[5:0], RegionAddr[3:0], RegionTrig[7:0], RegionData[15:0]}
```

This packet reflects the currently-selected region (lowest-index TOREAD region). It changes
every cycle as regions drain. `ReadIn` is pinned high, so the core reads one region per cycle
whenever a region is ready.

---

## 6. Local Network Buffer

### 6.1 Structure

The buffer is a flat array of `NETWORK_MEM_DEPTH` (5) slots. Each slot is either **free** or
**occupied** (holds a 30-bit packet). There is no ordering; the read-pointer rule (§6.3)
determines which slot is output each cycle. Occupancy is counted combinationally and exposed as
`buf_status_self`.

### 6.2 Passthrough Mode

**When ALL 5 slots are free** (buffer completely empty) and `core_mem_valid` is high:

The local core's packet bypasses the buffer entirely. `local_data_packet` is selected directly as
the output packet without occupying any slot. No slot transitions occur for this core data.

**When any slot is occupied:** Passthrough is disabled. Local core data must be written into a
free slot before being routed.

### 6.3 Read Pointer (Output Selection)

Each cycle, the output packet is selected from the **lowest-index occupied slot**. Its index is
the read pointer (`network_rptr`). The selected packet is presented as `selected_data_packet`.

In passthrough mode (buffer empty, core has data): `selected_data_packet = local_data_packet`.

### 6.4 Local Data Valid

`local_data_valid = (network_m_cnt > 0) OR (passthrough_active AND core_mem_valid)`

where `passthrough_active = (network_m_cnt == 0)`.

This is the gate for all valid output signals. If `local_data_valid` is false, the core does not
assert valid in any direction.

### 6.5 Write Logic and Source Priority

Each cycle, up to five sources may write packets into the buffer:

| Priority | Source | Active condition |
|---|---|---|
| 1 (highest) | Local core | `core_mem_valid AND NOT passthrough_active` |
| 2 | UP neighbor | `read_n[UP]` asserted (this cycle, this core is the receiver on UP) |
| 3 | DOWN neighbor | `read_n[DN]` asserted |
| 4 | LEFT neighbor | `read_n[L]` asserted |
| 5 (lowest) | RIGHT neighbor | `read_n[R]` asserted |

Sources are assigned to slots using a **priority chain**: for each slot in order (0, 1, 2, …),
the highest-priority remaining active source is assigned. Once a source is assigned to a slot, it
is removed from the pool for subsequent slots.

A slot is eligible to receive a write if:

`(slot is free) OR (slot is the read pointer AND this core is asserting outward this cycle)`

The second condition allows the read-pointer slot to be simultaneously freed and refilled in one
cycle.

### 6.6 Overflow

When the buffer is full (all 5 slots occupied) and no slot is being freed this cycle, no writes
occur. All incoming packets from all directions are silently dropped.

When fewer free/freeable slots than active sources: lower-priority sources are dropped (RIGHT
first, then LEFT, DOWN, UP, local core last).

---

## 7. Router

### 7.1 Scoring

The router scores five ports. Port indices and their fixed distance penalties:

| Index | Direction | Distance Score |
|---|---|---|
| 0 | UP | 2 |
| 1 | DOWN | 0 |
| 2 | LEFT | 1 |
| 3 | RIGHT | 1 |
| 4 | SELF | 1 |

Score for each port:

`score[i] = buf_status[i] + distScore[i]`

where `buf_status[4] = network_m_cnt` (this core's own buffer count) and `buf_status[0..3]` are
the four neighbor buffer counts from the input signals.

### 7.2 Winner Selection — Argmin with All-Pairs Comparison

Evaluate all unique pairs `(i, j)` with `i < j`, in that order:

```
comp = (score[i] < score[j])   // strict less-than
```

For each port, maintain a boolean `lowest[port]` initialized to `true`.

For each pair:
```
lowest[i] = lowest[i] AND comp
lowest[j] = lowest[j] AND NOT comp
```

After all pairs are evaluated, exactly one port has `lowest = true`. That port is the winner.

**Tie-breaking:** Strict less-than means equal scores result in `comp = false`, so the
higher-indexed port wins. Tie-break order (highest index wins): SELF (4) > RIGHT (3) > LEFT (2)
> DOWN (1) > UP (0).

### 7.3 Routing Decision Output

`routing_decision` is a 4-bit one-hot covering only the four cardinal directions:

| Bit | Direction |
|---|---|
| 0 | UP |
| 1 | DOWN |
| 2 | LEFT |
| 3 | RIGHT |

If the SELF port wins: `routing_decision = 4'b0000` (hold; do not route this cycle).

`routing_decision` is evaluated combinationally each cycle from the current buffer status values.

---

## 8. Handshake Protocol

### 8.1 Valid Output Signals (Combinational)

Per direction `d` (UP, DN, L, R):

`valid_[d]_o = routing_decision[d] AND local_data_valid`

These are fully combinational. They change whenever `routing_decision` or `local_data_valid`
changes.

### 8.2 Transaction Arbitration — Negedge-Registered

On the **falling edge of `ClkIn`**, per-direction transaction flags are registered:

`transact_n[d] <= valid_[d]_o XOR valid_[d]_in`

- **XOR = 1** (exactly one side asserts valid): a transaction proceeds.
- **XOR = 0** (both sides assert, or neither asserts): transaction cancelled; no bus activity.

From `transact_n` (combinational):

```
assert_n[d] = transact_n[d] AND valid_[d]_o    // this core is the SENDER
read_n[d]   = transact_n[d] AND valid_[d]_in   // this core is the RECEIVER
```

Only one of `assert_n[d]` or `read_n[d]` can be true for any direction in a given cycle.

### 8.3 Bus Driving (Combinational from assert_n)

When `assert_n[d]` is asserted:
- This core drives `data_bus_[d]` with `selected_data_packet`
- The bus carries the packet from negedge through the following posedge

When `assert_n[d]` is not asserted:
- This core puts `data_bus_[d]` in high-Z
- If `read_n[d]` is asserted, this core reads `data_bus_[d]` as the incoming packet for that
  direction

### 8.4 Collision and Retry

When both this core and the neighbor simultaneously assert valid in the same direction (`XOR = 0`):
- `transact_n[d] = 0`, so `assert_n[d] = 0` and `read_n[d] = 0`
- Neither side drives the bus; neither side writes to its buffer
- Both cores hold their current selected packets
- On the next cycle, each core's router recomputes `routing_decision` independently based on
  current buffer states. The router may select the same or a different direction. There is no
  backoff counter or timeout.

---

## 9. Cycle-by-Cycle Timing

### 9.1 Nominal Transaction Sequence

```
Posedge N:      Buffer state, FSM updates, and register writes settle (NBA phase).
                selected_data_packet, local_data_valid, routing_decision settle combinationally.

Negedge N:      transact_n[d] registers sample (valid_[d]_o XOR valid_[d]_in).

After negedge N: assert_n and read_n settle combinationally.
                 Asserting core drives data_bus_[d] with selected_data_packet.

Posedge N+1:    Network buffer writes commit:
                  - For each direction d where read_n[d] was asserted: data_bus_[d]_in is the
                    incoming packet, written to a free slot per priority rules.
                  - Read-pointer slot is freed if assert_n was set and no write targets it.
                  - Local core data is written to a free slot if active sources exist.
```

### 9.2 Local Hit to Network Availability — 2-Cycle Delay

Define Cycle N as the rising edge where **both** of the following are simultaneously true:

- The stored `LatCnt` for a hit slot equals `LatCntReqIn` (`counter_last` fires)
- `TrigIn == 1`

Timeline:

| Cycle | Event |
|---|---|
| N (posedge) | Trigger matched. FSM transitions COUNTING → TRIGGERED. TrigId stored. ReadyToRead remains 0. |
| N+1 (posedge) | FSM transitions TRIGGERED → TOREAD. ReadyToRead asserts. Token chain propagates. Core presents `local_data_packet`. `core_mem_valid` goes high. |
| N+1 (negedge) | If in passthrough mode: `transact_n` captures valid. Bus driven with the packet. |
| N+2 (posedge) | **Packet enters network layer.** If buffered mode: packet written to network buffer slot. If passthrough: neighbor captures packet from bus. Either way, the packet is available for downstream routing as of this edge. |

The 2-cycle delay (posedge N to posedge N+2) is fixed for buffered mode. For passthrough mode,
the packet is on the bus from negedge N+1 and captured by the neighbor at posedge N+2 — same
endpoint.

### 9.3 Network Forwarding Latency

A packet written to the network buffer at posedge N can be selected as the read pointer and
driven on the output bus at negedge N. The neighbor captures it at posedge N+1. Minimum
forwarding latency from buffer write to neighbor capture: **1 clock cycle**.

---

## 10. Reset Behavior

On any rising edge of `ClkIn` while `ResetIn_b == 0` (synchronous reset):

- All 5 network buffer slots: marked free, contents zeroed
- `transact_n` registers: cleared to 0
- All LatencyMemCell FSMs: all slots → IDLE (`start = 0`, `trig = 0`)
- All pixel ToT state machines: reset (counters cleared, state bits zeroed)

Normal operation begins on the first rising edge after `ResetIn_b` goes high.

---

## 11. Corner Cases and Boundary Conditions

**Simultaneous multi-pixel hits in one region:** Multiple pixels in the same region firing in
the same cycle share a single timestamp slot. Each pixel's ToT is measured independently. The
shared slot records one timestamp; the packet carries all four ToTs.

**Multiple distinct hits in one region (different cycles):** Each leading-edge event claims the
next free slot (up to 8 slots). Each slot runs its FSM independently. Multiple slots from the
same region may be in TOREAD simultaneously; the token chain drains them one per cycle in slot
order (lowest triggered index first due to the priority chain in LatencyMem).

**Timestamp slot exhaustion (>8 hits before readout):** If all 8 slots are occupied when a new
`HitLeAny` fires, the hit is silently discarded. No error flag is generated.

**Multiple regions triggered simultaneously:** The token chain enforces one region per cycle.
If regions 3, 7, and 11 all have TOREAD slots on the same cycle, region 3 drives data that
cycle. Region 7 drives the next cycle (after region 3's slot is cleared and token propagates).
Region 11 drives the following cycle.

**Network buffer full, local hit triggered:** If the network buffer has all 5 slots occupied
when `core_mem_valid` rises, the local core packet competes for any slot being freed that
cycle. If no slot is freed, the local core packet is not written and is retried on the next
cycle (it remains valid until the token-chain slot is read and cleared).

**Router selects SELF:** `routing_decision = 4'b0000`. `local_data_valid` may be true but no
valid_o is asserted in any direction. The selected packet stays in the buffer. Next cycle the
router recomputes with updated neighbor buffer states.

**Collision on a direction where this core is not sending:** If this core has `valid_[d]_o = 0`
and `valid_[d]_in = 1` from the neighbor, then XOR = 1 → `read_n[d] = 1` → normal receive.
No collision. Collision only occurs when BOTH sides assert valid in the same direction.

**TrigClear while TRIGGERED:** If TrigClearIn is asserted on the cycle when `req_to_read` would
fire (start=1, trig=1), the slot returns to IDLE on the next edge instead of advancing to
TOREAD. The packet is cancelled.

**TrigIn high but LatCntReq does not match any stored value:** No action. `TrigIn` only affects
slots in COUNTING state where `counter_last` is simultaneously true.

**LatCntReqIn matches stored value but TrigIn = 0:** The hit is discarded. The slot returns to
IDLE on the next rising edge.

---

## 12. Signals Not Modeled

The following hardware signals are present in the RTL but excluded from the golden model:

- Pixel masking and power-down (`PwrDwn`, `conf_hit_en`) — all pixels assumed always active
- Analog front-end bias rails (`VBP_PREAMP_A`, etc.) — not used in digital simulation
- Calibration injection signals (`CalEdgeIn`, `CalAuxIn`, `S0`, `S1`) — FeControl is a passthrough
- Pixel configuration registers (`PixelConfDefault`, `PixelConfDataWr`, etc.) — not wired in sim
- Clock skew compensation delay lines (`ProgrammableDelay`) — all clocks treated as ClkIn
- Gray encoding of `LatCnt`/`LatCntReq` — treated as plain binary integers
- `CoreRowAddrOut`, `ClkOut`, `ResetOut_b`, `CalEdgeOut`, `CalAuxOut`, `LatCntOut`,
  `LatCntReqOut`, `TrigOut`, `TrigClearOut`, `TrigIdOut` — passthrough wires; no modeling needed
