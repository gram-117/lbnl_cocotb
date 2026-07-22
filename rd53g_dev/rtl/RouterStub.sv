module RouterStub (
    // neighbors + self buffer status
    input  wire [$clog2(`NETWORK_MEM_DEPTH)-1:0] buf_status_up,
    input  wire [$clog2(`NETWORK_MEM_DEPTH)-1:0] buf_status_dn,
    input  wire [$clog2(`NETWORK_MEM_DEPTH)-1:0] buf_status_l,
    input  wire [$clog2(`NETWORK_MEM_DEPTH)-1:0] buf_status_r,
    input  wire [$clog2(`NETWORK_MEM_DEPTH)-1:0] buf_status_self,

    // one-hot routing decision:
    // [3] = up
    // [2] = down
    // [1] = left
    // [0] = right
    output wire [3:0]                            routing_decision
);

    // Stub: always route down
    assign routing_decision = 4'b0010;

endmodule