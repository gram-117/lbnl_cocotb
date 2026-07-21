
// Dependencies :
//
// $TSMC_PDK/../../digital/Front_End/verilog/tcbn65lp_200a/tcbn65lp.v


`ifndef CG_MOD__SV   // include guard
`define CG_MOD__SV

`timescale 1ns / 1ps

`default_nettype none


//module CG_MOD_pos (ClkIn, Enable, test, ClkOut);
//input ClkIn,Enable,test;
//output ClkOut;
//wire tm_out, ck_inb;
//reg enl;
//
//    assign tm_out = Enable | test;
//    assign ck_inb = ~ClkIn;
//    always @ (ck_inb or tm_out)
//    if (ck_inb)
//        enl = tm_out;
//    assign ClkOut = ClkIn & enl;
//
//endmodule

//module CG_MOD_neg (ClkIn, Enable, test, ClkOut);
//    input ClkIn,Enable,test;
//    output ClkOut;
//    wire tm_out;
//    reg enl;
//        assign tm_out = Enable | test;
//        always @ (ClkIn or tm_out )
//        if (ClkIn)
//            enl = tm_out;
//        assign ClkOut = ClkIn | ~enl;
//endmodule


module CG_MOD (
   input  wire ClkIn,
   input  wire Enable,
   output wire ClkOut   
   ) ;
   // CKLNQD1 cg_cell (.TE (1'b0), .E (Enable), .CP (ClkIn), .Q(ClkOut));
   // original code from rtl file:
   
    wire ck_inb;
    reg enl = 1'b0; // added init
    assign ck_inb = ~ClkIn;
    always_latch begin // was always @ (ck_inb or Enable)
        if (ck_inb) enl = Enable;
    end
    assign ClkOut = ClkIn & enl;
endmodule

/*
module CG_MOD_neg (ClkIn, Enable, ClkOut);
    input wire ClkIn,Enable;
    output wire ClkOut;
    //reg enl;

        CKLHQD1 cg_cell_neg (.TE(1'b0), .E (Enable), .CPN (ClkIn),.Q(ClkOut));
    //always @ (ClkIn or Enable)
    //if (ClkIn)
    //enl = Enable ;
    //assign ClkOut = ClkIn | ~enl;
endmodule
*/

`default_nettype wire

`endif   // include guard
