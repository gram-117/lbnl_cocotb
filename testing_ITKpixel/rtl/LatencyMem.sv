
//-----------------------------------------------------------------------------------------------------
// [Filename]       LatencyMem.sv [RTL]
// [Project]        HL-LHC ATLAS/CMS pixel readout chip
// [Author]         -
// [Language]       SystemVerilog 2012 [IEEE Std. 1800-2012]
// [Created]        RTL code derived from FE-I4 and FE65_P2 projects
// [Modified]       -
//
// [Clock]          - Clk:       40 MHz clock
// [Reset]          - Reset_b:   Synchronous active low
//
// [Description]    Common per pixel-region timestamp memory and trigger-matching/arbitration logic.
//                  ATLAS double-trigger supported.
// [Notes]          -
// [Status]         devel
//-----------------------------------------------------------------------------------------------------

`ifndef LATENCY_MEM__SV   // include guard
`define LATENCY_MEM__SV

//
// Dependencies:
// //
// `include "rtl/common/defines.sv"
// `include "rtl/common/CgWrapper.v"   // hard-coded clock-gating module

// `include "rtl/core/dba/LatencyMemCell.sv"

`timescale 1ns / 1ps
//`include "timescale.v"

module LatencyMem (

    // clock and reset
    input  wire Clk,                                        // 40 MHz
    input  wire Reset_b,                                    // Synchronous reset, active low
 
    // from pixels
    input  wire HitLeAny,                                   // leading-edge detector flag from any firing pixel, i.e all HitLe flags OR-ed together 
    input  wire PwrDwnAll,                                  // turn-off the entire logic if all 4 pixels are powere-off
 
    // timestamp/trigger section
    input  wire [`LATENCY_COUNTER_BITS-1:0] LatCnt,         // Gray-encoded BX timestamp
    input  wire [`LATENCY_COUNTER_BITS-1:0] LatCntReq,      // Gray-encoded BX timestamp shifted back in time by programmable trigger latency
    input  wire Trig,                                       // L1A trigger
    input  wire TrigClear,                                  // double-trigger support (ATLAS-only)
    input  wire [`TRIG_ID_BITS-1:0] TrigId,                 // Gray-encoded timestamp associated to the received trigger (from chip periphery)
    input  wire [`TRIG_ID_BITS-1:0] TrigIdReq,              // TrigId shifted back in time by programmable trigger latency (from chip periphery)
 
    // region-data readout
    input  wire TokIn,                                      // token from previous pixel-region
    output wire TokOut,                                     // token to next pixel-region
    input  wire Read,                                       // read request issued from Core-Column Readout Controller (CCRC) in the chip periphery
    output wire [`LATENCY_MEM_DEPTH-1:0] TotMemWriteAddr,   // write pointer for the per-pixel ToT memories, selects the memory slot to be used to store new ToT values
    output wire [`LATENCY_MEM_DEPTH-1:0] TotMemReadAddr,    // read pointer for the per-pixel ToT memories, selects memory slot to read ToT values from
    output wire DataToCoreEn                                // output data MUX enable
 
    ) ;
 

    /////////////////////////////
    //   common clock gating   //
    /////////////////////////////
 
    // OR-together all ClkLatMemEn output flags from replicated LatencyMemCell modules to enable the clock
    wire [`LATENCY_MEM_DEPTH-1:0] clk_lat_mem_en ;
    wire clk_lat_mem_gated ;
 
    CG_MOD cg_start(.ClkIn(Clk), .Enable(|clk_lat_mem_en), .ClkOut(clk_lat_mem_gated)) ;
 
 
 
    /////////////////////////////////////////////////////////
    //    shared timestamp memory/trigger matching logic   // 
    /////////////////////////////////////////////////////////
 
    //
    // **REM: Each ToT memory-slot is uniquely associated (hard-wired) with one and only one
    //        timestamp memory-slot/trigger matching controller :
    //
    //   TotMem[0][3:0] x 4 pixels  =>   LatencyMem[0][`LATENCY_COUNTER_BITS-1:0]
    //   TotMem[1][3:0] x 4 pixels  =>   LatencyMem[1][`LATENCY_COUNTER_BITS-1:0]
    //   TotMem[2][3:0] x 4 pixels  =>   LatencyMem[2][`LATENCY_COUNTER_BITS-1:0]
    //      ...                             ...
    //   TotMem[i][3:0] x 4 pixels  =>   LatencyMem[i][`LATENCY_COUNTER_BITS-1:0]
    //      ...                             ...
    //   TotMem[7][3:0] x 4 pixels  =>   LatencyMem[7][`LATENCY_COUNTER_BITS-1:0]  (assuming `LATENCY_MEM_DEPTH = 8)
    //
    // If the trigger-latency elapses and no trigger has been received during it, the i-th slot
    // will be marked as "free" and at the next HitLeAny flag ToT values will be saved in that location.
    //
 
    // 1:1 write/read/handshake signals
    wire [`LATENCY_MEM_DEPTH-1:0] ready_to_read ;
    wire [`LATENCY_MEM_DEPTH-1:0] full ;   //synopsys keep_signal_name "full"
    wire [`LATENCY_MEM_DEPTH-1:0] write_addr_lat_mem ;     // write-pointer for the timestamp memory
    wire [`LATENCY_MEM_DEPTH-1:0] read_addr_lat_mem ;      // read-pointer for the timestamp memory
    wire [`LATENCY_MEM_DEPTH-1:0] free_write_addr ;        // selects which ToT memory location to mark as "free" for next write operation according to full-flags from each timestamp memory slot
 
    //wire [3:0] data [`LATENCY_MEM_DEPTH-1:0] ;   **UNUSED, commented out
 
    generate
 
        genvar k ;
 
        // generate `LATENCY_MEM_DEPTH timestamp memory-slots/trigger matching controllers
        for (k = 0; k < `LATENCY_MEM_DEPTH; k++) begin : TimestampMem
  
            LatencyMemCell  LatencyMemCell (
   
                .Clk         (                    clk_lat_mem_gated ),
                .Reset_b     (                              Reset_b ),
                .WriteLatMem (                write_addr_lat_mem[k] ),
                .ReadLatMem  (                 read_addr_lat_mem[k] ),
                .LatCnt      (    LatCnt[`LATENCY_COUNTER_BITS-1:0] ),
                .LatCntReq   ( LatCntReq[`LATENCY_COUNTER_BITS-1:0] ),
                .Trig        (                                 Trig ),
                .TrigClear   (                            TrigClear ),
                .TrigId      (            TrigId[`TRIG_ID_BITS-1:0] ),
                .TrigIdReq   (         TrigIdReq[`TRIG_ID_BITS-1:0] ),
                .ClkLatMemEn (                    clk_lat_mem_en[k] ),
                .ReadyToRead (                     ready_to_read[k] ),
                .Full        (                              full[k] )
   
               ) ;
  
         end
    endgenerate
  
  
    generate

        genvar m ;
  
        // loop over memory slots and mark them as "free" if no timestamp is stored in the centrale memory 
        for (m = 0; m < `LATENCY_MEM_DEPTH; m++) begin : FreeTotMemWriteAddrGen
            if( m == 0 )
                assign free_write_addr[m] = (full[m] == 1'b0) ? 1'b1 : 1'b0 ;
   
            else
                assign free_write_addr[m] = (full[m] == 1'b0 & ( &full[m-1:0] )) ;
        end
    endgenerate 
 
 
    // write-pointer for timestamp memory: write in slots marked as "free" if a leading-edge is detected from any firing pixel
    assign write_addr_lat_mem[`LATENCY_MEM_DEPTH-1:0] = {`LATENCY_MEM_DEPTH{HitLeAny}} & free_write_addr[`LATENCY_MEM_DEPTH-1:0] ;
 
    // read-pointer for timestamp memory: a Read request arrives from chip periphery 
    assign read_addr_lat_mem[`LATENCY_MEM_DEPTH-1:0] = (DataToCoreEn & Read) ? ready_to_read[`LATENCY_MEM_DEPTH-1:0] : {`LATENCY_MEM_DEPTH{1'b0}} ;
 
    // write-pointer for per-pixel ToT memory
    assign TotMemWriteAddr[`LATENCY_MEM_DEPTH-1:0] = free_write_addr[`LATENCY_MEM_DEPTH-1:0]  ;
 
    // read-pointer to get ToT values from selected ToT memory slot
    assign TotMemReadAddr[`LATENCY_MEM_DEPTH-1:0] = (DataToCoreEn == 1'b1) ? ready_to_read[`LATENCY_MEM_DEPTH-1:0] : {`LATENCY_MEM_DEPTH{1'b0}} ;
 
 
 
    /////////////////////////////////////////////////////
    //   OR-based token and output arbitration logic   //
    /////////////////////////////////////////////////////
 
    // enable the readout if at least one memory-slot has been triggered and is ready for reading
    wire is_triggered_to_read ;
 
    //assign is_triggered_to_read = (ready_to_read != '0) ;
    //assign is_triggered_to_read = (ready_to_read[`LATENCY_MEM_DEPTH-1:0] != '0) ? 1'b1 : 1'0 ;
    assign is_triggered_to_read = |ready_to_read[`LATENCY_MEM_DEPTH-1:0] ;
 
 
    wire token_rise ; //synopsys keep_signal_name "token_rise"
    assign token_rise = is_triggered_to_read & ~PwrDwnAll ;
 
    // token-OR
    assign TokOut = token_rise | TokIn ;
 
    // enable for data readout
    assign DataToCoreEn = (TokIn == 0 && is_triggered_to_read == 1'b1) & (~PwrDwnAll) ;
 
    `ifdef SVA_EN
    `ifndef CORE_GL
        logic[`LATENCY_COUNTER_BITS-1:0] aux_latcnt, aux_latcnt_delay;
        logic[`LATENCY_COUNTER_BITS-1:0] aux_latcnt_gray, aux_latcnt_delay_gray;
        logic[2:0] aux_disable_lat_check;

        default clocking DEF_CLK@(posedge Clk); endclocking

        READ_SHAPE: assume property (
            disable iff (~Reset_b || $isunknown(Reset_b) || $isunknown(Read))
            Read == 1'b1 |-> ##1 ~Read ##[1:2] ~Read
        );
        
        `ifndef FORMAL
            assign aux_latcnt_gray = LatCnt;
            assign aux_latcnt_delay_gray = LatCntReq;

            always_comb begin
                logic[`LATENCY_COUNTER_BITS-1:0] tmp;

                aux_latcnt  = LatCnt;
                tmp         = LatCnt;
                for (int bb = 0; bb < `LATENCY_COUNTER_BITS; bb++) begin
                    tmp >>= 1;
                    aux_latcnt ^= tmp;
                end

                aux_latcnt_delay = LatCntReq;
                tmp              = LatCntReq;
                for (int bb = 0; bb < `LATENCY_COUNTER_BITS; bb++) begin
                    tmp >>= 1;
                    aux_latcnt_delay ^= tmp;
                end
            end

            always @(DEF_CLK) begin
                if (Reset_b == 1'b0)
                    aux_disable_lat_check <= 3'b0;
                else begin
                    aux_disable_lat_check[0] <= $changed(RD53C.ChipBottom.DigitalChipBottom.LatencyConfig);
                    aux_disable_lat_check[1] <= aux_disable_lat_check[0];
                    aux_disable_lat_check[2] <= aux_disable_lat_check[1];
                end
            end

            RISING_CNT: assume property (
                disable iff (~Reset_b || $isunknown(Reset_b) || $isunknown(LatCnt) || aux_latcnt == 0 || $past(aux_latcnt, , , @(DEF_CLK)) == 0)
                !$rose(Reset_b) |=> aux_latcnt == $past(aux_latcnt)+9'b1
            );

            AUX_LATENCY:    assume property (
                disable iff (~Reset_b || $isunknown(Reset_b) || $isunknown(LatCnt) || aux_disable_lat_check != 3'b0)
                aux_latcnt_delay == aux_latcnt-RD53C.ChipBottom.DigitalChipBottom.LatencyConfig
            );
        `endif
        
        `ifdef FORMAL
            ALWAYS_ON:      assume property (PwrDwnAll == 0);
            NO_CLEAR:       assume property (TrigClear == 0);
            LATENCY_CFG:    assume property (RD53C.ChipBottom.DigitalChipBottom.LatencyConfig == 20);
            HIT_ACTIVITY:   assume property ($fell(HitLeAny) |=> ##[0:14] HitLeAny);

            always @(DEF_CLK) begin
                if (~Reset_b) aux_latcnt <= '{(`LATENCY_COUNTER_BITS){1'b0}};
                else aux_latcnt <= aux_latcnt+1;
            end
            assign aux_latcnt_delay = aux_latcnt-RD53C.ChipBottom.DigitalChipBottom.LatencyConfig;

            assign aux_latcnt_gray = aux_latcnt ^ (aux_latcnt >> 1);
            assign aux_latcnt_delay_gray = aux_latcnt_delay ^ (aux_latcnt_delay >> 1);
            
            COUNTER: assume property (LatCnt == aux_latcnt_gray);
            LATENCY: assume property (LatCntReq == aux_latcnt_delay_gray);
        
            //genvar dd;
            //for (dd = 1; dd < 14; dd++) begin: HIT_DOWN
            //    HIT_LE_SHAPE: cover property($fell(HitLeAny) ##(dd) $rose(HitLeAny));
            //end

            WRAP_CNT: cover property (aux_latcnt == 9'h1ff ##1 aux_latcnt == 9'h0);

            GETS_FULL:  cover property ($countones(full) == `LATENCY_MEM_DEPTH);
            GETS_EMPTY: cover property ($countones(full) == `LATENCY_MEM_DEPTH ##[*] $countones(full) == 0);

            genvar bb;
            for (bb = 0; bb < `LATENCY_MEM_DEPTH; bb++) begin : CAN_READ
                CELL_CAN_READ: cover property (full[bb] == 1 ##[0:49] full[bb] == 0);
            end
            for (bb = 0; bb < `LATENCY_MEM_DEPTH; bb++) begin : WRITE_CELL
                CELL_WRITE: assert property ($rose(full[bb]) |-> $past(HitLeAny));
            end
            for (bb = 0; bb < `LATENCY_MEM_DEPTH; bb++) begin : FULL
                READ_EMPTY: assert property (@(posedge TimestampMem[bb].LatencyMemCell.gated_clk) full[bb] && $rose(ready_to_read[bb]) |=> ##[0:2] ~full[bb]);
                LATENCY_EMPTY: assert property (
                    full[bb] && $rose(TimestampMem[bb].LatencyMemCell.counter_last)
                    ##0 ~TimestampMem[bb].LatencyMemCell.trig_state[*2] |-> ~full[bb]
                );
                COUNTER_EXPIRES: cover property (TimestampMem[bb].LatencyMemCell.counter_last);
            end
        `endif //FORMAL
    `endif //CORE_GL
    `endif //SVA_EN
 
    // **OBSOLETE ! Delete ?
    //wire [`LATENCY_MEM_DEPTH-1:0] trigger_read ;
    //generate
    //   genvar t ;
    //  for(t = 0; t < `LATENCY_MEM_DEPTH; t++ begin : read_dec
    //     if(t==0)
    //        assign trigger_read[t] = (ready_to_read[t]==1) ;
    //     else
    //        assign trigger_read[t] = (ready_to_read[t]==1 & !( |ready_to_read[t-1:0] )) ;
    //   end
    //endgenerate
 
 
    // **OBSOLTE ! Delete ?
    //assign DataToCoreEn = (TokIn == 0 && is_triggered_to_read ) ;

endmodule : LatencyMem

`endif   // PIXEL_REGION_LATENCY_MEM__SV

