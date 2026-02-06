module cocotb_iverilog_dump();
initial begin
    $dumpfile("sim_build/PixelLogic.fst");
    $dumpvars(0, PixelLogic);
end
endmodule
