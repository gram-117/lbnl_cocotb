# test_my_design.py (extended)

import cocotb
from cocotb.triggers import RisingEdge, FallingEdge, Timer
from cocotb.types    import LogicArray


async def generate_clock(dut):
    """Simulate region gate clock, enable after dut enables PixelClkEn""" 
    while True:
        dut.PixelClkGated.value = 0
        await Timer(200, unit="ps")
        if dut.PixelClkEn.value == 1:
            dut.PixelClkGated.value = 1
        else:
            dut.PixelClkGated.value = 0
        await Timer(200, unit="ps")


@cocotb.test()
async def pixel_test(dut):
    """Try accessing the design."""

    cocotb.start_soon(generate_clock(dut))  # run the clock "in the background"
    dut.Reset_b.value = 1
    dut.TotMemWriteAddr.value = LogicArray.from_unsigned(0, 8)
    dut.TotMemReadAddr.value = LogicArray.from_unsigned(0, 8) # pretty sure this is one-hot encoded
    dut.Tot6to4Mapping.value = 0
    dut.TotDualEdgeCount.value = 1
    dut.HitLeAny.value = 0
    await Timer(1, unit="ns")
    dut.Reset_b.value = 0
    await Timer(1, unit="ns")  # wait a bit
    dut.Reset_b.value = 1
    await Timer(1, unit="ns")

    

    for i in range(0, 7):
        one_hot = 1 << i
        dut.TotMemWriteAddr.value = LogicArray.from_unsigned(one_hot, 8)
        dut.TotMemReadAddr.value = LogicArray.from_unsigned(one_hot, 8)
        dut.Hit.value = 1 # simulate hit
        # wait small amt of time? 
        await Timer(20, unit="ps") # async but let logic do its thing

        await RisingEdge(dut.PixelClkGated)  
        cocotb.log.info("PixelClkGate rising edge:  %s", dut.PixelClkEn.value)
        assert int(dut.PixelClkEn.value) == 1

        # do whatever
        await Timer((450 + (i * 400)), unit="ps") # at least one clock cycle
        dut.Hit.value = 0 # end hit, value should be in datamem. 
        #await RisingEdge(dut.PixelClkGated) 
        await Timer(560, unit="ps") # async but let logic do its thing + one clock cycle

        #await RisingEdge(dut.PixelClkGated)  
        cocotb.log.info("wrote to mem[%s], got %s", i, dut.TotMemDataOut.value) # i instead of one hot
        cocotb.log.info("expected %s", (((450 + (i * 400) + 20) // 400)))

        #cocotb.log.info("after TE of hit and rising edge, HitTe is %s", dut.HitTe)
        await Timer(4, unit="ns")

    # await Timer(1, unit="ns")
    # cocotb.log.info("my_sign is %s", )
    # assert dut. == 0