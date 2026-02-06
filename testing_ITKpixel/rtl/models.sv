// misc stuff from pixel logic and assosicated files that I don't have access
// to the module def/ analog stuff etc
// //////////////////////////////////////
// //   custom Toggle FlipFlop (TFF)   //
// //////////////////////////////////////
module TFF_NCLK_NRST (
    input  wire nclk,   // negative edge-triggered FF
    input  wire nrst,   // asynchronous reset, active-low
    output logic q,
    output logic qn
    ) ;

//     DFNCND1 q_reg (.CDN(nrst), .CPN(nclk), .D(qn), .Q(q), .QN(qn));

    always_ff @(negedge nclk or negedge nrst) begin
        if(~nrst)
            q <= 1'b0 ;
        else
            q <= !q ;
    end
    assign qn = !q;
endmodule


// use custom 4-bit latch => **CHANGED** to use new cell-based multibit latch
module LNQD1shrinkX4_V2 (
input EN,
input D0,
input D1,
input D2,
input D3,
output logic Q0 = 1'b0,
output logic Q1 = 1'b0,
output logic Q2 = 1'b0,
output logic Q3 = 1'b0
);

always_latch begin
    if (~EN) begin // transparent low best guess from enable signals
        Q0 = D0;
        Q1 = D1;
        Q2 = D2;
        Q3 = D3;
    end
end

endmodule


