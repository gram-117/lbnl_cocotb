
//-----------------------------------------------------------------------------------------------------
// [Filename]       RegionAddrEnc.sv [RTL]
// [Project]        HL-LHC ATLAS/CMS pixel readout chip
// [Author]         -
// [Language]       SystemVerilog 2012 [IEEE Std. 1800-2012]
// [Created]        -
// [Modified]       -
// [Description]    Each 8x8 core contains 16x DBA regions of 1x4 pixels organized in 8 rows x 2 columns.
//                  This module encodes as a binary number the position (address) of each 1x4 pixel region
//                  inside the core from the map of tokens according to the numbering scheme adopted for
//                  pixel regions in the core.
// [Notes]          -
// [Status]         devel
//-----------------------------------------------------------------------------------------------------


`ifndef REGION_ADDR_ENC__SV   // include guard
`define REGION_ADDR_ENC__SV

//
// Dependencies:
//
// n/a


//
// **NOTE: The numbering scheme adopted for 1x4 pixel regions inside the core is:
//
//   PixelRegion_0   PixelRegion_1
//   PixelRegion_2   PixelRegion_3
//   PixelRegion_4   PixelRegion_5
//       ...             ...
//       ...             ...
//   PixelRegion_14  PixelRegion_15
//


`timescale 1ns / 1ps
//`include "timescale.v"


module RegionAddrEnc (

   input  wire [0:15] TokMap,   // token flags from all 16 pixel regions 1x4 in the core
   output wire [3:0] RegionAddr   // decoded pixel-region number 0,1,2, ... 15 in true binary

   ) ;


   wire [16:0] addr_lut [3:0] ;   // TokMap to RegionAddr table

   assign addr_lut[0][16] = 1'b1 ;
   assign addr_lut[1][16] = 1'b1 ;
   assign addr_lut[2][16] = 1'b1 ;
   assign addr_lut[3][16] = 1'b1 ;

   generate

      genvar k ;   // 0,1,2 ... 15

      // loop over pixel-regions
      for(k = 0; k < 16; k++) begin : RegionAddrGen

         assign addr_lut[0][k] = !(addr_lut[0][k+1] & TokMap[k]) ;

         if(k%2 == 0)   // k = 0,2,4,6,8,10,12,14
            assign addr_lut[1][k] = !(addr_lut[1][k+2] & TokMap[k]) ;

         if(k%4 == 0)   // k = 0,4,8,12
            assign addr_lut[2][k] = !(addr_lut[2][k+4] & TokMap[k]) ;

         if(k%8 == 0)   // k = 8
            assign addr_lut[3][k] = !(addr_lut[3][k+8] & TokMap[k]) ;
      end
   endgenerate


   // assign the final region-address
   assign RegionAddr[0] = !addr_lut[0][0] ;
   assign RegionAddr[1] = !addr_lut[1][0] ;
   assign RegionAddr[2] = !addr_lut[2][0] ;
   assign RegionAddr[3] = !addr_lut[3][0] ;

endmodule : RegionAddrEnc

`endif   // REGION_ADDR_ENC__SV

