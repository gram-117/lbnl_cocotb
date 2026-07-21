`default_nettype none
// NetworkedCore.sv [RTL]

module NetworkedCore (
   input  wire [63:0] AnaHit,

   inout  wire [7:0] VBP_PREAMP_A,
   inout  wire [7:0] VBP_PREAMP_B,
   inout  wire [7:0] VBN_COMP,
   inout  wire [7:0] VBN_PRECOMP,
   inout  wire [7:0] VTH1,
   inout  wire [7:0] VTH2,
   inout  wire [7:0] VBN_LCC,
   inout  wire [7:0] VBP_VFF,
   inout  wire [7:0] VCTRL_CF0,
   inout  wire [7:0] VCTRL_LCC,
   inout  wire [7:0] VCAL_HI,
   inout  wire [7:0] VCAL_MI,

   input  wire ClkIn,
   output wire ClkOut,
   input  wire ResetIn_b,
   output wire ResetOut_b,

   input  wire [5:0] CoreRowAddrIn,
   output wire [5:0] CoreRowAddrOut,

   input  wire CalEdgeIn,
   output wire CalEdgeOut,
   input  wire CalAuxIn,
   output wire CalAuxOut,

   input  wire [`LATENCY_COUNTER_BITS-1:0] LatCntIn,
   output wire [`LATENCY_COUNTER_BITS-1:0] LatCntOut,
   input  wire [`LATENCY_COUNTER_BITS-1:0] LatCntReqIn,
   output wire [`LATENCY_COUNTER_BITS-1:0] LatCntReqOut,

   input  wire TrigIn,
   output wire TrigOut,
   input  wire TrigClearIn,
   output wire TrigClearOut,

   // NETWORKING LAYER
   inout  wire  [`PACKET_SIZE-1:0] data_bus_up,
   inout  wire  [`PACKET_SIZE-1:0] data_bus_dn,
   inout  wire  [`PACKET_SIZE-1:0] data_bus_l,
   inout  wire  [`PACKET_SIZE-1:0] data_bus_r,

   input  wire  [$clog2(`NETWORK_MEM_DEPTH+1)-1:0] buf_status_up,
   input  wire  [$clog2(`NETWORK_MEM_DEPTH+1)-1:0] buf_status_dn,
   input  wire  [$clog2(`NETWORK_MEM_DEPTH+1)-1:0] buf_status_l,
   input  wire  [$clog2(`NETWORK_MEM_DEPTH+1)-1:0] buf_status_r,
   output logic [$clog2(`NETWORK_MEM_DEPTH+1)-1:0] buf_status_self,

   output logic valid_up_o,
   output logic valid_dn_o,
   output logic valid_l_o,
   output logic valid_r_o,

   input  logic valid_up_in,
   input  logic valid_dn_in,
   input  logic valid_l_in,
   input  logic valid_r_in    // FIX: removed trailing comma
);                            // FIX: added missing semicolon

   // ---------------------------------------------------------------
   // Local params / declarations
   // ---------------------------------------------------------------
   localparam int PKT   = `PACKET_SIZE;
   localparam int DEPTH = `NETWORK_MEM_DEPTH;
   localparam int CNTW  = $clog2(DEPTH+1);       // FIX: count width
   localparam int PTRW  = (DEPTH > 1) ? $clog2(DEPTH) : 1; // FIX: rptr width = mem index

   // bus in/out
   logic [PKT-1:0] data_bus_up_in, data_bus_dn_in, data_bus_l_in, data_bus_r_in;
   logic [PKT-1:0] data_bus_up_o,  data_bus_dn_o,  data_bus_l_o,  data_bus_r_o;

   // packet gen / selection
   logic [PKT-1:0] local_data_packet;
   logic [PKT-1:0] selected_data_packet;
   logic [PTRW-1:0] network_rptr;

   // memory + occupancy
   logic [PKT-1:0]     network_mem   [DEPTH-1:0];
   logic [DEPTH-1:0]   network_m_Wen;
   logic [DEPTH-1:0]   network_m_free;   // FIX: ':' not ';'
   logic [CNTW-1:0]    network_m_cnt;    // FIX: ':' not ';', proper width

   // valid / arbitration
   logic [4:0] valid_arr_chain [DEPTH-1:0];
   logic [4:0] valid_arr;                 // {R,L,DN,UP,CORE}, core = LSB = highest prio
   logic       core_mem_valid;
   logic       core_mem_passthrough;
   logic       local_data_valid;

   logic [3:0] routing_decision;          // FIX: declared
   logic [3:0] read_n;                    // FIX: declared
   logic [3:0] assert_n;
   logic [3:0] transact_n;

   // digital core
   logic [4*`TRIG_ID_BITS-1:0] RegionDataOut;
   logic [`TRIG_ID_BITS-1:0]   RegionTrigId;

   // per-slot chosen source (priority-encoded), shared by comb + seq  // FIX
   logic [4:0] slot_src [DEPTH-1:0];

   // ---------------------------------------------------------------
   // Digital core
   // ---------------------------------------------------------------
   DigitalCoreStub DigitalCore (
      .AnaHit(AnaHit),
      .VBP_PREAMP_A(VBP_PREAMP_A), .VBP_PREAMP_B(VBP_PREAMP_B),
      .VBN_COMP(VBN_COMP), .VBN_PRECOMP(VBN_PRECOMP),
      .VTH1(VTH1), .VTH2(VTH2), .VBN_LCC(VBN_LCC), .VBP_VFF(VBP_VFF),
      .VCTRL_CF0(VCTRL_CF0), .VCTRL_LCC(VCTRL_LCC),
      .VCAL_HI(VCAL_HI), .VCAL_MI(VCAL_MI),
      .ClkIn(ClkIn), .ClkOut(ClkOut),
      .ResetIn_b(ResetIn_b), .ResetOut_b(ResetOut_b),
      .CoreRowAddrIn(CoreRowAddrIn), .CoreRowAddrOut(CoreRowAddrOut),
      .CalEdgeIn(CalEdgeIn), .CalEdgeOut(CalEdgeOut),
      .CalAuxIn(CalAuxIn), .CalAuxOut(CalAuxOut),
      .LatCntIn(LatCntIn), .LatCntOut(LatCntOut),
      .LatCntReqIn(LatCntReqIn), .LatCntReqOut(LatCntReqOut),
      .TrigIn(TrigIn), .TrigOut(TrigOut),
      .TrigClearIn(TrigClearIn), .TrigClearOut(TrigClearOut),
      .RegionDataOut(RegionDataOut), .RegionTrigId(RegionTrigId)
   );

   // ---------------------------------------------------------------
   // Router
   // ---------------------------------------------------------------
   RouterStub router_stub_inst (
      .buf_status_up(buf_status_up),
      .buf_status_dn(buf_status_dn),
      .buf_status_l (buf_status_l),
      .buf_status_r (buf_status_r),
      .buf_status_self(network_m_cnt),
      .routing_decision(routing_decision)   // one-hot or all-zero
   );

   assign local_data_valid = (network_m_cnt != '0) ||
                             (core_mem_passthrough && core_mem_valid); // FIX: != 0

   assign valid_up_o = routing_decision[0] && local_data_valid;
   assign valid_dn_o = routing_decision[1] && local_data_valid;
   assign valid_l_o  = routing_decision[2] && local_data_valid;
   assign valid_r_o  = routing_decision[3] && local_data_valid;

   // ---------------------------------------------------------------
   // Handshake (negedge intentional)
   // ---------------------------------------------------------------
   always_ff @(negedge ClkIn) begin
      transact_n[0] <= valid_up_o ^ valid_up_in;
      transact_n[1] <= valid_dn_o ^ valid_dn_in;
      transact_n[2] <= valid_l_o  ^ valid_l_in;
      transact_n[3] <= valid_r_o  ^ valid_r_in;
   end

   assign assert_n[0] = transact_n[0] && valid_up_o;
   assign assert_n[1] = transact_n[1] && valid_dn_o;
   assign assert_n[2] = transact_n[2] && valid_l_o;
   assign assert_n[3] = transact_n[3] && valid_r_o;

   assign read_n[0] = transact_n[0] && valid_up_in;
   assign read_n[1] = transact_n[1] && valid_dn_in;
   assign read_n[2] = transact_n[2] && valid_l_in;
   assign read_n[3] = transact_n[3] && valid_r_in;

   // FIX: per-direction assert index; drive net only when asserting, else 'z
   assign data_bus_up = assert_n[0] ? data_bus_up_o : 'z;
   assign data_bus_dn = assert_n[1] ? data_bus_dn_o : 'z;
   assign data_bus_l  = assert_n[2] ? data_bus_l_o  : 'z;
   assign data_bus_r  = assert_n[3] ? data_bus_r_o  : 'z;

   // FIX: read the shared net back in
   assign data_bus_up_in = data_bus_up;
   assign data_bus_dn_in = data_bus_dn;
   assign data_bus_l_in  = data_bus_l;
   assign data_bus_r_in  = data_bus_r;

   assign buf_status_self = network_m_cnt;

   // ---------------------------------------------------------------
   // Combinational: occupancy, rptr (LIFO), arbitration chain, driver
   // ---------------------------------------------------------------
   integer mem_idx;
   always_comb begin
      // occupancy count
      network_m_cnt = '0;
      for (mem_idx = 0; mem_idx < DEPTH; mem_idx++)
         network_m_cnt = network_m_cnt + (network_m_free[mem_idx] ? '0 : 1'b1);

      // network empty => passthrough local straight to bus
      core_mem_passthrough = &network_m_free;

      // source valids: {R,L,DN,UP,CORE}
      valid_arr = {read_n, (core_mem_valid && !core_mem_passthrough)};

      // priority chain: slot 0 gets raw mask, grabs highest-prio bit,
      // passes reduced mask down. slot_src[i] = one-hot source for slot i.
      for (mem_idx = 0; mem_idx < DEPTH; mem_idx++) begin
         automatic logic [4:0] mask;
         mask = (mem_idx == 0) ? valid_arr : valid_arr_chain[mem_idx-1];

         // can this slot accept a write? (free, or being read-and-replaced)
         network_m_Wen[mem_idx] =
            (network_m_free[mem_idx] ||
             (network_rptr == mem_idx[PTRW-1:0] && |assert_n)) && (|mask);

         // priority-encode the winning source (LSB = core = highest prio) // FIX
         slot_src[mem_idx] = '0;
         if (network_m_Wen[mem_idx]) begin
            if      (mask[0]) slot_src[mem_idx] = 5'b00001;
            else if (mask[1]) slot_src[mem_idx] = 5'b00010;
            else if (mask[2]) slot_src[mem_idx] = 5'b00100;
            else if (mask[3]) slot_src[mem_idx] = 5'b01000;
            else if (mask[4]) slot_src[mem_idx] = 5'b10000;
         end

         // reduced mask for next slot: clear the WON source bit  // FIX (was [i][i])
         valid_arr_chain[mem_idx] = mask & ~slot_src[mem_idx];
      end

      // LIFO read pointer + selected packet: highest occupied index
      selected_data_packet = local_data_packet; // default: network empty -> local
      network_rptr = '0;
      for (mem_idx = DEPTH-1; mem_idx >= 0; mem_idx--) begin
         if (!network_m_free[mem_idx]) begin
            selected_data_packet = network_mem[mem_idx];
            network_rptr = mem_idx[PTRW-1:0];
            break;
         end
      end

      // drive the owned bus (one-hot assert_n)
      data_bus_up_o = '0; data_bus_dn_o = '0;
      data_bus_l_o  = '0; data_bus_r_o  = '0;
      unique case (assert_n)
         4'b0001: data_bus_up_o = selected_data_packet;
         4'b0010: data_bus_dn_o = selected_data_packet;
         4'b0100: data_bus_l_o  = selected_data_packet;
         4'b1000: data_bus_r_o  = selected_data_packet;
         default: ; // idle / no assertion (all-zero); buses stay 0 (driven 'z anyway)
      endcase
   end

   // ---------------------------------------------------------------
   // Sequential: memory + occupancy update
   // ---------------------------------------------------------------
   always_ff @(posedge ClkIn) begin
      if (!ResetIn_b) begin
         network_m_free <= '1;               // all free
         for (mem_idx = 0; mem_idx < DEPTH; mem_idx++)
            network_mem[mem_idx] <= '0;
      end
      else begin
         for (mem_idx = 0; mem_idx < DEPTH; mem_idx++) begin
            if (network_m_Wen[mem_idx]) begin
               network_m_free[mem_idx] <= 1'b0;  // FIX: nonblocking throughout
               // FIX: select data via the SAME slot_src the comb logic cleared
               unique case (slot_src[mem_idx])
                  5'b00001: network_mem[mem_idx] <= local_data_packet;
                  5'b00010: network_mem[mem_idx] <= data_bus_up_in;
                  5'b00100: network_mem[mem_idx] <= data_bus_dn_in;
                  5'b01000: network_mem[mem_idx] <= data_bus_l_in;
                  5'b10000: network_mem[mem_idx] <= data_bus_r_in;
                  default:  $error("NetworkedCore: Wen high but slot_src empty (slot %0d)", mem_idx);
               endcase
            end
            // read-and-consume: rptr slot read out this cycle and NOT rewritten
            else if (network_rptr == mem_idx[PTRW-1:0] && |assert_n) begin
               network_m_free[mem_idx] <= 1'b1;
            end
         end
      end
   end

endmodule

`default_nettype wire