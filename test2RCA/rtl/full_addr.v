module full_addr (
input a, b, cin,
output s, cout
);

assign {cout, s} = a + b + cin;


endmodule