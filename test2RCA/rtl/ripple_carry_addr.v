module ripple_carry_addr (
input clk,
input [3:0] a,
input [3:0] b,
output [3:0] out,
output overflow
);

wire c1, c2, c3;
full_addr u1(a[0], b[0], 1'b0, out[0], c1);
full_addr u2(a[1], b[1], c1, out[1], c2);
full_addr u3(a[2], b[2], c2, out[2], c3);
full_addr u4(a[3], b[3], c3, out[3], overflow);

endmodule