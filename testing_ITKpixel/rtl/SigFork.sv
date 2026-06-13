
//-----------------------------------------------------------------------------------------------------
// [Filename]       SigFork.sv [STRUCTURAL]
// [Project]        HL-LHC ATLAS/CMS pixel readout chip
// [Author]         -
// [Language]       SystemVerilog 2012 [IEEE Std. 1800-2012]
// [Created]        -
// [Modified]       -
// [Description]    Hard-coded "signal fork" for time-sensitive core-to-core signals propagation.
// [Notes]          -
// [Status]         devel
//-----------------------------------------------------------------------------------------------------


`ifndef SIG_FORKS__SV   // include guard
`define SIG_FORKS__SV

//
//            |\
//  I  _______| \_____________________________  O (to next core)
//            | /     |
//            |/      |           |\
//                    |___________| \_________  L (local signal)
//                                | /
//                                |/
//


//
// Dependencies:
//
// n/a


`timescale 1ns / 1ps
//`include "timescale.v"


module SigFork (

   input  wire I,   // input signal from previous digital core
   output wire O,   // output signal fed to the next digital core
   output wire L    // local buffered copy of the input signal fed to core logic

   ) ;


   CKBD16LVT outbuf(.I(I), .Z(O)) ;   // CKBD16LVT is the second largest clock buffer provided by the library
   CKBD8LVT  locbuf(.I(O), .Z(L)) ;

   // synopsys dc_script_begin
   // set_dont_touch outbuf
   // set_dont_touch locbuf
   // synopsys dc_script_end

endmodule : SigFork


module SigForkConf (

   input  wire I,   
   output wire O,  
   output wire L  

   ) ;


   CKBD8LVT outbuf(.I(I), .Z(O)) ;
   CKBD4    locbuf(.I(O), .Z(L)) ;

   // synopsys dc_script_begin
   // set_dont_touch outbuf
   // set_dont_touch locbuf
   // synopsys dc_script_end

endmodule : SigForkConf



module SigForkStatic (

   input  wire I,   
   output wire O,  
   output wire L  

   ) ;


   CKBD4  outbuf(.I(I), .Z(O)) ;
   CKBD2  locbuf(.I(O), .Z(L)) ;

   // synopsys dc_script_begin
   // set_dont_touch outbuf
   // set_dont_touch locbuf
   // synopsys dc_script_end

endmodule : SigForkStatic

`endif   // SIG_FORKS__SV

