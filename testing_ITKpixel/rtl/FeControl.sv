
//-----------------------------------------------------------------------------------------------------
// [Filename]       FeControl.sv [RTL]
// [Project]        HL-LHC ATLAS/CMS pixel readout chip
// 
// [Language]       SystemVerilog 2012 [IEEE Std. 1800-2012]
// 
// [Hierarchy]      RD53B ---> PixelArray ---> DigitalCore ---> FeControl
//
//                  FeControl:
//                           |--> CgWrapper
//                           |--> LHQD1_TMR
//
// [Description]    Control logic for the analog front-end:
//
//                     - implements latch-based per-pixel configuration registers, optionally
//                       with TMR protection
//                     - switch between hard-wired default-configuration and external configuration
//                     - implements "digital injection" for test purposes using CalEdge to mimic
//                       the hit pulse and bypass analog front-end output
//                     - per-pixel enable/disable S0 and S1 calibration signals
//                     - per-pixel enable/disable of hit (either analog or digital) and hit-OR
//                       fed to pixel-region logic.
//
// [Notes]          The same code supports both ATLAS and CMS front-end flavors.
//
// [Status]         devel
//-----------------------------------------------------------------------------------------------------


`ifndef FE_CONTROL__SV   // include guard
`define FE_CONTROL__SV

//
// Dependencies:
//
// `include "rtl/common/defines.sv"       // provided via files.f
// `include "rtl/common/CgWrapper.v"      // provided via files.f as CgWrapper.sv
// `include "rtl/TMR/core/LHQD1_TMR.v"   // provided via files.f as models.sv

`timescale 1ns / 1ps
//`include "timescale.v"

//
// trimming DAC startup value
//
`ifdef ATLAS_CHIP
   `define TDAC_DEFAULT 5'b01111
`elsif CMS_CHIP
   `define TDAC_DEFAULT 5'b00000         // for CMS start with very high threshold (ref. to https://gitlab.cern.ch/rd53/RD53B/issues/249)
`endif


module FeControl (

   // configuration section
   input  wire       PixelConfDefault,   // MUX control to switch between default configuration and external configuration
   input  wire       PixelConfWr5bit,    // write = 1'b1 / read = 1'b0 => write/read configuration for trimming DAC only to speedup S-curves
   input  wire       PixelConfWr3bit,    // write = 1'b1 / read = 1'b0 => write/read configuration for remaining bits
   input  wire [7:0] PixelConfDataWr,    // new external configuration data
   output wire [7:0] PixelConfDataRd,    // readback configuration data
   output wire PwrDwn,                   // **NOTE: also fed to pixel-region logic for clock-gating !
`ifdef CMS_CHIP
   input  wire EnSeuCount,               // configuration bit to enable routing of SeuAny to HitOr
`endif
   // calibration section
   input  wire DefaultCalEn,      // by default the first two pixel rows in each core are ready for charge-injection without the need to enable calibration
   input  wire EvenOddS0,         // injection takes place for enabled pixels when either S0 or S1 switch from low to high (proper swapping between even/odd pixels based on pixel position in each quad is performed in the core)
   input  wire EvenOddS1,
   output wire FeS0,              // actual switching signals fed to calibration circuit inside analog front-end
   output wire FeS1,

   // configuration bits to the trimming DAC (4+4 bits ATLAS, 5-bits CMS)
`ifdef ATLAS_CHIP
   output wire [3:0] FeDTH1,
   output wire [3:0] FeDTH2,
`elsif CMS_CHIP
   output wire [4:0] FeThDac,
`endif

   // hit pulse from analog front-end
   input wire FeHit,

   // **DEBUG: digital injection
   input wire EnDigHit,
   input wire CalEdge,   // used to mimic a hit pulse and bypass DISC analog output

   // hit information
   output wire HitOut,   // either analog or digital hit, after per-pixel enabling
   output wire HitOr     // prompt hitOR, after per-pixel enabling

   ) ;



   ///////////////////////////////////////////////////////////////////////////////////////////////
   //   latch-based Pixel Configuration Register (PCR), optionally with TMR and SEU detection   //
   ///////////////////////////////////////////////////////////////////////////////////////////////

   wire [7:0] conf_latches ;

   //
   // Configuration bits assignment:
   //
   // B0     =>  hit enable (aka pixel "masking")
   // B1     =>  charge-injection calibration enable
   // B2     =>  hit-OR enable
   // B3-7   =>  trimming DAC code (B7 trimming DAC sign for DIFF front-end)

   //
   // **PRIORITY** list for TMR:
   //
   //          tdac sign
   // hit_en > tdac_code[4] > tdac_code[3] > tdac_code[2] > tdac_code[1] > tdac_code[0]
   // 
   // TMR on hitOr_en and cal_en is any case completely useless => 5/6-bits would be enough
   //
   // See also https://gitlab.cern.ch/rd53/RD53B/issues/128
   //

   `ifdef ATLAS_CHIP   //_____________________________________

      LHQD1_TMR  conf_latch_0 (.D(PixelConfDataWr[0]), .E(PixelConfWr3bit), .Q(conf_latches[0]), .Q0(), .Q1(), .Q2()) ;   // hit_en
      LHQD1      conf_latch_1 (.D(PixelConfDataWr[1]), .E(PixelConfWr3bit), .Q(conf_latches[1])) ;                        // cal_en              **NO TMR**
      LHQD1      conf_latch_2 (.D(PixelConfDataWr[2]), .E(PixelConfWr3bit), .Q(conf_latches[2])) ;                        // hitOr_en            **NO TMR**
      LHQD1      conf_latch_3 (.D(PixelConfDataWr[3]), .E(PixelConfWr5bit), .Q(conf_latches[3])) ;                        // tdac_code[0]        **NO TMR**
      LHQD1_TMR  conf_latch_4 (.D(PixelConfDataWr[4]), .E(PixelConfWr5bit), .Q(conf_latches[4]), .Q0(), .Q1(), .Q2()) ;   // tdac_code[1]
      LHQD1_TMR  conf_latch_5 (.D(PixelConfDataWr[5]), .E(PixelConfWr5bit), .Q(conf_latches[5]), .Q0(), .Q1(), .Q2()) ;   // tdac_code[2]
      LHQD1_TMR  conf_latch_6 (.D(PixelConfDataWr[6]), .E(PixelConfWr5bit), .Q(conf_latches[6]), .Q0(), .Q1(), .Q2()) ;   // tdac_code[3]
      LHQD1_TMR  conf_latch_7 (.D(PixelConfDataWr[7]), .E(PixelConfWr5bit), .Q(conf_latches[7]), .Q0(), .Q1(), .Q2()) ;   // tdac_code[4] = tdac_sign for DIFF front-end

   `elsif CMS_CHIP   //_____________________________________

      wire [3:0] q0, q1, q2 ;

      wire [3:0] upset ; // **NOTE: SEU detection is performed only for a subset of TMR latches, tdac_code[1:0] are protected by TMR but without SEU detection

      LHQD1_TMR  conf_latch_0 (.D(PixelConfDataWr[0]), .E(PixelConfWr3bit), .Q(conf_latches[0]), .Q0(q0[0]), .Q1(q1[0]), .Q2(q2[0])) ;   // hit_en                                 + SEU detection
      LHQD1      conf_latch_1 (.D(PixelConfDataWr[1]), .E(PixelConfWr3bit), .Q(conf_latches[1])                                    ) ;   // cal_en              **NO TMR**
      LHQD1      conf_latch_2 (.D(PixelConfDataWr[2]), .E(PixelConfWr3bit), .Q(conf_latches[2])                                    ) ;   // hitOr_en            **NO TMR**
      LHQD1      conf_latch_3 (.D(PixelConfDataWr[3]), .E(PixelConfWr5bit), .Q(conf_latches[3])                                    ) ;   // tdac_code[0]        **NO TMR**
      LHQD1_TMR  conf_latch_4 (.D(PixelConfDataWr[4]), .E(PixelConfWr5bit), .Q(conf_latches[4])                                    ) ;   // tdac_code[1]
      LHQD1_TMR  conf_latch_5 (.D(PixelConfDataWr[5]), .E(PixelConfWr5bit), .Q(conf_latches[5]), .Q0(q0[1]), .Q1(q1[1]), .Q2(q2[1])) ;   // tdac_code[2]                           + SEU detection
      LHQD1_TMR  conf_latch_6 (.D(PixelConfDataWr[6]), .E(PixelConfWr5bit), .Q(conf_latches[6]), .Q0(q0[2]), .Q1(q1[2]), .Q2(q2[2])) ;   // tdac_code[3]                           + SEU detection
      LHQD1_TMR  conf_latch_7 (.D(PixelConfDataWr[7]), .E(PixelConfWr5bit), .Q(conf_latches[7]), .Q0(q0[3]), .Q1(q1[3]), .Q2(q2[3])) ;   // tdac_code[4]                           + SEU detection

      assign upset[0] = (q0[0] ^ q1[0]) | (q1[0] ^ q2[0]) ;
      assign upset[1] = (q0[1] ^ q1[1]) | (q1[1] ^ q2[1]) ;
      assign upset[2] = (q0[2] ^ q1[2]) | (q1[2] ^ q2[2]) ;
      assign upset[3] = (q0[3] ^ q1[3]) | (q1[3] ^ q2[3]) ;

      wire seu_any ;
      assign seu_any = upset[0] | upset[1] | upset[2] | upset[3] ;

   `endif   //_____________________________________


   ////////////////////////////////////////////////////////////////////////////////////////////////////////
   //   configuration bits assignment and MUX between default configuration and external configuration   //
   ////////////////////////////////////////////////////////////////////////////////////////////////////////

   //
   // **NOTE: This ensures that after chip power-up all pixels are already properly
   //         configured without the need of a reset in configuration latches.
   //         NO RESET is used for pixel configuration registers to avoid that any
   //         spurious reset would erease the configuration of thousands pixels !
   //         Use the PixelConfDefault static line to switch between hard-wired
   //         defaults (tie cells) and latches outputs at any time
   //

   logic conf_hit_en ;
   logic conf_cal_en ;
   logic conf_hitOr_en ;

   logic [4:0] conf_tdac_code ;   // **NOTE: same for both ATLAS and CMS chips
   //logic       conf_tdac_sign ;   // **REMOVED, simply use conf_tdac_code[4]

   always_comb begin
      if(PixelConfDefault == 1'b1)
         //{conf_tdac_sign , conf_tdac_code[3:0] , conf_hitOr_en , conf_cal_en , conf_hit_en} = {1'b0 , 4'b1111 , 1'b1 , DefaultCalEn , 1'b1} ; 
         {conf_tdac_code[4:0] , conf_hitOr_en , conf_cal_en , conf_hit_en} = {`TDAC_DEFAULT , 1'b1 , DefaultCalEn , 1'b1} ; 
      else
         //{conf_tdac_sign , conf_tdac_code[3:0] , conf_hitOr_en , conf_cal_en , conf_hit_en} = conf_latches[7:0] ;
         {conf_tdac_code[4:0] , conf_hitOr_en , conf_cal_en , conf_hit_en} = conf_latches[7:0] ;
   end   // always_comb


   /////////////////////////////////////
   //   configuration readback data   //
   /////////////////////////////////////

   // **NOTE: the configuration readout must read actual values, not latches outputs !
   //assign PixelConfDataRd[7:0] = {conf_tdac_sign , conf_tdac_code[3:0] , conf_hitOr_en , conf_cal_en , conf_hit_en} ;
   assign PixelConfDataRd[7:0] = {conf_tdac_code[4:0] , conf_hitOr_en , conf_cal_en , conf_hit_en} ;


   ////////////////////////
   //   output signals   //
   ////////////////////////

   // switching signals FeS0 and FeS1 (AFE input pins) are locally gated based on cal_en configuration bit,
   // proper swapping between even/odd pixels based on pixel position in each quad is performed in the core

   assign FeS0 = (conf_cal_en == 1'b1) ? EvenOddS0 : 1'b1 ;   // **NOTE: 1'b1 ensures that if no calibration occurs, FeS0 = FeS1 = VDDD hence Cinj is tied to GNDA
   assign FeS1 = (conf_cal_en == 1'b1) ? EvenOddS1 : 1'b1 ;

   //
   // assign configuration bits for trimming DAC
   //

`ifdef ATLAS_CHIP
   //assign FeDTH1 = (conf_tdac_sign == 1'b1) ? 4'b0000 : conf_tdac_code[3:0] ;
   //assign FeDTH2 = (conf_tdac_sign == 1'b1) ? conf_tdac_code[3:0] : 4'b0000 ;
   assign FeDTH1[3:0] = (conf_tdac_code[4] == 1'b1) ? 4'b0000 : conf_tdac_code[3:0] ;
   assign FeDTH2[3:0] = (conf_tdac_code[4] == 1'b1) ? conf_tdac_code[3:0] : 4'b0000 ;
`elsif CMS_CHIP
   assign FeThDac[4:0] = conf_tdac_code[4:0] ;
`endif

   // power-down
   assign PwrDwn = 1'b0;   // if hit is disabled (i.e. the pixel is "masked"), the pixel logic is turned off TODO: fix


   // **DEBUG: digital injection (use CalEdge pulse to mimic a hit from the DISC output)
   wire dig_hit ;
   assign dig_hit = CalEdge & conf_cal_en ;


   // MUX between analog and digital hits
   wire hit_mux ;
   assign hit_mux = (EnDigHit == 1'b1) ? dig_hit : FeHit ;


   // enable/disable hit and hit-OR fed to pixel-region logic
   assign HitOut = hit_mux & conf_hit_en ;

`ifdef ATLAS_CHIP
   assign HitOr = hit_mux & conf_hitOr_en ;
`elsif CMS_CHIP
   assign HitOr = (EnSeuCount == 1'b1) ? seu_any : ( hit_mux & conf_hitOr_en ) ;
`endif

endmodule : FeControl

`endif   // FE_CONTROL__SV

