# test_my_design.py (extended)

import cocotb
from cocotb.triggers import FallingEdge, Timer
from cocotb.types    import LogicArray


async def generate_clock(dut):
    """Generate clock pulses."""

    for _ in range(10):
        dut.clk.value = 0
        await Timer(1, unit="ns")
        dut.clk.value = 1
        await Timer(1, unit="ns")


@cocotb.test()
async def my_second_test(dut):
    """Try accessing the design."""

    cocotb.start_soon(generate_clock(dut))  # run the clock "in the background"

    await Timer(5, unit="ns")  # wait a bit
    await FallingEdge(dut.clk)  # wait for falling edge/"negedge"

    for i in range (0, 12, 3):
        for j in range (0, 10, 2):
            dut.a.value, dut.b.value = i, j
            await Timer(3, unit="ns") # wait for update
            cocotb.log.info("%s + %s is %s", i, j, dut.out.value)
            # build expected 
            sum_val = i + j
            expected_out = LogicArray.from_unsigned(sum_val & 0xF, 4)
            expected_ovf = (sum_val >> 4) & 1

            assert dut.out.value == expected_out
            assert dut.overflow.value == expected_ovf