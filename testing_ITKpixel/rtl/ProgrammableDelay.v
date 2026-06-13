
`ifndef PROGRAMMABLE_DELAY__V2001
`define PROGRAMMABLE_DELAY__V2001

//
// Dependencies:
//
// `include "sim/common/tech.vp   **WARN: only for reference, technology library compiled in separate task


`timescale 1ns / 1ps

module CkMux4to1_core (

    input  wire [3:0] in,
    input  wire [1:0] sel,
    output wire out

    ) ;
    
    wire mux1_out, mux2_out, mux3_out;
    CKMUX2D4 mux1(.I0(in[0]),.I1(in[1]),.S(sel[0] /*sel_buff[0]*/), .Z(mux1_out)),
             mux2(.I0(in[2]),.I1(in[3]), .S(sel[0] /*sel_buff[0]*/), .Z(mux2_out)),
             mux3(.I0(mux1_out), .I1(mux2_out), .S(sel[1]/*sel_buff[1]*/),.Z(mux3_out));
                       
     assign out = mux3_out;
    
endmodule // CkMux4to1_core 

//____________________________________________________________________________________



`timescale 1ns / 1ps

module CkMux12to1_core (

    input wire [11:0] in,
    input wire [3:0] sel,
    output wire out

    ) ;
    
    wire mux1_out, mux2_out, mux3_out, mux4_out, mux5_out;
    
    CkMux4to1_core mux1(.in(in[3:0]),.sel(sel[1:0] /*sel_buff[1:0]*/), .out(mux1_out));
    CkMux4to1_core mux2(.in(in[7:4]),.sel(sel[1:0] /*sel_buff[1:0]*/), .out(mux2_out));
    CkMux4to1_core mux3(.in(in[11:8]),.sel(sel[1:0] /*sel_buff[1:0]*/), .out(mux3_out));
    //CkMux4to1_core mux4(.in(in[15:12]),.sel(sel[1:0] /*sel_buff[1:0]*/), .out(mux4_out));   

    assign mux4_out = 1'b0;
    
    // Second multiplexing stage
    CkMux4to1_core mux5(.in({mux4_out, mux3_out, mux2_out, mux1_out}),.sel(sel[3:2]), .out(mux5_out));                   
    assign out = mux5_out;
    
endmodule //CkMux12to1_core 
 
//____________________________________________________________________________________



`timescale 1ns / 1ps

module ProgrammableDelay (

    input wire InToDelay,
    input wire [3:0] Select, 
    output wire OutDelayed

    ) ;

    wire [11:0] delay;
    assign delay[0] = InToDelay ;

   DEL01LVT \fine_delay[0].delay_chain_cell  (.I(InToDelay),	.Z(delay[1]));
   DEL01 \fine_delay[1].delay_chain_cell  (.I(delay[1]),	.Z(delay[2]));
   DEL01LVT \fine_delay[2].delay_chain_cell  (.I(delay[2]),	.Z(delay[3]));
   DEL01 \fine_delay[3].delay_chain_cell  (.I(delay[3]),	.Z(delay[4]));
   DEL01LVT \fine_delay[4].delay_chain_cell  (.I(delay[4]),	.Z(delay[5]));
   DEL01 \fine_delay[5].delay_chain_cell  (.I(delay[5]),	.Z(delay[6]));
   DEL01LVT \fine_delay[6].delay_chain_cell  (.I(delay[6]),	.Z(delay[7]));
   DEL01 \fine_delay[7].delay_chain_cell  (.I(delay[7]),	.Z(delay[8]));
   DEL01LVT \fine_delay[8].delay_chain_cell  (.I(delay[8]),	.Z(delay[9]));
   DEL01 \fine_delay[9].delay_chain_cell  (.I(delay[9]),	.Z(delay[10]));
   DEL01LVT \fine_delay[10].delay_chain_cell  (.I(delay[10]),	.Z(delay[11]));

   CkMux12to1_core mux12to1(.in(delay[11:0]),.sel(Select[3:0]), .out(OutDelayed));

endmodule

module ProgrammableDelaySlow (

    input wire InToDelay,
    input wire [3:0] Select, 
    output wire OutDelayed

    ) ;

    wire [11:0] delay;
    assign delay[0] = InToDelay ;

   DEL01 \fine_delay[0].delay_chain_cell  (.I(InToDelay),	.Z(delay[1]));
   DEL01 \fine_delay[1].delay_chain_cell  (.I(delay[1]),	.Z(delay[2]));
   DEL01 \fine_delay[2].delay_chain_cell  (.I(delay[2]),	.Z(delay[3]));
   DEL01 \fine_delay[3].delay_chain_cell  (.I(delay[3]),	.Z(delay[4]));
   DEL01 \fine_delay[4].delay_chain_cell  (.I(delay[4]),	.Z(delay[5]));
   DEL01 \fine_delay[5].delay_chain_cell  (.I(delay[5]),	.Z(delay[6]));
   DEL01 \fine_delay[6].delay_chain_cell  (.I(delay[6]),	.Z(delay[7]));
   DEL01 \fine_delay[7].delay_chain_cell  (.I(delay[7]),	.Z(delay[8]));
   DEL01 \fine_delay[8].delay_chain_cell  (.I(delay[8]),	.Z(delay[9]));
   DEL01 \fine_delay[9].delay_chain_cell  (.I(delay[9]),	.Z(delay[10]));
   DEL01 \fine_delay[10].delay_chain_cell  (.I(delay[10]),	.Z(delay[11]));

   CkMux12to1_core mux12to1(.in(delay[11:0]),.sel(Select[3:0]), .out(OutDelayed));

endmodule


//____________________________________________________________________________________



`timescale 1ns / 1ps

module ProgrammableDelayBuff24 (

    input  wire       InToBuf,
    input  wire [3:0] Select,
    output wire       OutDelayed,
    output wire       OutBuf

    ) ;

    // synopsys dc_script_begin
    // set_dont_touch main_clk_buffer_core
    // set_dont_touch InToBuf
    // set_dont_touch OutBuf
    // synopsys dc_script_end



    CKBD24LVT main_clk_buffer_core ( .I(InToBuf), .Z(OutBuf) ) ; // **NEW: hard-code the main buffer already in RTL



    // 40 MHz clock skew compensation
    ProgrammableDelay programmable_delay (

        .InToDelay  ( OutBuf      ),
        .Select     ( Select[3:0] ),
        .OutDelayed ( OutDelayed  )

    ) ;


endmodule

//____________________________________________________________________________________



`timescale 1ns / 1ps

module ProgrammableDelayBuff16 (

    input  wire       InToBuf,
    input  wire [3:0] Select,
    output wire       OutDelayed,
    output wire       OutBuf

    ) ;

    // synopsys dc_script_begin
    // set_dont_touch main_caledge_buffer_core
    // set_dont_touch InToBuf
    // set_dont_touch OutBuf
    // synopsys dc_script_end


    CKBD16LVT main_caledge_buffer_core ( .I(InToBuf), .Z(OutBuf) ) ; // **NEW: hard-code the main buffer already in RTL


    // CalEdge skew compensation
    ProgrammableDelaySlow programmable_delay (

        .InToDelay  ( OutBuf      ),
        .Select     ( Select[3:0] ),
        .OutDelayed ( OutDelayed  )

    ) ;


endmodule

`endif   // PROGRAMMABLE_DELAY__V2001

