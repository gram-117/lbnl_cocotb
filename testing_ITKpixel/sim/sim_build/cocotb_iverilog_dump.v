module cocotb_iverilog_dump();
initial begin
    $dumpfile("sim_build/NetworkedCore.fst");
    $dumpvars(0, NetworkedCore);
end
endmodule
