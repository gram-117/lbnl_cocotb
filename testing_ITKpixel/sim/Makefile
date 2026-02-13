# RCA/sim/Makefile

SIM ?= icarus # icarus has complains abt sv verilator instead?
TOPLEVEL_LANG ?= verilog
# VERILATOR_ARGS += -Wno-fatal
# EXTRA_ARGS += -Wno-fatal
# COMPILE_ARGS += -Wno-fatal
# VERILATOR_VERILOG ?= 1
# COMPILE_ARGS += --sv
# EXTRA_ARGS   += --trace --trace-fst --trace-structs --trace-depth 10
# COMPILE_ARGS += --trace --trace-fst --trace-structs --trace-depth 10

# project dir = one level above this sim/ directory
PROJ_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST)))/..)

# HDL sources
VERILOG_SOURCES := $(addprefix $(PROJ_DIR)/rtl/,$(shell cat $(PROJ_DIR)/rtl/files.f))

# # Top-level module name in the HDL
# TOPLEVEL := PixelRegionLogic
TOPLEVEL := PixelLogic

# # Python test module (file name without .py)
# MODULE := pixelregion_test_top
MODULE := pixel_test_top

# Tests live in ../tests, so add that to PYTHONPATH
export PYTHONPATH := $(PROJ_DIR)/tests:$(PYTHONPATH)


include $(shell cocotb-config --makefiles)/Makefile.sim
