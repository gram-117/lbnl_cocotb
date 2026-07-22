
//-----------------------------------------------------------------------------------------------------
// [Filename]       PixelRegionLogic.sv [RTL]
// [Project]        HL-LHC ATLAS/CMS pixel readout chip
// [Author]         -
// [Language]       SystemVerilog 2012 [IEEE Std. 1800-2012]
// [Created]        Code derived and adapted from FE-I4 and FE65_P2 projects.
// [Modified]       -
// [Hierarchy]      - RD53B --- PixelArray --- DigitalCore --- PixelRegionLogic
//                  - PixelRegionLogic |
//                                     |--- PixelLogic
//                                     |--- LatencyMem
//
// [Clock]          - Clk:       40 MHz clock
// [Reset]          - Reset_b:   Synchronous active low
//
// [Description]    Implements the Distributed Buffering Architecture (DBA) assuming a pixel region
//                  of 1x4 pixels with independent per-pixel ToT counter/memory and common latency
//                  counter. The RTL is mainly structural code that instantiates and interconnects:
//
//                     - a common region-level clock-gating module to minimize power consumption
//                     - 4 independent per-pixel ToT counter/memory modules
//                     - the common timestamp latency buffer
//
// [Notes]          - 
// [Status]         devel
//-----------------------------------------------------------------------------------------------------
/* GRAMMY MODIFICATIONS:
removed trigidreq
added region trig out for packet generation
removed 6to4 mapping and 80mhz
removed CMS stuff
*/

`ifndef PIXEL_REGION_LOGIC__SV   // include guard
`define PIXEL_REGION_LOGIC__SV

//
// Dependencies:
//
// `include "rtl/common/defines.sv"

// `include "rtl/core/dba/PixelLogic.sv"
// `include "rtl/core/dba/LatencyMem.sv"


`timescale 1ns / 1ps
//`include "timescale.v"


module PixelRegionLogic (

   // clock and reset
   input  wire Clk,
   input  wire Reset_b,

   // Hit from analog FE or digital injection gets fanned out to individual PixelLogic
   input  wire [3:0] Hit,                                // hit pulses from pixels, any  firing pixel will start the common latency counter
   input  wire [3:0] PwrDwn,                             // if all front-ends in the pixel-region are turned off, turn off also the shared memory and tie-down output data

   // timestamp/trigger section
   input  wire [`LATENCY_COUNTER_BITS-1:0] LatCnt,       // Gray-encoded BX timestamp (from chip periphery)
   input  wire [`LATENCY_COUNTER_BITS-1:0] LatCntReq,    // BX timestamp shifted by programmable trigger latency (from chip periphery)
   input  wire Trig,                                     // L1A trigger
   input  wire TrigClear,                                // double-trigger support (ATLAS-only)
   input  wire [`TRIG_ID_BITS-1:0] TrigIdIn,               // BX timestamp associated to the received trigger (from chip periphery)

   // data readout
   input  wire TokIn,                                    // token from previous pixel-region
   output wire TokOut,                                   // token to next pixel-region
   input  wire Read,                                     // read request from chip periphery
   output wire [`REGION_DATA_BITS-1:0] DataToCore,                         // triggered ToT values from pixels
   output wire [`TRIG_ID_BITS-1:0] RegionTrigOut
   ) ;
// latency config 

   //////////////////////////////////////////
   //   common region-level clock gating   //
   //////////////////////////////////////////

   //
   // **NOTE: The clock is enabled to pixels if:
   //
   //    - at least one hit is detected (hit_le_any = 1) OR
   //    - at least one no-overflow occur in ToT counters
   //

   wire [3:0] hit_le ;         // HitLe flags from pixels
   wire [3:0] pixel_clk_en ;   // PixelClkEn flags from pixels

   wire hit_le_any ;
   assign hit_le_any = |hit_le[3:0] ;  // asserted if any of 1x4 pixels fires

   wire pixel_clk_en_any ;
   assign pixel_clk_en_any = |pixel_clk_en[3:0] ;

   wire pixel_clk_gated_en, pixel_clk_gated ;

   assign pixel_clk_gated_en = hit_le_any | pixel_clk_en_any ;

   CG_MOD CG_clk(.ClkIn(Clk), .Enable(pixel_clk_gated_en), .ClkOut(pixel_clk_gated)) ;



   //////////////////////////////////////////////////////////////////////////
   //   replicate per-pixel hit/ToT control logic and ToT latency memory   //
   //////////////////////////////////////////////////////////////////////////

   // internal signals for interconnections
   wire [`LATENCY_MEM_DEPTH-1:0] tot_mem_write_addr ;  // write-pointer from the common latency buffer (i.e. in which memory slot the ToT must be saved, same for all pixels)
   wire [`LATENCY_MEM_DEPTH-1:0] tot_mem_read_addr ;   // read-pointer from the common latency buffer (i.e. from which memory location ToT is retrieved when a trigger arrives, same for all pixels)

   wire [3:0] pixel_data [3:0] ;   // ToT values from all 4 pixels in the region


   generate

      genvar p ;

      for (p = 0; p < 4; p++) begin : Pixel

         PixelLogic   PixelLogic (

            // clock and reset
            .PixelClkEn       (                            pixel_clk_en[p] ),
            .PixelClkGated    (                            pixel_clk_gated ),
            .Reset_b          (                                    Reset_b ),

            // global configuration
            .Tot6to4Mapping   (                             Tot6to4Mapping ),
            .TotDualEdgeCount (                           TotDualEdgeCount ),

         `ifdef CMS_CHIP
            .HitSampleMode    (                              HitSampleMode ),
         `endif

            // hit/ToT control logic
            .Hit              (                                     Hit[p] ),
            .HitLe            (                                  hit_le[p] ),
            .HitLeAny         (                                 hit_le_any ),

            // write/read data to/from ToT memory
            .TotMemWriteAddr  ( tot_mem_write_addr[`LATENCY_MEM_DEPTH-1:0] ),
            .TotMemReadAddr   (  tot_mem_read_addr[`LATENCY_MEM_DEPTH-1:0] ),
            .TotMemDataOut    (                         pixel_data[p][3:0] )

         ) ;

      end   // for
   endgenerate

   ////////////////////////////////////////////////////////////////////////////////
   //   shared timestamp latency memory and trigger-matching/arbitration logic   //
   ////////////////////////////////////////////////////////////////////////////////


   // if ALL pixels in the pixel-region are turned off, turn off also the shared memory
   wire pwr_dwn_all ;
   assign pwr_dwn_all = &PwrDwn[3:0] ;

   wire data_to_core_en;   // data-output enable from common latency memory
   wire [`TRIG_ID_BITS-1:0] trig_id_out;
   assign RegionTrigOut = (data_to_core_en == 1'b1) ? trig_id_out : '0;

   LatencyMem   LatencyMem (

      // clock and reset
      .Clk             (                                        Clk ), 
      .Reset_b         (                                    Reset_b ),

      // from pixels
      .HitLeAny        (                                 hit_le_any ),
      .PwrDwnAll       (                                pwr_dwn_all ),

      // timestamp/trigger section 
      .LatCnt          (          LatCnt[`LATENCY_COUNTER_BITS-1:0] ),
      .LatCntReq       (       LatCntReq[`LATENCY_COUNTER_BITS-1:0] ),
      .Trig            (                                       Trig ),
      .TrigClear       (                                  TrigClear ),
      .TrigId          (                  TrigIdIn[`TRIG_ID_BITS-1:0] ),

      // region-data readout
      .TokIn           (                                      TokIn ),
      .TokOut          (                                     TokOut ), 
      .Read            (                                       Read ), 
      .TotMemWriteAddr ( tot_mem_write_addr[`LATENCY_MEM_DEPTH-1:0] ),
      .TotMemReadAddr  (  tot_mem_read_addr[`LATENCY_MEM_DEPTH-1:0] ),
      .DataToCoreEn    (                            data_to_core_en ),
      .RegionTrigOut       (                                trig_id_out)
      );



   /////////////////////////
   //   output data MUX   //
   /////////////////////////

   // re-pack ToT values and send them to the core for readout
   wire [`REGION_DATA_BITS-1:0] region_data;
   assign region_data = {pixel_data[3][3:0], pixel_data[2][3:0], pixel_data[1][3:0], pixel_data[0][3:0]};

   //wire en_out ;
   //assign en_out = en_out_pre & ~pwr_dwn_all ;   // **WARN: pwr_dwn_all check now moved into LatencyMem

   // output MUX
   assign DataToCore[15:0] = (data_to_core_en == 1'b1) ? region_data[15:0] : 16'b0 ; 


endmodule : PixelRegionLogic

`endif   // PIXEL_REGION_LOGIC__SV

