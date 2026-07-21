
//-----------------------------------------------------------------------------------------------------
// [Filename]       LatencyMemCell.sv [RTL]
// [Project]        HL-LHC ATLAS/CMS pixel readout chip
// [Author]         - 
// [Language]       SystemVerilog 2012 [IEEE Std. 1800-2012]
// [Created]        Code derived from FE-I4 and FE65_P2 projects.
// [Modified]       -
//
// [Clock]          - Clk:       40 MHz clock
// [Reset]          - Reset_b:   Synchronous active low
//
// [Description]    One-slot timestamp memory and FSM controller/trigger matching logic
//                  for the Distributed Bufferign Architecture (DBA).
//                  This cell is then replicated `LATENCY_MEM_DEPTH times in the common logic
//                  shared among all 1x4 pixels in the pixel region.
//
// [Notes]          LatCnt is the actual value stored in the memory when HitLeAny
//                  is asserted, while LatCntReq is used to perform trigger-matching.
//
// [Status]         devel
//-----------------------------------------------------------------------------------------------------


`ifndef LATENCY_MEM_CELL__SV   // include guard
`define LATENCY_MEM_CELL__SV


//
// Dependencies:
//
// `include "rtl/common/defines.sv"
// `include "rtl/common/CgWrapper.v"   // hard-coded clock-gating module


`timescale 1ns / 1ps
//`include "timescale.v"


module LatencyMemCell (

    // clock and reset
    input  wire Clk,                                     // 40 MHz clock 
    input  wire Reset_b,                                 // asynchronous reset, active low
 
    // write/read flags
    input  wire WriteLatMem,                             // write-enable for the memory (saves the current LatCnt in memory)
    input  wire ReadLatMem,                              // read-enable for the memory (but no data are actually read from the memory)
 
    // timestamp/trigger section
    input  wire [`LATENCY_COUNTER_BITS-1:0] LatCnt,      // Gray-encoded timestamp + latency
    input  wire [`LATENCY_COUNTER_BITS-1:0] LatCntReq,   // true BX count, i.e. timestamp shifted back in time by latency
    input  wire Trig,                                    // L1A trigger
    input  wire TrigClear,                               // double-trigger support (ATLAS chip)
    input  wire [`TRIG_ID_BITS-1:0] TrigId,              // trigger timestamp
 
    // output flags
    output wire ClkLatMemEn,                             // clock-gating enable also sent to shared-logic
    output wire ReadyToRead,                             // inform the region logic that trigger matching
    output wire Full                                     // inform the region logic if in this memory slot a timestamp is stored, waiting to check if a trigger arrives within the latency
 
    ) ;
 
    wire gated_clk ;   // gated clock for the entire logic, generated at the end of the module
 
    //////////////////////////////////////////
    //   timestamp memory slot (register)   //
    //////////////////////////////////////////
    //
    // **REM: Each "timestamp slot" (also referred to as "latency timer" or "latency counter" in the documentation
    //        but it is NOT a per-region counter, just a bank of registers).
    //        The timer is normally "idle" and no clock is revceived to safe power, until a hit comes
    //        from any firing pixel in the pixel
    //        region when the WriteLatMem input flag starts the counter.
    //
 
    logic start_state ;   // just single-FF flags
    logic trig_state ;
 
    wire req_to_read ;
    wire cleared ;
 
 
    logic [`LATENCY_COUNTER_BITS-1:0] counter ;
 

    // latched bcid == top level request BCID?
    wire counter_last ;
    // MODIFIED     assign counter_last = (counter[`LATENCY_COUNTER_BITS-1:0] == `[`LATENCY_COUNTER_BITS-1:0] ) & start_state & (!trig_state) ;
    assign counter_last = (counter[`LATENCY_COUNTER_BITS-1:0] == LatCntReq[`LATENCY_COUNTER_BITS-1:0] ) & start_state & (!trig_state) ;
 
 
    // trigger arrived !
    wire triggered ;
    assign triggered = counter_last & Trig ;
 
    always_ff @(posedge gated_clk) begin
 
        if (counter_last)       
        // // register the number identifying the trigger REUSE THE SAME FLIP FLOPS
            counter[`LATENCY_COUNTER_BITS-1:0] <= {LatCnt[`LATENCY_COUNTER_BITS-1:`TRIG_ID_BITS], TrigId[`TRIG_ID_BITS-1:0]} ;    // saves gates and improves testability

        else if(WriteLatMem)
        // get current LatCnt
            counter[`LATENCY_COUNTER_BITS-1:0] <= LatCnt[`LATENCY_COUNTER_BITS-1:0] ;
 
    end   // always_ff
 
 
 
 
    ///////////////////////////////////////////
    //   timestamp memory controller (FSM)   //
    ///////////////////////////////////////////

    //*****Memory FSM states******
    //           start_state  trig
    // IDLE        0          0
    // COUNTING    1          0
    // TRIGGERED   1          1
    // TOREAD      0          1
    //****************************

    //synopsys sync_set_reset "Reset_b"

    //always_ff @(posedge gated_clk or negedge Reset_b) begin     **WARN: changed to SYNCHRONOUS RESET, ref. to issue#132
    always_ff @(posedge gated_clk) begin
        if(~Reset_b)
            start_state <= 1'b0 ;
  
        else if(WriteLatMem | triggered)
            start_state <= 1'b1 ;
  
        else
            start_state <= 1'b0 ;
    end   // always_ff
 
 
    //always_ff @(posedge gated_clk or negedge Reset_b) begin   **WARN: changed to SYNCHRONOUS RESET, ref. to issue#132
    always_ff @(posedge gated_clk) begin
        if(~Reset_b)
            trig_state <= 1'b0 ;
  
        else if(triggered | req_to_read)
            trig_state <= 1'b1 ;
  
        else
            trig_state <= 1'b0 ;
  
    end   // always_ff
 
    assign Full = start_state | trig_state ;
 
 
    /////////////////////////
    //   trigger matchig   //
    /////////////////////////
 
    // compare trigger timestamp (shifted back in time by trigger-latency) with timestamp stored in memory
    wire trig_id_match ;
    assign trig_id_match = (counter[`TRIG_ID_BITS-1:0] == TrigIdReq[`TRIG_ID_BITS-1:0] ) ? 1'b1 : 1'b0 ;
 
    // trigger matched before trigger latency elapses, it's time to get ToT data from per-pixel ToT memories !
    assign req_to_read = start_state & trig_state & trig_id_match & !TrigClear;
 
    assign cleared = trig_state & trig_id_match & TrigClear ;
 
    // trigger matched ! it's time to get ToT data from per-pixel TOT memories 
    assign ReadyToRead = trig_state & !start_state ; 
 
 
    //////////////////////
    //   clock gating   //
    //////////////////////
 
    assign ClkLatMemEn = ~Reset_b | WriteLatMem | counter_last  | req_to_read | ReadLatMem | cleared ;
 
    CG_MOD cg_start_state(.ClkIn(Clk), .Enable(ClkLatMemEn), .ClkOut(gated_clk)) ; 
 
 
endmodule : LatencyMemCell

`endif   // LATENCY_MEM_CELL__SV

