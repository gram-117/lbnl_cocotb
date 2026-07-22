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

// define packet to be: addr trig data **

// data from other cores takes >= 1 cycle to be clocked in and then out
// if network memory is empty, local core data is routed straight out

// two cores cannot route data to each other, this is unproductive and also 
// allows us to use one shared bus between all cores

// notes:
/*
TODO: 
FIGURE OUT CONFIG STUFF
FIGURE OUT DISABLING/FAULT TOLERANCE

ADD STEADY STATE 1 CYCLE HIT (clock in and out of same register)
need to do this by chooiing to write into the slot that is selected by the rptr
^ done need to be verified!!!!!


*/

module NetworkedCore (
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
    input logic valid_r_in,

// for simulation icarus doesnt like packed arr
  output logic [`PACKET_SIZE-1:0] network_mem_tb_0,
  output logic [`PACKET_SIZE-1:0] network_mem_tb_1,
  output logic [`PACKET_SIZE-1:0] network_mem_tb_2,
  output logic [`PACKET_SIZE-1:0] network_mem_tb_3,
  output logic [`PACKET_SIZE-1:0] network_mem_tb_4,
  output logic [`PACKET_SIZE-1:0] network_mem_tb_5,
  output logic [`PACKET_SIZE-1:0] network_mem_tb_6,
  output logic [`PACKET_SIZE-1:0] network_mem_tb_7
);

assign network_mem_tb_0 = network_mem[0];
assign network_mem_tb_1 = network_mem[1];
assign network_mem_tb_2 = network_mem[2];
assign network_mem_tb_3 = network_mem[3];
assign network_mem_tb_4 = network_mem[4];
assign network_mem_tb_5 = network_mem[5];
assign network_mem_tb_6 = network_mem[6];
assign network_mem_tb_7 = network_mem[7];



// gated clock for network memory, high on valids 
logic ClkNetGated;
logic found;// bullshit signal bcs icarus likes complaining


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
logic [`NETWORK_MEM_DEPTH-1:0] network_m_free;
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
logic [3:0] transact_n;

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


// // ROUTER
// abstracty router will take in local buffer status, neighboring buffer status, 
// and output a routing decision (set one of the valid signals to high)
RouterStub router_stub_inst (
    .buf_status_up (buf_status_up),
    .buf_status_dn (buf_status_dn),
    .buf_status_l  (buf_status_l),
    .buf_status_r  (buf_status_r),
    .buf_status_self (network_m_cnt),

    .routing_decision (routing_decision)
);

// do we have valid data? (data in network or core bypass)

assign local_data_valid = network_m_cnt > 0 || (core_mem_passthrough && core_mem_valid);

assign valid_up_o = routing_decision[0] && local_data_valid; 
assign valid_dn_o = routing_decision[1] && local_data_valid; 
assign valid_l_o = routing_decision[2] && local_data_valid; 
assign valid_r_o = routing_decision[3] && local_data_valid; 


// need some decision making to choose which slots next for now just fixed priority
// Router2 Router2 (
// )
// prolly won't be a module though (wptr in memory)

// Transaction occurs when exactly one side asserts valid
// Order: Up, Down, Left, Right
always_ff @(negedge ClkIn) begin
    if (!ResetIn_b) begin
        transact_n <= '0;
    end else begin
        transact_n[0] <= valid_up_o ^ valid_up_in;
        transact_n[1] <= valid_dn_o ^ valid_dn_in;
        transact_n[2] <= valid_l_o  ^ valid_l_in;
        transact_n[3] <= valid_r_o  ^ valid_r_in;
    end
end


// this core is asserting for a transaction
assign assert_n[0] = transact_n[0] && valid_up_o;
assign assert_n[1] = transact_n[1] && valid_dn_o;
assign assert_n[2] = transact_n[2] && valid_l_o;
assign assert_n[3] = transact_n[3] && valid_r_o;

// Neighbor owns the bus; this core reads from it.
assign read_n[0] = transact_n[0] && valid_up_in;
assign read_n[1] = transact_n[1] && valid_dn_in;
assign read_n[2] = transact_n[2] && valid_l_in;
assign read_n[3] = transact_n[3] && valid_r_in;

// TODO CHECK THIS
// mux based based off who's turn to assert data on shared bus
  // per-direction assert index; drive net only when asserting, else 'z
  assign data_bus_up = assert_n[0] ? data_bus_up_o : 'z;
  assign data_bus_dn = assert_n[1] ? data_bus_dn_o : 'z;
  assign data_bus_l  = assert_n[2] ? data_bus_l_o  : 'z;
  assign data_bus_r  = assert_n[3] ? data_bus_r_o  : 'z;

  // read the shared net back in
  assign data_bus_up_in = data_bus_up;
  assign data_bus_dn_in = data_bus_dn;
  assign data_bus_l_in  = data_bus_l;
  assign data_bus_r_in  = data_bus_r;

assign buf_status_self = network_m_cnt;



// READ LOGIC : rptr + occupancy count
always_comb begin
  selected_data_packet = local_data_packet; // default to empty-network case
  network_rptr         = '0;
  found                = 1'b0;

  for (int mem_idx = 0; mem_idx < `NETWORK_MEM_DEPTH; mem_idx++) begin
    if (!network_m_free[mem_idx] && !found) begin
      selected_data_packet = network_mem[mem_idx];
      network_rptr         = mem_idx;
      found                = 1'b1;
    end
  end

  // occupancy count
  network_m_cnt = '0;
  for (int mem_idx = 0; mem_idx < `NETWORK_MEM_DEPTH; mem_idx++)
    network_m_cnt = network_m_cnt + (network_m_free[mem_idx] ? 1'b0 : 1'b1);
end


// WRITE-DECISION LOGIC : valid chain, Wen, slot_src priority encode
always_comb begin
  // LOCAL CORE, UP, DOWN, LEFT, RIGHT (0->4), core = LSB = highest prio
  core_mem_passthrough = &network_m_free; // network empty -> pass straight through
  valid_arr            = {read_n, (core_mem_valid && !core_mem_passthrough)};

  for (int mem_idx = 0; mem_idx < `NETWORK_MEM_DEPTH; mem_idx++) begin : NetworkMemory
    if (mem_idx == 0)
      valid_arr_chain[mem_idx] = valid_arr;              // first slot gets raw value
    else
      valid_arr_chain[mem_idx] = valid_arr_chain[mem_idx-1]; // rest chained

    // Wen if (free or being read this cycle) && valid data remaining
    network_m_Wen[mem_idx] = (network_m_free[mem_idx] ||
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


// could gate clk on |valid  or some shit later.....
always_ff @(posedge ClkIn) begin
  // for each slot index into the write enable and valid chain for that slot
  // then used fixed priority to choose the next input if Wen 
  if (!ResetIn_b) begin
    // network mem can be garbage
    network_m_free <= '1; // start off with all slots free!
    for (int i = 0; i < `NETWORK_MEM_DEPTH; i++)
      network_mem[i] <= '0;   // no Z on unwritten reads
  end

  else begin
    // default case
    network_m_free <= network_m_free;

    for (int mem_idx = 0; mem_idx < `NETWORK_MEM_DEPTH; mem_idx++) begin : NetworkMemory
      if (network_m_Wen[mem_idx]) begin
        network_m_free[mem_idx] <= 1'b0; // just wrote data in its not free

        unique case (slot_src[mem_idx])
          5'b00001: network_mem[mem_idx] <= local_data_packet;
          5'b00010: network_mem[mem_idx] <= data_bus_up_in;
          5'b00100: network_mem[mem_idx] <= data_bus_dn_in;
          5'b01000: network_mem[mem_idx] <= data_bus_l_in;
          5'b10000: network_mem[mem_idx] <= data_bus_r_in;
          // should never reach this
          // default:  $error("NetworkedCore: Wen high but slot_src empty (slot %0d)", mem_idx); 
          // System task ($error) cannot be synthesized in an always_ff process.^
        endcase
        // if no cases match: Wen was high (there was valid data and this slot was free)
        // and then the valid_arr was empty this should never happen
      end
      // if reading from the node and not writing data in on the same cycle then clear
      // if no current status check, if empty -> already writes 1
      if (network_rptr == mem_idx && !network_m_Wen[mem_idx] && |assert_n) begin 
        network_m_free[mem_idx] <= 1'b1;
      end
    end


  end


end

endmodule