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

// for free slots in network layer
module DualEdgeFf (
    input  logic clk,
    input  logic rst_n,
    input  logic d,
    output logic q
);

logic q_pos;
logic q_neg;

always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n)
        q_pos <= 1'b0;
    else
        q_pos <= d;
end

always_ff @(negedge clk or negedge rst_n) begin
    if (!rst_n)
        q_neg <= 1'b0;
    else
        q_neg <= d;
end

always_comb begin
    if (clk)
        q = q_pos;
    else
        q = q_neg;
end

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

// standard cells models:
//`CKBD24LVT`, `CKBD16LVT`, `CKBD8LVT`, `CKBD8`, `CKBD4`, `CKBD2`, `CKBD1`, 
//`CKMUX2D4`, `DEL01`, `DEL01LVT`, `LHQD1`, `LHQD2`, `DFNCND1`, `CKLNQD1`, 
//`AO222D1`, `BUFFD2LVT`, `OR2D8LVT`, `TIEL`.

// clock buffers
module CKBD1(input logic I, output logic Z);
    assign Z = I;
endmodule

module CKBD2(input logic I, output logic Z);
    assign Z = I;
endmodule

module CKBD4(input logic I, output logic Z);
    assign Z = I;
endmodule

module CKBD8(input logic I, output logic Z);
    assign Z = I;
endmodule

module CKBD8LVT(input logic I, output logic Z);
    assign Z = I;
endmodule

module CKBD16LVT(input logic I, output logic Z);
    assign Z = I;
endmodule

module CKBD24LVT(input logic I, output logic Z);
    assign Z = I;
endmodule

// buffer
module BUFFD2LVT(
    input  logic I,
    output logic Z
);
    assign Z = I;
endmodule

// or gate
module OR2D8LVT(
    input logic A1,
    input logic A2,
    output logic Z
);
    assign Z = A1 | A2;
endmodule

// and gate
module AO222D1(
    input logic A1,A2,
    input logic B1,B2,
    input logic C1,C2,
    output logic Z
);

assign Z = (A1&A2) |
           (B1&B2) |
           (C1&C2);

endmodule

// tie low
module TIEL(
    output logic ZN
);

assign ZN = 1'b0;

endmodule

// // clock mux
// if S=0 -> A
// if S=1 -> B

module CKMUX2D4(
    input logic I0,
    input logic I1,
    input logic S,
    output logic Z
);

assign Z = S ? I1 : I0;

endmodule

// delay cell
module DEL01(
    input logic I,
    output logic Z
);

assign Z = I;
// or assign #1 Z = I; // dont need for cocotb stuff for now

endmodule

module DEL01LVT(
    input logic I,
    output logic Z
);

assign Z = I;

endmodule

// latch
module LHQD1(
    input logic D,
    input logic E,
    output logic Q
);

always_latch
    if (E)
        Q <= D;

endmodule

module LHQD2(
    input logic D,
    input logic E,
    output logic Q
);

always_latch
    if (E)
        Q <= D;

endmodule

// TMR latch: three independent copies voted by majority; for sim all three are identical
module LHQD1_TMR(
    input  logic D,
    input  logic E,
    output logic Q,
    output logic Q0,
    output logic Q1,
    output logic Q2
);

always_latch
    if (E) begin
        Q  <= D;
        Q0 <= D;
        Q1 <= D;
        Q2 <= D;
    end

endmodule

//negedge ff
module DFNCND1(
    input logic D,
    input logic CK,
    output logic Q
);

always_ff @(negedge CK)
    Q <= D;

endmodule

// clock gating latch
module CKLNQD1(
    input logic CP,
    input logic E,
    output logic Q
);

logic en_latched;

always_latch
    if (!CP)
        en_latched <= E;

assign Q = CP & en_latched;

endmodule
