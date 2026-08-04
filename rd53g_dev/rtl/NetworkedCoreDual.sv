`default_nettype none
// NetworkedCore.sv [RTL]
// NaN
// [Language]       SystemVerilog 2012 [IEEE Std. 1800-2012]
// hierarchy -> sits above digitalcore.sv

// description: 
// Networked core wraps modified rd53c and is responsible for buffering/routing locally 
// generated ToT packets and acting as part of the transport mesh for all packets
// which get routed into it
// each core can take in data from each cardinal direction + data from its local memory each cycles

// but only outputs to one core per cycle

// IMPORTANT
// ---------------------this version----------------------------------------------------
// up/left on negedge, down/right on negedge "double data rate"
// -------------------------------------------------------------------------------------

// define packet to be: addr trig data **

// data from other cores takes >= 1 cycle to be clocked in and then out
// if network memory is empty, local core data is routed straight out

// two cores cannot route data to each other, this is unproductive and also 
// allows us to use one shared bus between all cores

// notes:
/*
TODO: 
BIG PICTURE SHID (not 100% grammy problem)
FIGURE OUT CONFIG STUFF
FIGURE OUT DISABLING/FAULT TOLERANCE

ADD STEADY STATE 1 CYCLE HIT (clock in and out of same register)
need to do this by chooiing to write into the slot that is selected by the rptr
^ done need to be verified!!!!! looks good 7/22/26 :) 

tested canceling as well basic verif

dual edge stuff!!!


*/

module NetworkedCoreDual (
   // TO DIGITAL CORE
   /////////////////////////////
   //   hits from testbench   //
   ///////////////////////////// 
   input wire [63:0] AnaHit,                // 8 x 8 pixels per-core

   // DIFF front-end
   inout wire [7:0] VBP_PREAMP_A,
   inout wire [7:0] VBP_PREAMP_B,
   inout wire [7:0] VBN_COMP,
   inout wire [7:0] VBN_PRECOMP,
   inout wire [7:0] VTH1,
   inout wire [7:0] VTH2,
   inout wire [7:0] VBN_LCC,
   inout wire [7:0] VBP_VFF,
   inout wire [7:0] VCTRL_CF0,
   inout wire [7:0] VCTRL_LCC,
   inout wire [7:0] VCAL_HI,
   inout wire [7:0] VCAL_MI,


   /////////////////////////
   //   clock and reset   //
   /////////////////////////
   input  wire ClkIn,
   output wire ClkOut,

   input  wire ResetIn_b,
   output wire ResetOut_b,


   ///////////////////////////////
   //   configuration section   //
   ///////////////////////////////

   // static core-row addressing
   input  wire [5:0] CoreRowAddrIn, // static core-row address for readout and clock-skew compensation i.e. 6-bit to address 48 (42) core-rows in ATLAS (CMS) chip
   output wire [5:0] CoreRowAddrOut,
   

   // go over global config bits with Timon.... TODO

   /////////////////////////////
   //   calibration section   //
   /////////////////////////////

   // calibration signals
   input  wire CalEdgeIn,      // global signal for first analog injection (needs fine timing), also used for "digital injection"
   output wire CalEdgeOut,

   input  wire CalAuxIn,       // global signal for (optional) second analog injection (not critical timing)
   output wire CalAuxOut,

   input  wire [`LATENCY_COUNTER_BITS-1:0] LatCntIn,      // BX timestamp
   output wire [`LATENCY_COUNTER_BITS-1:0] LatCntOut,
   input  wire [`LATENCY_COUNTER_BITS-1:0] LatCntReqIn,   // BX timestamp shifted back in time by trigger latency
   output wire [`LATENCY_COUNTER_BITS-1:0] LatCntReqOut,

  // this remains global?
   input  wire TrigIn,
   output wire TrigOut,

   // TODO look at trig clear usage
   input  wire TrigClearIn,
   output wire TrigClearOut,

   input wire [`TRIG_ID_BITS-1:0] TrigIdIn,
   input wire [`TRIG_ID_BITS-1:0] TrigIdOut,

    // NETWORKING LAYER SIGNALS
    inout wire [`PACKET_SIZE-1:0] data_bus_up,
    inout wire [`PACKET_SIZE-1:0] data_bus_dn,
    inout wire [`PACKET_SIZE-1:0] data_bus_l,
    inout wire [`PACKET_SIZE-1:0] data_bus_r,

    input wire [$clog2(`NETWORK_MEM_DEPTH+1)-1:0] buf_status_up,
    input wire [$clog2(`NETWORK_MEM_DEPTH+1)-1:0] buf_status_dn,
    input wire [$clog2(`NETWORK_MEM_DEPTH+1)-1:0] buf_status_l,
    input wire [$clog2(`NETWORK_MEM_DEPTH+1)-1:0] buf_status_r,
    output logic [$clog2(`NETWORK_MEM_DEPTH+1)-1:0] buf_status_self,

    output logic valid_up_o,
    output logic valid_dn_o,
    output logic valid_l_o,
    output logic valid_r_o,

    input logic valid_up_in,
    input logic valid_dn_in,
    input logic valid_l_in,
    input logic valid_r_in

// for simulation icarus doesnt like packed arr
  // output logic [`PACKET_SIZE-1:0] network_mem_tb_0,
  // output logic [`PACKET_SIZE-1:0] network_mem_tb_1,
  // output logic [`PACKET_SIZE-1:0] network_mem_tb_2,
  // output logic [`PACKET_SIZE-1:0] network_mem_tb_3,
  // output logic [`PACKET_SIZE-1:0] network_mem_tb_4,
  // output logic [`PACKET_SIZE-1:0] network_mem_tb_5,
  // output logic [`PACKET_SIZE-1:0] network_mem_tb_6,
  // output logic [`PACKET_SIZE-1:0] network_mem_tb_7
);

// assign network_mem_tb_0 = network_mem[0];
// assign network_mem_tb_1 = network_mem[1];
// assign network_mem_tb_2 = network_mem[2];
// assign network_mem_tb_3 = network_mem[3];
// assign network_mem_tb_4 = network_mem[4];
// assign network_mem_tb_5 = network_mem[5];
// assign network_mem_tb_6 = network_mem[6];
// assign network_mem_tb_7 = network_mem[7];


// gated clock for network memory, high on valids 
logic ClkNetGated; // TODO


// arbiting / busses
logic [`PACKET_SIZE-1:0] data_bus_up_in;
logic [`PACKET_SIZE-1:0] data_bus_dn_in;
logic [`PACKET_SIZE-1:0] data_bus_l_in;
logic [`PACKET_SIZE-1:0] data_bus_r_in;

logic [`PACKET_SIZE-1:0] data_bus_up_o;
logic [`PACKET_SIZE-1:0] data_bus_dn_o;
logic [`PACKET_SIZE-1:0] data_bus_l_o;
logic [`PACKET_SIZE-1:0] data_bus_r_o;


// core wrapper/ logic + packet gen stage
logic [`PACKET_SIZE-1:0] local_data_packet;
logic [`PACKET_SIZE-1:0] selected_data_packet;

logic TokIn, TokOut;
// if token is being occupied by core -> it has data to read


// network layer + logic
logic [`PACKET_SIZE-1:0] network_mem [`NETWORK_MEM_DEPTH-1:0];

logic [`NETWORK_MEM_DEPTH-1:0] network_m_Wen;
logic [`NETWORK_MEM_DEPTH-1:0] network_m_full;
logic [$clog2(`NETWORK_MEM_DEPTH+1)-1:0] network_m_cnt;
logic [$clog2(`NETWORK_MEM_DEPTH+1)-1:0] network_rptr;

// valid / arbitration
logic [4:0] valid_arr_chain [`NETWORK_MEM_DEPTH-1:0];
logic [4:0] valid_arr;                 // {R,L,DN,UP,CORE}, core = LSB = highest prio
logic       core_mem_valid;
logic       core_mem_passthrough;
logic       local_data_valid;

logic [3:0] routing_decision;          
logic [3:0] read_n;                    
logic [3:0] assert_n;

// negedge buffer
logic [`PACKET_SIZE-1:0] negedge_buffer_l;
logic [`PACKET_SIZE-1:0] negedge_buffer_dn;
logic valid_l_in_buf; 
logic valid_dn_in_buf;

// digital core interaction
logic [`REGION_DATA_BITS-1:0] RegionDataOut;
logic [`TRIG_ID_BITS-1:0] RegionTrigOut;
logic [3:0] RegionAddrOut;

logic [4:0] slot_src [`NETWORK_MEM_DEPTH-1:0];

// CORE WRAPPER
// digital core instantiation, best guess for signals needs some modifications...

DigitalCore DigitalCore (
    .AnaHit(AnaHit),

    .VBP_PREAMP_A(VBP_PREAMP_A),
    .VBP_PREAMP_B(VBP_PREAMP_B),
    .VBN_COMP(VBN_COMP),
    .VBN_PRECOMP(VBN_PRECOMP),
    .VTH1(VTH1),
    .VTH2(VTH2),
    .VBN_LCC(VBN_LCC),
    .VBP_VFF(VBP_VFF),
    .VCTRL_CF0(VCTRL_CF0),
    .VCTRL_LCC(VCTRL_LCC),
    .VCAL_HI(VCAL_HI),
    .VCAL_MI(VCAL_MI),

    .ClkIn(ClkIn),
    .ClkOut(ClkOut),

    .ResetIn_b(ResetIn_b),
    .ResetOut_b(ResetOut_b),

    .CoreRowAddrIn(CoreRowAddrIn),
    .CoreRowAddrOut(CoreRowAddrOut),

    .CalEdgeIn(CalEdgeIn),
    .CalEdgeOut(CalEdgeOut),

    .CalAuxIn(CalAuxIn),
    .CalAuxOut(CalAuxOut),

    .LatCntIn(LatCntIn),
    .LatCntOut(LatCntOut),

    .LatCntReqIn(LatCntReqIn),
    .LatCntReqOut(LatCntReqOut),

    .TrigIn(TrigIn),
    .TrigOut(TrigOut),

    .TrigClearIn(TrigClearIn),
    .TrigClearOut(TrigClearOut),

    .TrigIdIn(TrigIdIn),      // unique ID number associated to L1 trigger
    .TrigIdOut(TrigIdOut),

    .TokIn(TokIn),
    .TokOut(TokOut),

    .RegionDataOut(RegionDataOut),
    .RegionAddrOut(RegionAddrOut),
    .RegionTrigOut(RegionTrigOut)
);

assign TokIn = 1'b0; // keep low, lets core readout whenever

// NETWORK LAYER

  assign core_mem_valid = TokOut; // 1 if core has data remaining;
  assign local_data_packet = {CoreRowAddrIn, RegionAddrOut, RegionTrigOut, RegionDataOut};


logic [$clog2(`NETWORK_MEM_DEPTH + 1)-1:0] buf_status_all [4:0]; // hard coded for now, 2d array of buffer status
assign buf_status_all[0] = buf_status_up;
assign buf_status_all[1] = buf_status_dn;
assign buf_status_all[2] = buf_status_l;
assign buf_status_all[3] = buf_status_r;
assign buf_status_all[4] = network_m_cnt;
// // ROUTER
// abstracty router will take in local buffer status, neighboring buffer status, 
// and output a routing decision (set one of the valid signals to high)
Router router_inst (
    // .buf_status_up (buf_status_up),
    // .buf_status_dn (buf_status_dn),
    // .buf_status_l  (buf_status_l),
    // .buf_status_r  (buf_status_r),
    // .buf_status_self (network_m_cnt),
    .buf_status(buf_status_all),
    .routing_decision (routing_decision) // if no 1s then don't route! only looks at nsew no self
);

// do we have valid data? (data in network or core bypass)
assign local_data_valid = network_m_cnt != 0 || (core_mem_passthrough && core_mem_valid);

  // [3] = right
  // [2] = left
  // [1] = down
  // [0] = up
assign valid_up_o = routing_decision[0] && local_data_valid && ClkIn; 
assign valid_dn_o = routing_decision[1] && local_data_valid && !ClkIn; 
assign valid_l_o = routing_decision[2] && local_data_valid && ClkIn; 
assign valid_r_o = routing_decision[3] && local_data_valid && !ClkIn; 


// need some decision making to choose which slots next for now just fixed priority
// Router2 Router2 (
// )
// prolly won't be a module though (wptr in memory)

// this core is asserting for a transaction
assign assert_n[0] = valid_up_o;
assign assert_n[1] = valid_dn_o;
assign assert_n[2] = valid_l_o;
assign assert_n[3] = valid_r_o;

// Neighbor owns the bus; this core reads from it.
assign read_n[0] = valid_up_in;
assign read_n[3] = valid_r_in;
// dn and l are negedge buffered
assign read_n[1] = valid_dn_in_buf;
assign read_n[2] = valid_l_in_buf;

// TODO CHECK THIS
// mux based based off who's turn to assert data on shared bus
// turn is based off current clock state due to neg/posedge logic
assign data_bus_up = !ClkIn ? data_bus_up_o : 'z;
assign data_bus_dn = ClkIn ? data_bus_dn_o : 'z;
assign data_bus_l  = ClkIn ? data_bus_l_o  : 'z;
assign data_bus_r  = !ClkIn ? data_bus_r_o  : 'z;

// read the shared net back in
assign data_bus_up_in = data_bus_up;
assign data_bus_dn_in = data_bus_dn;
assign data_bus_l_in  = data_bus_l;
assign data_bus_r_in  = data_bus_r;

assign buf_status_self = network_m_cnt;


logic packet_found; // signal bcs icarus likes complaining (can't use break)
// READ LOGIC : rptr + occupancy count
always_comb begin
  selected_data_packet = local_data_packet; // default to empty-network case
  network_rptr         = '0;
  packet_found         = 1'b0;

  for (int mem_idx = 0; mem_idx < `NETWORK_MEM_DEPTH; mem_idx++) begin
    if (!network_m_full[mem_idx] && !packet_found) begin
      selected_data_packet = network_mem[mem_idx];
      network_rptr         = mem_idx;
      packet_found                = 1'b1;
    end
  end

  // occupancy count
  network_m_cnt = '0;
  for (int mem_idx = 0; mem_idx < `NETWORK_MEM_DEPTH; mem_idx++)
    network_m_cnt = network_m_cnt + (network_m_full[mem_idx] ? 1'b1 : 1'b0);
end


// WRITE-DECISION LOGIC : valid chain, Wen, slot_src priority encode
always_comb begin
  // LOCAL CORE, UP, DOWN, LEFT, RIGHT (0->4), core = LSB = highest prio
  core_mem_passthrough = !(|network_m_full); // network empty -> pass straight through
  valid_arr            = {read_n, (core_mem_valid && !core_mem_passthrough)};

  for (int mem_idx = 0; mem_idx < `NETWORK_MEM_DEPTH; mem_idx++) begin 
    if (mem_idx == 0)
      valid_arr_chain[mem_idx] = valid_arr;              // first slot gets raw value
    else
      valid_arr_chain[mem_idx] = valid_arr_chain[mem_idx-1]; // rest chained

    // Wen if (free or being read this cycle) && valid data remaining
    network_m_Wen[mem_idx] = (!network_m_full[mem_idx] ||
                              (network_rptr == mem_idx && |assert_n))
                             && |valid_arr_chain[mem_idx];

    // priority-encode the winning source (LSB = core = highest prio)
    slot_src[mem_idx] = '0;
    if (network_m_Wen[mem_idx]) begin
      if      (valid_arr_chain[mem_idx][0]) slot_src[mem_idx] = 5'b00001;
      else if (valid_arr_chain[mem_idx][1]) slot_src[mem_idx] = 5'b00010;
      else if (valid_arr_chain[mem_idx][2]) slot_src[mem_idx] = 5'b00100;
      else if (valid_arr_chain[mem_idx][3]) slot_src[mem_idx] = 5'b01000;
      else if (valid_arr_chain[mem_idx][4]) slot_src[mem_idx] = 5'b10000;
    end

    // reduced mask for next slot: clear the WON source bit
    valid_arr_chain[mem_idx] = valid_arr_chain[mem_idx] & ~slot_src[mem_idx];
  end
end


// OUTPUT MUX : drive selected packet onto the per-direction *_o register input
always_comb begin
  data_bus_up_o = '0;
  data_bus_dn_o = '0;
  data_bus_l_o  = '0;
  data_bus_r_o  = '0;

  unique case (assert_n)
    4'b0001: data_bus_up_o = selected_data_packet;
    4'b0010: data_bus_dn_o = selected_data_packet;
    4'b0100: data_bus_l_o  = selected_data_packet;
    4'b1000: data_bus_r_o  = selected_data_packet;
    default: ; // all default to '0 (set above)
  endcase
end



// 1/2 cycle buffering on negedge signals 
always_ff @(negedge ClkIn) begin
  if (!ResetIn_b) begin
      valid_l_in_buf <= 1'b0;
      valid_dn_in_buf <= 1'b0;
  end
  else begin // only high for one cycle, gets read or dropped
      if (valid_l_in) valid_l_in_buf <= 1'b1;
      else valid_l_in_buf <= 1'b0;

      if (valid_dn_in) valid_dn_in_buf <= 1'b1;
      else valid_dn_in_buf <= 1'b0;
  end
end

always_ff @(negedge ClkIn) begin
    if (valid_l_in) negedge_buffer_l <= data_bus_l_in;
    if (valid_dn_in) negedge_buffer_dn <= data_bus_dn_in;
end


// network memory: gate on valid inputs 
// writes happen on pos, reads happen on pos and neg
logic valid_input;
// this path could be long... run through timing and potentially remove clock gating
// TODO: use cg std cell or explicit clock enable w/ latch
assign valid_input = valid_up_in | valid_dn_in_buf | valid_l_in_buf | valid_r_in;
logic network_clk;
assign network_clk = ClkIn && (valid_input); // network is unchanged unless we write

always_ff @(posedge network_clk) begin
  // for each slot index into the write enable and valid chain for that slot
  // then used fixed priority to choose the next input if Wen 
  // might not need (we dont read if free buffer is all free....)
  if (!ResetIn_b) begin
    // network mem can be garbage
    for (int i = 0; i < `NETWORK_MEM_DEPTH; i++)
      network_mem[i] <= '0;   // sim complain
  end
  else begin
    for (int mem_idx = 0; mem_idx < `NETWORK_MEM_DEPTH; mem_idx++) begin : NetworkMemory
      if (network_m_Wen[mem_idx]) begin
        unique case (slot_src[mem_idx])
          5'b00001: network_mem[mem_idx] <= local_data_packet;
          5'b00010: network_mem[mem_idx] <= data_bus_up_in;
          5'b00100: network_mem[mem_idx] <= negedge_buffer_dn;
          5'b01000: network_mem[mem_idx] <= negedge_buffer_l;
          5'b10000: network_mem[mem_idx] <= data_bus_r_in;
          // should never reach this
          // default:  $error("NetworkedCore: Wen high but slot_src empty (slot %0d)", mem_idx); 
          // System task ($error) cannot be synthesized in an always_ff process.^
        endcase
        // if no cases match: Wen was high (there was valid data and this slot was free)
        // and then the valid_arr was empty this should never happen
      end
    end
  end
end

// FREE STATUS BUFFER FOR NETWORK LAYER ENTRIES
// writes into the network only happen on posedge
// reads happen on both edges of the clock
// BE CAREFUL HERE
// switched from free to full so reset to 0 means empty

logic [`NETWORK_MEM_DEPTH-1:0] full_next;
always_comb begin
  for (int mem_idx = 0; mem_idx < `NETWORK_MEM_DEPTH; mem_idx++) begin
    // if we are reading and not writing (posedge && index are equal) 
    // either need found signal or default case for network_rptr
    // fire when rptr == same idx, not writing in, and reading out this edge

    // default case, same as prev
    full_next = network_m_full[mem_idx];
    // reading out and not writing in
    if (network_rptr == mem_idx && (!network_m_Wen[mem_idx] | ClkIn) && packet_found) begin 
        full_next[mem_idx] = 1'b1; // free slot
    end
    // writing and not reading (happens on posedge only)
    else if (network_m_Wen[mem_idx] == mem_idx && !ClkIn) begin
        full_next[mem_idx] = 1'b0; // just wrote valid data into slot
    end
  end
end

// could clock gate here: ClkIn && (|valid in || !(|network_m_full))
// clk off if no valids inputs and nothing to be read out
generate 
  genvar network_idx;
    for (network_idx = 0; network_idx < `NETWORK_MEM_DEPTH; network_idx++) begin : NetworkMemoryStatus
      DualEdgeFf dual_edge_ff (
        .clk(ClkIn), 
        .rst_n(ResetIn_b), 
        .d(full_next[network_idx]), 
        .q(network_m_full[network_idx])
      );
    end
endgenerate


// assertions:

// slot source maps between inputs and their network write address
// each index should be one hot or 0
// jk icraus doesnt allow concurrent assertions for some fucking reason
// genvar i;
// generate
//   for (i = 0; i < N; i++) begin : slot_src_0onehot
//     check_onehot0 : assert property (
//       @(posedge clk) disable iff (!ResetIn_b)
//       $onehot0(arr[i])
//     ) else $error("arr[%0d] = %b is neither one-hot nor zero", i, arr[i]);
//   end
// endgenerate

endmodule