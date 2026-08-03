
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

// NOTE UNTOUCHED

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


   // was standard cell: CKBD16LVT (outbuf)
   assign O = I;
   // was standard cell: CKBD8LVT (locbuf)
   assign L = O;

endmodule : SigFork


module SigForkConf (

   input  wire I,
   output wire O,
   output wire L

   ) ;

   // was standard cell: CKBD8LVT (outbuf)
   assign O = I;
   // was standard cell: CKBD4 (locbuf)
   assign L = O;

endmodule : SigForkConf



module SigForkStatic (

   input  wire I,
   output wire O,
   output wire L

   ) ;

   // was standard cell: CKBD4 (outbuf)
   assign O = I;
   // was standard cell: CKBD2 (locbuf)
   assign L = O;

endmodule : SigForkStatic

`endif   // SIG_FORKS__SV

