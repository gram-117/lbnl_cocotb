module cocotb_iverilog_dump();
initial begin
    $dumpfile("sim_build/DigitalCore.fst");
    $dumpvars(0, DigitalCore);
end
endmodule
