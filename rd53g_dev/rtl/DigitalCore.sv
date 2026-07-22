
//-----------------------------------------------------------------------------------------------------
// [Filename]       DigitalCore.sv [RTL]
// [Project]        HL-LHC ATLAS/CMS pixel readout chip
// [Author]         -
// [Language]       SystemVerilog 2012 [IEEE Std. 1800-2012]
// [Created]        -
// [Modified]       -
// [Hierarchy]      - RD53B --- PixelArray --- DigitalCore
//                  - DigitalCore |
//                                |--- ProgrammableDelayBuff24
//                                |--- ProgrammableDelayBuff16
//                                |--- SigFork
//                                |--- SigForkConf
//                                |--- RD53B_ANALOG_QUAD_DIFF
//                                |--- FeControl
//                                |--- PixelRegionLogic
//                                |--- RegionAddrEnc
//
// [Clock]          - ClkIn, ClkOut: 40 MHz clock
// [Reset]          - ResetIn_b, ResetOut_b:( Synchronous active low)
//
// [Description]    Digital core grouping 8x8 pixels and implementing the Distributed Buffering
//
//                  Architecture (DBA) assuming pixel regions of 1x4 pixels.
//                  The code is mainly structural code that instantiates and interconnects:
//
//                     - delay lines for skew adjustment on clock and CalEdge signals
//                     - 4x4 analog quads of 2x2 pixels each one
//                     - 8x8 front-end controllers (hit-logic + per-pixel configuration register)
//                     - 16 pixel-regions grouping 1x4 pixels with distributed latency buffers
//                     - a single trigger matching and readout logic common to all pixel regions
//                     - core-to-core signals with proper buffering.
//
// [Notes]          Charge-injection is enabled by default for the 2 pixel rows at the top of the
//                  8x8 core (quad_row == 0). In RD53A this allowed to immediately probe analog
//                  waveforms at the oscilloscope using top pads without the need to configure the
//                  pixel array to enable calibration in top pixels.
//    
// [Status]         devel
//-----------------------------------------------------------------------------------------------------

// using different structure, files handled by makefile/cocotb
// `ifndef DIGITAL_CORE__SV   // include guard
// `define DIGITAL_CORE__SV

/******************************
// post rtl audit:
// assign pix_hit_analog = AnaHit; treats digital injections as hits at core level for sim
// also quad front or whatever is commented out but should just be for applying bias

// using token logic as is for core, pinning readIn high so core will always dump data
// token out is used as a valid signal

// added explicit signal from memory for the trigger ID (prev columns knew this already)

// removed CMS STUFF
// removed some global config HitSampleMode(CMS), ToT6to4, DualEdgeSelection (always on)

// removed logic surroudning passing data through cores 

// all config/bias stuff is commented out but keeping it around for reference when we do it again
**************************/


// `include "rtl/common/defines.sv"

// `ifndef DIGITAL_CORE_ABSTRACT
//     //
//     // Dependencies:
//     //
//     `include "rtl/models/models.sv"                     // for the "analog-quad" model
//     `include "rtl/core/dba/ProgrammableDelay.v"         // hard-coded delay chain
//     `include "rtl/core/dba/SigFork.sv"                  // hard-coded "signal fork" for time-sensitive core-to-core signals propagation
//     `include "rtl/core/dba/FeControl.sv"
//     `include "rtl/core/dba/PixelRegionLogic.sv"         // 1x4 pixel region with Distributed Buffering Architecture (DBA)
//     `include "rtl/core/dba/RegionAddrEnc.sv"
// `endif

`timescale 1ns / 1ps
//`include "timescale.v"


module DigitalCore (

   /////////////////////////////
   //   hits from testbench   //
   ///////////////////////////// 
   input wire [63:0] AnaHit,                // 8 x 8 pixels per-core


   /////////////////////////////////////
   //   analog front-end bias lines   //
   /////////////////////////////////////

`ifdef ATLAS_CHIP

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

  // Add back if needed, assumed to be handled elsewhere
   // global configuration
  //  input  wire AnaInjectionModeIn,  // uniform/alternating charge-injection mode selection
  //  output wire AnaInjectionModeOut,
  //  input  wire EnDigHitIn,          // digital injection = 1'b1, analog injection = 1'b0
  //  output wire EnDigHitOut,

   
  //  // **NOTE* CMS-only extra pin to enable SEU-counting, simple left unconnected for ATLAS (removed ifdef CMS_CHIP to have same digital I/O interface)
  //  input  wire EnSeuCountIn,
  //  output wire EnSeuCountOut,

  //  // per-pixel default/external configuration
  //  input  wire        PixelConfDefaultIn,      // global MUX control to switch between hard-wired default pixel configuration or Pixel Configuration Register (PCR) data
  //  output wire        PixelConfDefaultOut,
  //  input  wire [11:0] PixelConfAddrIn,         // full address of a pixel to be configured i.e. 12-bit = 6-bit core-row + 4-bit pixel-region + 2-bit pixel address per-region
  //  output wire [11:0] PixelConfAddrOut,
  //  input  wire        PixelConfWr5bitIn,       // write configuration = 1'b1, read configuration = 1'b0 => write configuration for trimming DAC only to speedup S-curves
  //  input  wire        PixelConfWr3bitIn,       // write configuration = 1'b1, read configuration = 1'b0 => write configuration for remaining bits
  //  output wire        PixelConfWr5bitOut,
  //  output wire        PixelConfWr3bitOut,
  //  input  wire  [7:0] PixelConfDataWrIn,       // 8-bit configuration data
  //  output wire  [7:0] PixelConfDataWrOut,
  //  input  wire  [7:0] PixelConfDataRdIn,       // 8-bit readback configuration data **WARN: actual values, not latched values !
  //  output wire  [7:0] PixelConfDataRdOut,

// NEED TO FIGURE OUT WHAT WE WANNA DO WITH CONFIG STUFF
   /////////////////////////////
   //   calibration section   //
   /////////////////////////////

   // calibration signals
   input  wire CalEdgeIn,      // global signal for first analog injection (needs fine timing), also used for "digital injection"
   output wire CalEdgeOut,

   input  wire CalAuxIn,       // global signal for (optional) second analog injection (not critical timing)
   output wire CalAuxOut,


   /////////////////////////
   //   readout section   //
   /////////////////////////

   // trigger-independent output data ("prompt" datapath)
  //  input  wire [3:0] HitOrIn,           // cluster-based hit-ORs //  look at these TODO dont need 
  //  output wire [3:0] HitOrOut,

   // BX timestamp/trigger section
   input  wire [`LATENCY_COUNTER_BITS-1:0] LatCntIn,      // BX timestamp
   output wire [`LATENCY_COUNTER_BITS-1:0] LatCntOut,
   input  wire [`LATENCY_COUNTER_BITS-1:0] LatCntReqIn,   // BX timestamp shifted back in time by trigger latency
   output wire [`LATENCY_COUNTER_BITS-1:0] LatCntReqOut,

   input  wire TrigIn,
   output wire TrigOut,
   input  wire TrigClearIn,
   output wire TrigClearOut,

   input  wire [`TRIG_ID_BITS-1:0] TrigIdIn,      // unique ID number associated to L1 trigger
   output wire [`TRIG_ID_BITS-1:0] TrigIdOut,

   // pixels <=> periphery handshaking
  // pin Tok in 0, ReadIn 1 for now... always ready to read
   input  wire TokIn,                  // OR-based token to inform the chip-periphery that there is something to read
   output wire TokOut,
  //  input  wire ReadIn,                 // read request issued from chip-periphery
  //  output wire ReadOut,

   // triggered output data
    output wire [`REGION_DATA_BITS-1:0] RegionDataOut,
    output logic [`TRIG_ID_BITS-1:0] RegionTrigOut
   // tie-down
  //  output wire OutLo   // connected to a tie-down cell, used to initialize hit-ORs, pixel-configuration readback data, token and readout data in the top-most core

   ) ;

  // TEMP TEMP TEMP TEMP TEMP TEMP
   logic ReadIn, Readout;
   assign ReadIn = 1'b1;

  //  `ifndef DIGITAL_CORE_ABSTRACT


   ////////////////////////////////////////////
   //   static core-row address generation   //
   ////////////////////////////////////////////

   // **NOTE: Addresses are actually DECREMENTED from bottom to top, CoreRowAddrIn[5:0]
   //         goes from (`CORE_ROWS-1) to 0 from bottom to top. The initialization to
   //         (`CORE_ROWS-1) occurs inside the PixelArrayReadout module

   //assign CoreRowAddrOut[5:0] = CoreRowAddrIn[5:0] - 6'b1 ;

  // TODO: TEMP FIX FIX FIX 
  assign RegionDataOut = data_last_region;

  localparam logic ANA_INJECTION_MODE = 1'b0;
  localparam logic EN_DIG_HIT         = 1'b0;



    //  input  wire AnaInjectionModeIn,  // uniform/alternating charge-injection mode selection
  //  output wire AnaInjectionModeOut,
  //  input  wire EnDigHitIn,          // digital injection = 1'b1, analog injection = 1'b0
  localparam logic EnDigHitIn = 1'b1;
  //  output wire EnDigHitOut,

   //
   // **NEW
   //
   // Put the combinational circuit generating the address into a dedicated standalone module to create harder placement
   // constraits in place-and-route (flattening disabled on this module during synthesis, no degradation seen for QoR).
   // This module is not flattened by usig set_attribute ungroup false [find / -subdesign AddressGen] in synthesis setup.
   // Some small routing optimizations can be implemented with this, e.g. CoreRowAddrOut[0] = CoreRowAddrOut[1], just a buffer !
   //
   AddressGen AddressGen ( .AddrIn( CoreRowAddrIn[5:0] ), .AddrOut( CoreRowAddrOut[5:0] )) ;


   //////////////////////////////////////////////////////////////////////
   //   core-dependent 4-bit programmable delay for skew adjustments   //
   //////////////////////////////////////////////////////////////////////

   // timing-critical clock and injection signals are internally delayed in each core to
   // guarantee <  2ns skew along a full column. The delay value is automatically selectd
   // according to the static address of the core, with the largest (smallest) delay used
   // in the top (bottom) of the matrix.

   wire del_clk ;       // after skew compensation
   wire del_caledge ;   // after skew compensation

   // **TEST
   wire del_clk_int ;   // synopsys keep_signal_name "del_clk_int"


   // use the 4 MSBs out of the core-row address
   wire [3:0] del_select ;
   assign del_select[3:0] = CoreRowAddrIn[5:2];


   // 40 MHz clock skew compensation
   ProgrammableDelayBuff24    programmable_delay_clk (

      .InToBuf    ( ClkIn           ),
      .Select     ( del_select[3:0] ),
      .OutDelayed ( del_clk_int     ),
      .OutBuf     ( ClkOut          )
 
  ) ;

   // CalEdge skew compensation
   ProgrammableDelayBuff16   programmable_delay_caledge (

      .InToBuf    (       CalEdgeIn ),
      .Select     ( del_select[3:0] ), 
      .OutDelayed (     del_caledge ),
      .OutBuf     (      CalEdgeOut )

   ) ;



   // **TEST: pre-place the first clock buffer at the middle of the core

   // synopsys dc_script_begin
   // set_dont_touch clk_root_rtlbuf
   // synopsys dc_script_end

   CKBD8  clk_root_rtlbuf ( .I(del_clk_int), .Z(del_clk) ) ;



   ///////////////////////////////////////////////////////////////////////////////
   //   core-to-core signals propagation with no buffering/extra logic in RTL   //
   ///////////////////////////////////////////////////////////////////////////////


   wire local_read;
   SigFork read_fork (.I(ReadIn), .L(local_read), .O(ReadOut)) ;
     
   wire local_reset_b;
   SigFork reset_fork (.I(ResetIn_b), .L(local_reset_b), .O(ResetOut_b)) ;
    
   //assign CalEdgeOut = CalEdgeIn ;   // **CHANGED: now use CKBD16 hard-coded in ProgrammableDelayBuff16 module 
   //assign CalAuxOut = CalAuxIn ;     // **CHANGED: now use CKBD6 hard-coded in RTL

    
   //assign PixelConfAddrOut[11:0] = PixelConfAddrIn[11:0] ;
   //assign PixelConfWrOut = PixelConfWrIn ;
   //assign PixelConfDataWrOut[7:0] = PixelConfDataWrIn[7:0] ;

  //  wire [11:0] local_pixel_conf_addr_in;
  //  generate
  //    genvar addr_inx ;
  //     for(addr_inx = 0; addr_inx < 12; addr_inx++) begin : addr_conf_fork

  //         SigForkConf pixel_conf_addr_fork (.I(PixelConfAddrIn[addr_inx]), .L(local_pixel_conf_addr_in[addr_inx]), .O(PixelConfAddrOut[addr_inx])) ;
  //     end
  //  endgenerate 
   
   
  //  wire [7:0] pixel_conf_data_wr_buf ;   
  //  generate
  //    genvar data_wr_inx ;
  //     for(data_wr_inx = 0; data_wr_inx < 8; data_wr_inx++) begin : data_wr_conf_fork

  //         SigForkConf pixel_conf_data_fork (.I(PixelConfDataWrIn[data_wr_inx]), .L(pixel_conf_data_wr_buf[data_wr_inx]), .O(PixelConfDataWrOut[data_wr_inx])) ;
  //     end
  //  endgenerate 
   
  //  wire local_pixel_conf_wr_5bit ;
  //  wire local_pixel_conf_wr_3bit ;

  //  SigForkConf pixel_conf_wr_5bit_fork (.I(PixelConfWr5bitIn), .L(local_pixel_conf_wr_5bit), .O(PixelConfWr5bitOut)) ;
  //  SigForkConf pixel_conf_wr_3bit_fork (.I(PixelConfWr3bitIn), .L(local_pixel_conf_wr_3bit), .O(PixelConfWr3bitOut)) ;


   //assign EnDigHitOut = EnDigHitIn ;
   //assign AnaInjectionModeOut = AnaInjectionModeIn ;
   //assign PixelConfDefaultOut = PixelConfDefaultIn ;



   // **PATCH: remove assigns already in RTL to fix LVS


   //
   // CalAux buffer => pre-placed at the middle of the 8x8 core
   //
   CKBD8 CalAux_rtlbuf ( .I(CalAuxIn ), .Z(CalAuxOut )) ;            // **IMPORTANT: then changed into CKBD12 with ecoChangeCell in place-and-route to fix top-leve DRVs !

   // synopsys dc_script_begin
   // set_dont_touch CalAux_rtlbuf
   // synopsys dc_script_end



   //
   // basically **STATIC** global configuration bits => let the tool to optimize the distribution to avoid DRVs on these signals
   //

   //assign EnDigHitOut         = EnDigHitIn ;
   //assign AnaInjectionModeOut = AnaInjectionModeIn ;
   //assign PixelConfDefaultOut = PixelConfDefaultIn ;


   // **REMOVE** assigns already in RTL but then let the tool to the optimize the proper buffer strength => NO dont_touch statements on these buffers, only on input signals

   BUFFD2LVT          EnDigHit_rtlbuf ( .I(         EnDigHitIn ), .Z(         EnDigHitOut )) ;
   BUFFD2LVT  AnaInjectionMode_rtlbuf ( .I( AnaInjectionModeIn ), .Z( AnaInjectionModeOut )) ;
   CKBD1      PixelConfDefault_rtlbuf ( .I( PixelConfDefaultIn ), .Z( PixelConfDefaultOut )) ;




   //////////////////////////////////////////////////////////////////
   //   core-to-core signals propagation with hard-coded "forks"   //
   //////////////////////////////////////////////////////////////////

   //
   // **NOTE: hard-coded "forks" are pre-placed in RTL on the following selected timing-sensitive signals:
   //
   //   - Trig
   //   - TrigClear
   //   - LatCnt[`LATENCY_COUNTER_BITS-1:0], LatCntReq[`LATENCY_COUNTER_BITS-1:0]
   //


   // TrigIn => TrigOut
   // TrigClearIn => TrigClearOut

   wire local_trig ;
   wire local_trig_clear ;

   SigFork   trig_fork       (.I(TrigIn),      .L(local_trig),       .O(TrigOut)) ;
   SigFork   trig_clear_fork (.I(TrigClearIn), .L(local_trig_clear), .O(TrigClearOut)) ;


   // TrigIdIn[`TRIG_ID_BITS-1:0] => TrigIdOut[`TRIG_ID_BITS-1:0]
   assign TrigIdOut = TrigIdIn; 
   // prev was some buffering here with sigfork in RD53C ******

   wire [`TRIG_ID_BITS-1:0] local_trig_id ;
   wire [`TRIG_ID_BITS-1:0] local_trig_id_req ;




   // LatCntIn[`LATENCY_COUNTER_BITS-1:0] => LatCntOut[`LATENCY_COUNTER_BITS-1:0]
   // LatCntReqIn[`LATENCY_COUNTER_BITS-1:0] => LatCntReqOut[`LATENCY_COUNTER_BITS-1:0]

   wire [`LATENCY_COUNTER_BITS-1:0] local_lat_cnt ;
   wire [`LATENCY_COUNTER_BITS-1:0] local_lat_cnt_req ;

   generate

      genvar lat_cnt_index ;

      for(lat_cnt_index = 0; lat_cnt_index < `LATENCY_COUNTER_BITS; lat_cnt_index++) begin : lat_cnt_fork

         SigFork   lat_cnt_id_fork  (.I(LatCntIn[lat_cnt_index]), .L(local_lat_cnt[lat_cnt_index]), .O(LatCntOut[lat_cnt_index])) ;
         SigFork   lat_cnt_req_fork (.I(LatCntReqIn[lat_cnt_index]), .L(local_lat_cnt_req[lat_cnt_index]), .O(LatCntReqOut[lat_cnt_index])) ;
      end
   endgenerate 


// GRAMMY NOTE: ALL OF THE ABOVE HANDLES delay/skew stuff + core-to-core signals

//
//   .oooooo.   oooo                                                     ooooo                 o8o                         .    o8o                        
//  d8P'  `Y8b  `888                                                     `888'                 `"'                       .o8    `"'                        
// 888           888 .oo.    .oooo.   oooo d8b  .oooooooo  .ooooo.        888  ooo. .oo.      oooo  .ooooo.   .ooooo.  .o888oo oooo   .ooooo.  ooo. .oo.   
// 888           888P"Y88b  `P  )88b  `888""8P 888' `88b  d88' `88b       888  `888P"Y88b     `888 d88' `88b d88' `"Y8   888   `888  d88' `88b `888P"Y88b  
// 888           888   888   .oP"888   888     888   888  888ooo888       888   888   888      888 888ooo888 888         888    888  888   888  888   888  
// `88b    ooo   888   888  d8(  888   888     `88bod8P'  888    .o       888   888   888      888 888    .o 888   .o8   888 .  888  888   888  888   888  
//  `Y8bood8P'  o888o o888o `Y888""8o d888b    `8oooooo.  `Y8bod8P'      o888o o888o o888o     888 `Y8bod8P' `Y8bod8P'   "888" o888o `Y8bod8P' o888o o888o 
//                                             d"     YD                                       888                                                         
//                                             "Y88888P'                                   .o. 88P                                                         
//                                                                                         `Y888P                                                          
//
   ////////////////////////////////////////
   //   charge-injection control logic   //
   ////////////////////////////////////////

   //
   // "uniform" mode: S0 and S1 are the same for all injected pixels
   //
   // S0_uniform = CalEdge OR CalAux
   // S1_uniform = (NOT CalEdge) AND CalAux
   //                           ______________
   //   _______________________/              \___________  CalEdge
   //                                   ____________
   //   _______________________________/            \_____  CalAux
   //                           ____________________
   //   _______________________/                    \_____  S0
   //                                         ______
   //   _____________________________________/      \_____  S1
   //
   //
   //
   // "alternating" mode: S0 and S1 are routed as in uniform mode for "even" pixels
   //                     but swapped for "odd" pixels, allowing to inject different
   //                     size pulses into adjacent pixels
   // 
   // S0_even = S0_uniform    S0_odd = S1_uniform
   // S1_even = S1_uniform    S1_odd = S0_uniform
   //
   // where "even (odd) pixel"  => pixel row + pixel col inside the "quad" = even (odd) number
   //

   //
   // **NOTE: The chosen 2-dim (row,col) pixel numbering inside each quad is :
   //
   //   AFE_UL | AFE_UR          pixel[0][0] | pixel[0][1]           even | odd           {S1_even,S0_even} |  {S1_odd,S0_odd}
   //  --------|--------   =>   -------------|--------------   =>   ------|-----   =>   --------------------|-------------------
   //   AFE_LL | AFE_LR          pixel[1][0] | pixel[1][1]           odd  | even           {S1_odd,S0_odd}  | {S1_even,S0_even}
   //
   //
   // To enable edge-pixels in RTL instead, the "flat" pixel numbering is simply the binary value of {pix_row,pix_col} :
   //
   //   pixel[0][0] | pixel[0][1]          pixel_0 | pixel_1
   //  -------------|-------------   =>   ---------|---------
   //   pixel[1][0] | pixel[1][1]          pixel_2 | pixel_3
   //


   wire S0_even, S1_even ;
   wire S0_odd, S1_odd ;

   // "even" pixels are always in "uniform" mode
   assign S0_even = del_caledge | CalAuxIn ;             // **NOTE: precise timing is not required on CalAux signal (no need for skew-adjustement)
   assign S1_even = ! del_caledge & CalAuxIn ;

   //assign S0_even = del_caledge | CalAuxOut ;          **DO NOT USE** => uge impact on routing convergence despite logically equivalent
   //assign S1_even = ! del_caledge & CalAuxOut ;

   // "odd" pixels are either in "uniform" mode or in "alternating" mode according
   // to the value of the global AnaInjectionMode configuration bit, if in "alternating"
   // mode just swap S0 and S1
   assign S0_odd = (AnaInjectionModeOut  == 1'b0) ? S0_even : S1_even ;
   assign S1_odd = (AnaInjectionModeOut  == 1'b0) ? S1_even : S0_even ;



   //////////////////////////////////////////////////////////////
   //   analog quads and front-end controllers instantiation   //
   //////////////////////////////////////////////////////////////

   //
   // **NOTE: The numbering scheme is from top to bottom and from left to right to match sensor pixel numbering in MC data:
   //
   //
   //   AnalogQuad[0][0]  AnalogQuad[0][1] ... AnalogQuad[0][3]
   //         ...               ...                  ...
   //         ...               ...                  ...
   //   AnalogQuad[3][0]        ...            AnalogQuad[3][3]
   //





   //////////////////////////////////////////
   //   distribute/get per-pixel signals   //
   //////////////////////////////////////////

   // pix ROW pix COL

   wire [7:0][7:0] ana_hit_int = AnaHit[63:0] ;

   wire [7:0][7:0] pix_power_down ;            // from pixel configuration registers, but used also in PixelRegion logic
   wire [7:0][7:0] pix_hit_analog ;            // true analog front-end DISC outputs
   wire [7:0][7:0] pix_hit ;                   // final digital hits from pixels (after masking/optional digital-injection)
   wire [7:0][7:0] pix_hit_or ;                // final hit-OR from pixels (after masking/hit-OR enable)

   wire [7:0][7:0] pix_s0 ;                    // final S0 and S1 to analog front-ends (after proper swapping between edge/odd)
   wire [7:0][7:0] pix_s1 ;

   wire [7:0][7:0] pixel_conf_wr_5bit ;        // GLOBAL pixel configuration write/read enable PixelConfWr5bit distributed to pixels => write configuration for trimming DAC only to speedup S-curves
   wire [7:0][7:0] pixel_conf_wr_3bit ;        // GLOBAL pixel configuration write/read enable PixelConfWr3bit distributed to pixels => write configuration for remaining bits

   wire [7:0][7:0] pixel_conf_select ;         // ... IF external address matches the local pixel address !

   wire [7:0][7:0][7:0] pixel_conf_values ;    // effective per-pixel configuration for readback (default configuration or latched values)

   wire [7:0][7:0][3:0] pix_dth1 ;             // **ATLAS: DIFF front-end trimming DAC configuration bits
   wire [7:0][7:0][3:0] pix_dth2 ;



   wire [0:64][7:0] pixel_conf_data_rd ;       // all readback configuration data from pixels and from/to previous/next core

   assign pixel_conf_data_rd[0][7:0] = 8'b0; //PixelConfDataRdIn[7:0] ;     // from previous core
   //assign PixelConfDataRdOut[7:0] = pixel_conf_data_rd[64][7:0] | PixelConfDataRdIn[7:0] ;   // to next core

   assign pix_hit_analog = AnaHit; // route TB hits through FeControl masking before pix_hit
   // THIS IS FOR SIM!!!!! TODO THIS SHOULD NOT BE FOR FINAL 


   //wire [7:0] pixel_conf_data_rd_or;
   //assign pixel_conf_data_rd_or = PixelConfDataRdIn[7:0] | pixel_conf_data_rd[64];
  //  generate
  //     genvar pix_conf_data_rd ;

  //     for(pix_conf_data_rd = 0; pix_conf_data_rd < 8; pix_conf_data_rd++) begin : pix_conf_data_rd_fork
   
  //        //OR2D4LVT pix_conf_data_rd_inst (.A1(PixelConfDataRdIn[pix_conf_data_rd]), .A2(pixel_conf_data_rd[64][pix_conf_data_rd]), .Z(PixelConfDataRdOut[pix_conf_data_rd]));
  //        OR2D8LVT pix_conf_data_rd_or (.A1(PixelConfDataRdIn[pix_conf_data_rd]), .A2(pixel_conf_data_rd[64][pix_conf_data_rd]), .Z(PixelConfDataRdOut[pix_conf_data_rd]));      // fix maxTran DRVs
  //     end
  //  endgenerate

   // synopsys dc_script_begin
   // set_dont_touch *pix_conf_data_rd_or*
   // synopsys dc_script_end

   generate

      genvar quad_row ;   // 0,1,2,3 from top to bottom
      genvar quad_col ;   // 0,1,2,3 from left to right

      genvar pix_row ;    // 0,1 from top to bottom 
      genvar pix_col ;    // 0,1 from left to right

      // vertical loop over "quad" rows, from top to bottom
      for(quad_row = 0; quad_row < 4; quad_row++) begin : AnalogQuadRow

         // horizontal loop over "quad" columns, from left to right
         for(quad_col = 0; quad_col < 4; quad_col++) begin : AnalogQuadCol

            // loop over each pixel inside the quad
            for(pix_row = 0; pix_row < 2; pix_row++) begin : PixelRow 

               for(pix_col = 0; pix_col < 2; pix_col++) begin : PixelCol

                  //
                  // pixel selection to WRITE configuration data
                  //
                  // assign pixel_conf_select[quad_row*2+pix_row][quad_col*2+pix_col] = local_pixel_conf_addr_in[11:6] == CoreRowAddrIn[5:0] &  // external core address matches the local static address, AND...
                  //                                                                    local_pixel_conf_addr_in[5:4]  == quad_row &            // quad row in the core
                  //                                                                    local_pixel_conf_addr_in[3]    == pix_row &             // pixel row in the selected quad (0=top/1=bottom)
                  //                                                                    local_pixel_conf_addr_in[2]    == (quad_col > 1) &      // left 1x4 or right 1x4
                  //                                                                    local_pixel_conf_addr_in[1]    == (quad_col%2) &        // 1x2 left or right
                  //                                                                    local_pixel_conf_addr_in[0]    == pix_col ;             // pixel_col in 1x2

                  //                                                                    // **OBSOLETE: broadcast mode no more requested
                  //                                                                    //local_pixel_conf_addr_in[0]    == pix_col |             // pixel_col in 1x2
                  //                                                                    //local_pixel_conf_addr_in[11:0] == {12{1'b1}} ;          // OR we are in broadcast mode otherwise

                  // // final per-pixel write-enable
                  // assign pixel_conf_wr_5bit[quad_row*2+pix_row][quad_col*2+pix_col] = local_pixel_conf_wr_5bit & pixel_conf_select[quad_row*2+pix_row][quad_col*2+pix_col] ;
                  // assign pixel_conf_wr_3bit[quad_row*2+pix_row][quad_col*2+pix_col] = local_pixel_conf_wr_3bit & pixel_conf_select[quad_row*2+pix_row][quad_col*2+pix_col] ;


                  // //
                  // // pixel section to READBACK configuration data
                  // //
                  // localparam int pix_idx = (quad_row*2+pix_row)*8 + (quad_col*2+pix_col) + 1 ; // pix_idx = 0 is previous core

                  // assign pixel_conf_data_rd[pix_idx][7:0] = pixel_conf_data_rd[pix_idx-1][7:0] |
                  //                                           (pixel_conf_values[quad_row*2+pix_row][quad_col*2+pix_col][7:0] &
                  //                                      {8{pixel_conf_select[quad_row*2+pix_row][quad_col*2+pix_col]}}) ;

               end   // for pix_col
            end   // for pix_row
//
//       .o.                             oooo                              .oooooo.                                  .o8  
//      .888.                            `888                             d8P'  `Y8b                                "888  
//     .8"888.     ooo. .oo.    .oooo.    888   .ooooo.   .oooooooo      888      888    oooo  oooo   .oooo.    .oooo888  
//    .8' `888.    `888P"Y88b  `P  )88b   888  d88' `88b 888' `88b       888      888    `888  `888  `P  )88b  d88' `888  
//   .88ooo8888.    888   888   .oP"888   888  888   888 888   888       888      888     888   888   .oP"888  888   888  
//  .8'     `888.   888   888  d8(  888   888  888   888 `88bod8P'       `88b    d88b     888   888  d8(  888  888   888  
// o88o     o8888o o888o o888o `Y888""8o o888o `Y8bod8P' `8oooooo.        `Y8bood8P'Ybd'  `V88V"V8P' `Y888""8o `Y8bod88P" 
//                                                       d"     YD                                                        
//                                                       "Y88888P'                                                        
//                                                                                                                       
            //////////////////////////////////////////////////
            //   analog quads instantiation (hard macros)   //
            //////////////////////////////////////////////////

            //
            // **NOTE: "quads" instances of "edge" cores are properly remastered at the end of PNR flow
            //         by replacing RD53B_ANALOG_QUAD_DIFF with RD53B_ANALOG_QUAD_TOP_DIFF in the top
            //         quad row. Edge pixels are NOT SUPPORTED for RTL simulations !
            //

// GRAMMY NOTES: COMMENTING OUT, FROM MY UNDERSTANDING APPLIES BIAS FOR ANALOG BLOCKS WHICH ISNT NEEDED 
// FOR COCOTB SIM TODO: return to this when doing real chip stuff

// `ifdef ATLAS_CHIP

//             RD53B_ANALOG_QUAD_DIFF   ANALOG_QUAD (

//                // left-pixels bias lines (even index 0,2 ... 6) and right-pixels odd index 1,3 ... 7)
//                .VBP_PREAMP_A_L    (                 VBP_PREAMP_A[quad_col*2+0] ),
//                .VBP_PREAMP_B_L    (                 VBP_PREAMP_B[quad_col*2+0] ),
//                .VBN_COMP_L        (                     VBN_COMP[quad_col*2+0] ),
//                .VBN_PRECOMP_L     (                  VBN_PRECOMP[quad_col*2+0] ),
//                .VTH1_L            (                         VTH1[quad_col*2+0] ),
//                .VTH2_L            (                         VTH2[quad_col*2+0] ),
//                .VBN_LCC_L         (                      VBN_LCC[quad_col*2+0] ),
//                .VBP_VFF_L         (                      VBP_VFF[quad_col*2+0] ),
//                .VCTRL_CF0_L       (                    VCTRL_CF0[quad_col*2+0] ),
//                .VCTRL_LCC_L       (                    VCTRL_LCC[quad_col*2+0] ),

//                // right-pixels bias lines (odd index 1,3 ... 7)
//                .VBP_PREAMP_A_R    (                 VBP_PREAMP_A[quad_col*2+1] ),
//                .VBP_PREAMP_B_R    (                 VBP_PREAMP_B[quad_col*2+1] ),
//                .VBN_COMP_R        (                     VBN_COMP[quad_col*2+1] ),
//                .VBN_PRECOMP_R     (                  VBN_PRECOMP[quad_col*2+1] ),
//                .VTH1_R            (                         VTH1[quad_col*2+1] ),
//                .VTH2_R            (                         VTH2[quad_col*2+1] ),
//                .VBN_LCC_R         (                      VBN_LCC[quad_col*2+1] ),
//                .VBP_VFF_R         (                      VBP_VFF[quad_col*2+1] ),
//                .VCTRL_CF0_R       (                    VCTRL_CF0[quad_col*2+1] ),
//                .VCTRL_LCC_R       (                    VCTRL_LCC[quad_col*2+1] ),

//                // trimming DAC configuration bits
//                .DTH1_UL            ( pix_dth1[quad_row*2+0][quad_col*2+0][3:0] ),
//                .DTH2_UL            ( pix_dth2[quad_row*2+0][quad_col*2+0][3:0] ),
//                .DTH1_UR            ( pix_dth1[quad_row*2+0][quad_col*2+1][3:0] ),
//                .DTH2_UR            ( pix_dth2[quad_row*2+0][quad_col*2+1][3:0] ),
//                .DTH1_LL            ( pix_dth1[quad_row*2+1][quad_col*2+0][3:0] ),
//                .DTH2_LL            ( pix_dth2[quad_row*2+1][quad_col*2+0][3:0] ),
//                .DTH1_LR            ( pix_dth1[quad_row*2+1][quad_col*2+1][3:0] ),
//                .DTH2_LR            ( pix_dth2[quad_row*2+1][quad_col*2+1][3:0] ),

// `elsif CMS_CHIP

//             RD53B_ANALOG_QUAD_LIN   ANALOG_QUAD (

//                // left-pixels bias lines (even index 0,2 ... 6)
//                .IPA_A_L            (                       IPA_A[quad_col*2+0] ),
//                .IPA_B_L            (                       IPA_B[quad_col*2+0] ),
//                .ICOMP_L            (                       ICOMP[quad_col*2+0] ),
//                .ICOMP_STAR_L       (                  ICOMP_STAR[quad_col*2+0] ),
//                .VTH_L              (                         VTH[quad_col*2+0] ),
//                .VREF_KRUM_L        (                   VREF_KRUM[quad_col*2+0] ),
//                .IHU_KRUM_L         (                    IHU_KRUM[quad_col*2+0] ),
//                .IHD_KRUM_L         (                    IHD_KRUM[quad_col*2+0] ),
//                .IFC_L              (                         IFC[quad_col*2+0] ),
//                .ILDAC_MIR_L        (                   ILDAC_MIR[quad_col*2+0] ),

//                // right-pixels bias lines (odd index 1,3 ... 7)
//                .IPA_A_R            (                       IPA_A[quad_col*2+1] ),
//                .IPA_B_R            (                       IPA_B[quad_col*2+1] ),
//                .ICOMP_R            (                       ICOMP[quad_col*2+1] ),
//                .ICOMP_STAR_R       (                  ICOMP_STAR[quad_col*2+1] ),
//                .VTH_R              (                         VTH[quad_col*2+1] ),
//                .VREF_KRUM_R        (                   VREF_KRUM[quad_col*2+1] ),
//                .IHU_KRUM_R         (                    IHU_KRUM[quad_col*2+1] ),
//                .IHD_KRUM_R         (                    IHD_KRUM[quad_col*2+1] ),
//                .IFC_R              (                         IFC[quad_col*2+1] ),
//                .ILDAC_MIR_R        (                   ILDAC_MIR[quad_col*2+1] ),

//                // trimming DAC configuration bits
//                .TH_DAC_UL          ( pix_tdac[quad_row*2+0][quad_col*2+0][4:0] ),
//                .TH_DAC_UR          ( pix_tdac[quad_row*2+0][quad_col*2+1][4:0] ),
//                .TH_DAC_LL          ( pix_tdac[quad_row*2+1][quad_col*2+0][4:0] ),
//                .TH_DAC_LR          ( pix_tdac[quad_row*2+1][quad_col*2+1][4:0] ),

// `endif

//                /////////////////////////
//                //   COMMON AFE pins   //
//                /////////////////////////

//                // calibration circuit DC levels and charge-injection signals
//                .VCAL_MI_L         (                      VCAL_MI[quad_col*2+0] ),
//                .VCAL_HI_L         (                      VCAL_HI[quad_col*2+0] ),
//                .VCAL_MI_R         (                      VCAL_MI[quad_col*2+1] ),
//                .VCAL_HI_R         (                      VCAL_HI[quad_col*2+1] ),

//                .S0_UL             (         pix_s0[quad_row*2+0][quad_col*2+0] ),
//                .S0_UR             (         pix_s0[quad_row*2+0][quad_col*2+1] ),
//                .S0_LL             (         pix_s0[quad_row*2+1][quad_col*2+0] ),
//                .S0_LR             (         pix_s0[quad_row*2+1][quad_col*2+1] ),

//                .S1_UL             (         pix_s1[quad_row*2+0][quad_col*2+0] ),
//                .S1_UR             (         pix_s1[quad_row*2+0][quad_col*2+1] ),
//                .S1_LL             (         pix_s1[quad_row*2+1][quad_col*2+0] ),
//                .S1_LR             (         pix_s1[quad_row*2+1][quad_col*2+1] ),

//                // bump pads
//                .BUMP_UL           (    ana_hit_int[quad_row*2+0][quad_col*2+0] ),
//                .BUMP_UR           (    ana_hit_int[quad_row*2+0][quad_col*2+1] ),
//                .BUMP_LL           (    ana_hit_int[quad_row*2+1][quad_col*2+0] ),
//                .BUMP_LR           (    ana_hit_int[quad_row*2+1][quad_col*2+1] ),

//                // DISC outputs
//                .DISC_OUT_UL       ( pix_hit_analog[quad_row*2+0][quad_col*2+0] ),
//                .DISC_OUT_UR       ( pix_hit_analog[quad_row*2+0][quad_col*2+1] ),
//                .DISC_OUT_LL       ( pix_hit_analog[quad_row*2+1][quad_col*2+0] ),
//                .DISC_OUT_LR       ( pix_hit_analog[quad_row*2+1][quad_col*2+1] )

//             ) ;

//
// oooooooooooo oooooooooooo        .oooooo.                             .                      oooo  
// `888'     `8 `888'     `8       d8P'  `Y8b                          .o8                      `888  
//  888          888              888           .ooooo.  ooo. .oo.   .o888oo oooo d8b  .ooooo.   888  
//  888oooo8     888oooo8         888          d88' `88b `888P"Y88b    888   `888""8P d88' `88b  888  
//  888    "     888    "         888          888   888  888   888    888    888     888   888  888  
//  888          888       o      `88b    ooo  888   888  888   888    888 .  888     888   888  888  
// o888o        o888ooooood8       `Y8bood8P'  `Y8bod8P' o888o o888o   "888" d888b    `Y8bod8P' o888o 
//                                                                                                   
            ///////////////////////////////////////////////////
            //   front-end controllers instantiation (RTL)   //
            ///////////////////////////////////////////////////

            FeControl   FeControl_LL (

            //    // configuration section
            //    .PixelConfDefault (                            PixelConfDefaultOut ),
            //    .PixelConfWr5bit  ( pixel_conf_wr_5bit[quad_row*2+1][quad_col*2+0] ),
            //    .PixelConfWr3bit  ( pixel_conf_wr_3bit[quad_row*2+1][quad_col*2+0] ),
            //    .PixelConfDataWr  (                         pixel_conf_data_wr_buf ),
            //    .PixelConfDataRd  (  pixel_conf_values[quad_row*2+1][quad_col*2+0] ),
            //    .PwrDwn           (     pix_power_down[quad_row*2+1][quad_col*2+0] ),
            // `ifdef CMS_CHIP
            //    .EnSeuCount       (                                  EnSeuCountOut ),
            // `endif

               // calibration section
               .DefaultCalEn     (                  (quad_row == 0) ? 1'b1 : 1'b0 ),   // **NOTE: charge-injection is enabled by default in the two pixel-rows at the top of the core
               .EvenOddS0        (                                         S0_odd ),   // odd pixel
               .EvenOddS1        (                                         S1_odd ),
               .FeS0             (             pix_s0[quad_row*2+1][quad_col*2+0] ),
               .FeS1             (             pix_s1[quad_row*2+1][quad_col*2+0] ),

               // configuration bits to the trimming DAC
               .FeDTH1           (      pix_dth1[quad_row*2+1][quad_col*2+0][3:0] ),
               .FeDTH2           (      pix_dth2[quad_row*2+1][quad_col*2+0][3:0] ),

               // hit pulse from analog front-end
               .FeHit            (     pix_hit_analog[quad_row*2+1][quad_col*2+0] ),

               // **DEBUG: digital injection
               .EnDigHit         (                                    EnDigHitOut ),
               .CalEdge          (                                    del_caledge ),

               // hit information
               .HitOut           (            pix_hit[quad_row*2+1][quad_col*2+0] ),
               .HitOr            (         pix_hit_or[quad_row*2+1][quad_col*2+0] )

            ) ;

            FeControl   FeControl_LR (

               // configuration section
            //    .PixelConfDefault (                            PixelConfDefaultOut ),
            //    .PixelConfWr5bit  ( pixel_conf_wr_5bit[quad_row*2+1][quad_col*2+1] ),
            //    .PixelConfWr3bit  ( pixel_conf_wr_3bit[quad_row*2+1][quad_col*2+1] ),
            //    .PixelConfDataWr  (                         pixel_conf_data_wr_buf ),
            //    .PixelConfDataRd  (  pixel_conf_values[quad_row*2+1][quad_col*2+1] ),
            //    .PwrDwn           (     pix_power_down[quad_row*2+1][quad_col*2+1] ),
            // `ifdef CMS_CHIP
            //    .EnSeuCount       (                                  EnSeuCountOut ),
            // `endif
               // calibration section
               .DefaultCalEn     (                  (quad_row == 0) ? 1'b1 : 1'b0 ),
               .EvenOddS0        (                                        S0_even ),   // even pixel
               .EvenOddS1        (                                        S1_even ),
               .FeS0             (             pix_s0[quad_row*2+1][quad_col*2+1] ),
               .FeS1             (             pix_s1[quad_row*2+1][quad_col*2+1] ),

               // configuration bits to the trimming DAC
               .FeDTH1           (      pix_dth1[quad_row*2+1][quad_col*2+1][3:0] ),
               .FeDTH2           (      pix_dth2[quad_row*2+1][quad_col*2+1][3:0] ),

               // hit pulse from analog front-end
               .FeHit            (     pix_hit_analog[quad_row*2+1][quad_col*2+1] ),

               // **DEBUG: digital injection
               .EnDigHit         (                                    EnDigHitOut ),
               .CalEdge          (                                    del_caledge ),

                // hit information
               .HitOut           (            pix_hit[quad_row*2+1][quad_col*2+1] ),
               .HitOr            (         pix_hit_or[quad_row*2+1][quad_col*2+1] )

            ) ;

            FeControl   FeControl_UL (

            //    // configuration section
            //    .PixelConfDefault (                            PixelConfDefaultOut ),
            //    .PixelConfWr5bit  ( pixel_conf_wr_5bit[quad_row*2+0][quad_col*2+0] ),
            //    .PixelConfWr3bit  ( pixel_conf_wr_3bit[quad_row*2+0][quad_col*2+0] ),
            //    .PixelConfDataWr  (                         pixel_conf_data_wr_buf ),
            //    .PixelConfDataRd  (  pixel_conf_values[quad_row*2+0][quad_col*2+0] ),
            //    .PwrDwn           (     pix_power_down[quad_row*2+0][quad_col*2+0] ),
            // `ifdef CMS_CHIP
            //    .EnSeuCount       (                                  EnSeuCountOut ),
            // `endif
               // calibration section
               .DefaultCalEn     (                  (quad_row == 0) ? 1'b1 : 1'b0 ),
               .EvenOddS0        (                                        S0_even ),   // even pixel
               .EvenOddS1        (                                        S1_even ),
               .FeS0             (             pix_s0[quad_row*2+0][quad_col*2+0] ),
               .FeS1             (             pix_s1[quad_row*2+0][quad_col*2+0] ),

               // configuration bits to the trimming DAC
               .FeDTH1           (      pix_dth1[quad_row*2+0][quad_col*2+0][3:0] ),
               .FeDTH2           (      pix_dth2[quad_row*2+0][quad_col*2+0][3:0] ),

               // hit pulse from analog front-end
               .FeHit            (     pix_hit_analog[quad_row*2+0][quad_col*2+0] ),

               // **DEBUG: digital injection
               .EnDigHit         (                                    EnDigHitOut ),
               .CalEdge          (                                    del_caledge ),

               // hit information
               .HitOut           (            pix_hit[quad_row*2+0][quad_col*2+0] ),
               .HitOr            (         pix_hit_or[quad_row*2+0][quad_col*2+0] )

            ) ;

            FeControl   FeControl_UR (

               // configuration section
            //    .PixelConfDefault (                            PixelConfDefaultOut ),
            //    .PixelConfWr5bit  ( pixel_conf_wr_5bit[quad_row*2+0][quad_col*2+1] ),
            //    .PixelConfWr3bit  ( pixel_conf_wr_3bit[quad_row*2+0][quad_col*2+1] ),
            //    .PixelConfDataWr  (                    pixel_conf_data_wr_buf[7:0] ),
            //    .PixelConfDataRd  (  pixel_conf_values[quad_row*2+0][quad_col*2+1] ),
            //    .PwrDwn           (     pix_power_down[quad_row*2+0][quad_col*2+1] ),
            // `ifdef CMS_CHIP
            //    .EnSeuCount       (                                  EnSeuCountOut ),
            // `endif
               // calibration section
               .DefaultCalEn     (                  (quad_row == 0) ? 1'b1 : 1'b0 ),
               .EvenOddS0        (                                         S0_odd ),   // odd pixel
               .EvenOddS1        (                                         S1_odd ),
               .FeS0             (             pix_s0[quad_row*2+0][quad_col*2+1] ),
               .FeS1             (             pix_s1[quad_row*2+0][quad_col*2+1] ),

               // configuration bits to the trimming DAC
               .FeDTH1           (      pix_dth1[quad_row*2+0][quad_col*2+1][3:0] ),
               .FeDTH2           (      pix_dth2[quad_row*2+0][quad_col*2+1][3:0] ),

               // hit pulse from analog front-end
               .FeHit            (     pix_hit_analog[quad_row*2+0][quad_col*2+1] ),

               // **DEBUG: digital injection
               .EnDigHit         (                                    EnDigHitOut ),
               .CalEdge          (                                    del_caledge ),

               // hit information
               .HitOr            (         pix_hit_or[quad_row*2+0][quad_col*2+1] ),
               .HitOut           (            pix_hit[quad_row*2+0][quad_col*2+1] )

            ) ;

         end   // for quad_col
      end   // for quad_row
   endgenerate


  assign PwrDwn = '0;
  assign pix_power_down = '0;
   /////////////////////////////////////
   //   pixel-regions instantiation   //
   /////////////////////////////////////

   // Instantiate 16 pixel-regions grouping 1x4 pixels (r\phi x z) each one with Distributed Buffering Architecture (DBA).
   // Clusters in the barrel detector are elongated in z and are recorded more efficiently with 50um x 200um rectangular
   // regions rather than using square regions of 2x2 pixels as in FE-I4 chip.

   //
   // **REM: The numbering scheme adopted for 1x4 pixel regions inside the core is:
   //
   //   PixelRegion_0   PixelRegion_1
   //   PixelRegion_2   PixelRegion_3
   //   PixelRegion_4   PixelRegion_5
   //       ...             ...
   //       ...             ...
   //   PixelRegion_14  PixelRegion_15
   //

   //
   // token-based data readout
   //

   // token-chain between all 16 pixel-regions inside the core
   wire [16:0] tok_int ;   // **WARN: [16:0] this is just for region-to-region interconnections

   // initialize the chain at zero PixelRegion[0]/TokIn input port...
   assign tok_int[0] = 0 ;

   // ... then propagate last token from regions to next core below...
   //assign TokOut = TokIn | tok_int[16] ;

   wire tok_last_region = tok_int[16] ;

   //assign TokOut = TokIn | tok_last_region ;
   OR2D8LVT token_or_lvt (.A1(tok_last_region), .A2(TokIn), .Z(TokOut));

   // synopsys dc_script_begin
   // set_dont_touch token_or_lvt
   // synopsys dc_script_end
    
   // ... then enable readout for this core if at least one region asserted the token...
   wire this_core_read ;
   assign this_core_read = (TokIn == 1'b0 && TokOut == 1'b1) ? 1'b1 : 1'b0 ; 

   wire read_region ;
   assign read_region = this_core_read & local_read ;


   // token-map (indicates which regions have asserted a token)
   wire [15:0] tok_map ;
   assign tok_map[15:0] = tok_int[16:1] ;   // **WARN: [15:0], one for each region !

   wire [15:0] region_data_bus [15:0] ;     // `REGION_DATA_BITS (4-bit ToT x 4 pixels = 16-bit) from each region x 16 pixel regions per core

// ooooooooo.    o8o                        oooo  ooooooooo.                         o8o                             ooooo                              o8o            
// `888   `Y88.  `"'                        `888  `888   `Y88.                       `"'                             `888'                              `"'            
//  888   .d88' oooo  oooo    ooo  .ooooo.   888   888   .d88'  .ooooo.   .oooooooo oooo   .ooooo.  ooo. .oo.         888          .ooooo.   .oooooooo oooo   .ooooo.  
//  888ooo88P'  `888   `88b..8P'  d88' `88b  888   888ooo88P'  d88' `88b 888' `88b  `888  d88' `88b `888P"Y88b        888         d88' `88b 888' `88b  `888  d88' `"Y8 
//  888          888     Y888'    888ooo888  888   888`88b.    888ooo888 888   888   888  888   888  888   888        888         888   888 888   888   888  888       
//  888          888   .o8"'88b   888    .o  888   888  `88b.  888    .o `88bod8P'   888  888   888  888   888        888       o 888   888 `88bod8P'   888  888   .o8 
// o888o        o888o o88'   888o `Y8bod8P' o888o o888o  o888o `Y8bod8P' `8oooooo.  o888o `Y8bod8P' o888o o888o      o888ooooood8 `Y8bod8P' `8oooooo.  o888o `Y8bod8P' 
//                                                                       d"     YD                                                          d"     YD                  
//                                                                       "Y88888P'                                                          "Y88888P'                  
//                                                                                                                                                                    
   //
   // replicate pixel regions
   //

   wire [`TRIG_ID_BITS-1:0] region_trig_bus [15:0];


   generate

      genvar r ;  // r = 0,1, .. 15
      // maybe move to 1 for temp purposes, need to be able to do testing
      for(r = 0; r < 16; r++) begin: PixelRegion

         PixelRegionLogic   PixelRegionLogic (

            // clock and reset
            .Clk              (                                      del_clk ),
            .Reset_b          (                                local_reset_b ),

            // from pixels
            .Hit              (            pix_hit [r/2][(r%2 + 1)*4 -1 -:4] ),   // **NOTE: input [3:0] Hit
            .PwrDwn           (    pix_power_down[r/2][(r%2 + 1)*4 -1 -: 4 ] ),   // **NOTE: input [3:0] PwrDwn

            // timestamp/trigger section
            .LatCnt           (     local_lat_cnt[`LATENCY_COUNTER_BITS-1:0] ),
            .LatCntReq        ( local_lat_cnt_req[`LATENCY_COUNTER_BITS-1:0] ),
            .Trig             (                                   local_trig ),
            .TrigClear        (                             local_trig_clear ),
            .TrigIdIn           (              TrigIdIn), // from top level 

            // data readout
            .TokIn            (                                   tok_int[r] ),
            .TokOut           (                                 tok_int[r+1] ),
            .Read             (                                  read_region ),
            .DataToCore       (    region_data_bus[r][`REGION_DATA_BITS-1:0] ),
            .RegionTrigOut        (                       region_trig_bus[r] )

         ) ;

      end
   endgenerate


  always_comb begin
    RegionTrigOut = '0; // default
    for (int i = 0; i < 16; i++) begin
      if (tok_int[i]) begin
        RegionTrigOut = region_trig_bus[i]; // do we want a bus here? idk
      end
    end
  end

  
   ////////////////////////////
   //   hit-ORs generation   //
   ////////////////////////////

   wire [15:0][3:0] hit_or_int ;   // 4x hit-ORs from all 16 pixel regions

   generate

      genvar p_row ;   // pixel row
      genvar p_col ;   // pixel column

      // loop over pixel rows in the core (top to bottom)
      for(p_row = 0; p_row < 8; p_row++) begin

         // loop over pixel columns in the core (left to right)
         for(p_col = 0; p_col < 8; p_col++) begin

            localparam p_or = ( (p_row%2)*2 + p_col) % 4 ;
            localparam p_or_idx = (p_row*2) + (p_col/4) ;

            if(p_or_idx == 0)
               assign hit_or_int[0][p_or] = pix_hit_or[p_row][p_col] ;
            else
               assign hit_or_int[p_or_idx][p_or] = pix_hit_or[p_row][p_col] | hit_or_int[p_or_idx-1][p_or] ;
         end
      end
   endgenerate

  // TODO check if this is used!!!!!!!!!
  //  //assign HitOrOut[3:0] = hit_or_int[15][3:0] | HitOrIn[3:0] ;   // combine with hit-ORs from previous core
  //  OR2D8LVT hit_or0_lvt ( .A1( hit_or_int[15][0] ), .A2( HitOrIn[0] ), .Z( HitOrOut[0] )) ;
  //  OR2D8LVT hit_or1_lvt ( .A1( hit_or_int[15][1] ), .A2( HitOrIn[1] ), .Z( HitOrOut[1] )) ;
  //  OR2D8LVT hit_or2_lvt ( .A1( hit_or_int[15][2] ), .A2( HitOrIn[2] ), .Z( HitOrOut[2] )) ;
  //  OR2D8LVT hit_or3_lvt ( .A1( hit_or_int[15][3] ), .A2( HitOrIn[3] ), .Z( HitOrOut[3] )) ;


 
   // synopsys dc_script_begin
   // set_dont_touch hit_or0_lvt
   // set_dont_touch hit_or1_lvt
   // set_dont_touch hit_or2_lvt
   // set_dont_touch hit_or3_lvt
   // synopsys dc_script_end


   /////////////////////////////////////////////////
   //   output data arbitration and addressing    //
   /////////////////////////////////////////////////

   //
   // MUX triggered data from regions (help synthesis engine for this huge MUX... do this in two steps)
   //

   // take [`REGION_DATA_BITS-1:0] region_data_bus [15:0] and perform intermediate reduction...
   wire [3:0][15:0] data_or_stage_one ;

   assign data_or_stage_one[0][15:0] = region_data_bus[ 0][15:0] | region_data_bus[ 1][15:0] | region_data_bus[ 2][15:0] | region_data_bus[ 3][15:0] ;
   assign data_or_stage_one[1][15:0] = region_data_bus[ 4][15:0] | region_data_bus[ 5][15:0] | region_data_bus[ 6][15:0] | region_data_bus[ 7][15:0] ;
   assign data_or_stage_one[2][15:0] = region_data_bus[ 8][15:0] | region_data_bus[ 9][15:0] | region_data_bus[10][15:0] | region_data_bus[11][15:0] ;
   assign data_or_stage_one[3][15:0] = region_data_bus[12][15:0] | region_data_bus[13][15:0] | region_data_bus[14][15:0] | region_data_bus[15][15:0] ;

   // and then reduce again to single 16-bit output bus
   wire [15:0] data_or_stage_two ;
   assign data_or_stage_two[15:0] = data_or_stage_one[0][15:0] | data_or_stage_one[1][15:0] | data_or_stage_one[2][15:0] | data_or_stage_one[3][15:0] ;


   // **REMOVE ?
   //wire [3:0][7:0] conf_data_or_stage_one ;
   //wire      [7:0] conf_data_or_stage_two ;

   //assign conf_data_or_stage_one[0]  = data_conf_reg[0]  | data_conf_reg[1]  | data_conf_reg[2]  | data_conf_reg[3] ; //synopsys keep_signal_name "conf_data_or_stage_one"
   //assign conf_data_or_stage_one[1]  = data_conf_reg[4]  | data_conf_reg[5]  | data_conf_reg[6]  | data_conf_reg[7] ; //synopsys keep_signal_name "conf_data_or_stage_one"
   //assign conf_data_or_stage_one[2]  = data_conf_reg[8]  | data_conf_reg[9]  | data_conf_reg[10] | data_conf_reg[11]; //synopsys keep_signal_name "conf_data_or_stage_one"
   //assign conf_data_or_stage_one[3]  = data_conf_reg[12] | data_conf_reg[13] | data_conf_reg[14] | data_conf_reg[15]; //synopsys keep_signal_name "conf_data_or_stage_one"

   //assign conf_data_or_stage_two = conf_data_or_stage_one[0] | conf_data_or_stage_one[1] | conf_data_or_stage_one[2] | conf_data_or_stage_one[3] ;


   wire [15:0] data_last_region = data_or_stage_two[15:0] & {16{this_core_read}};

   // get the address of pixel-regions from the map of tokens
   //

   // 4-bit address of pixel-regions inside a core (0,1, .. 15)
   wire [3:0] region_addr ;

   // token-map to address encoder
   RegionAddrEnc   RegionAddrEnc (.TokMap(tok_map[15:0]), .RegionAddr(region_addr[3:0]) ) ;

  //  // concatenate core-row address and pixel-region address to build the full pixel-region address inside a core-column
  //  wire [`REGION_ADDRESS_BITS-1:0] col_region_addr ;
  //  assign col_region_addr[`REGION_ADDRESS_BITS-1:0] = { CoreRowAddrIn[`CORE_ROW_ADDRESS_BITS-1:0] , region_addr[3:0] } & {`REGION_ADDRESS_BITS{this_core_read}};


  //  generate
  //     genvar a ;
  //     for(a = 0; a < `REGION_ADDRESS_BITS; a++) begin: addr_or_gen
  //         OR2D8LVT addr_or (.A1(RegionAddrIn[a]), .A2(col_region_addr[a]), .Z(RegionAddrOut[a]));
  //     end
  //  endgenerate


    // synopsys dc_script_begin
    // set_dont_touch data_or*
    // set_dont_touch addr_or*
    // synopsys dc_script_end
    


   ////////////////////////
   //   output tie-low   //
   ////////////////////////

   // just a tied-down output pin, it is connected only at top-level to initialize token, fast-OR and data in the top core

   //assign OutLo = 1'b0 ; 

   // synopsys dc_script_begin
   // set_dont_touch LTIELO_OUTLO
   // synopsys dc_script_end 
   TIEL LTIELO_OUTLO (.ZN(OutLo)) ; 


   `endif   // DIGITAL_CORE_ABSTRACT 

endmodule : DigitalCore

// `endif   // DIGITAL_CORE__SV


//
// **NEW: standalone module to implement the static-address generator
//
`timescale 1ns / 1ps

module AddressGen (

   input  wire [5:0] AddrIn,
   output wire [5:0] AddrOut ) ;

   assign AddrOut[5:0] = AddrIn[5:0] - 6'b1 ;

endmodule : AddressGen

