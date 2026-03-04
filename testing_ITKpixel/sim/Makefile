# RCA/sim/Makefile

# SIM ?= icarus
# TOPLEVEL_LANG ?= verilog # just set to verilog, sv works 

SIM ?= verilator #
TOPLEVEL_LANG ?= verilog

ifeq ($(SIM),verilator)
  VERILATOR_ARGS += --sv
  VERILATOR_ARGS += --trace --trace-fst --trace-structs
  VERILATOR_ARGS += -Wno-fatal -Wno-UNOPTFLAT
endif

# project dir = one level above this sim/ directory
PROJ_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST)))/..)

# HDL sources
VERILOG_SOURCES := $(addprefix $(PROJ_DIR)/rtl/,$(shell cat $(PROJ_DIR)/rtl/files.f))

# # Top-level module name in the HDL
# # Python test module (file name without .py)
# TOPLEVEL := LatencyMem
# MODULE := LatencyMem_test_top

# MODULE := pixelregion_test_top
# TOPLEVEL := PixelRegionLogic

# known working
MODULE := pixel_test_top
TOPLEVEL := PixelLogic

# Tests live in ../tests, so add that to PYTHONPATH
export PYTHONPATH := $(PROJ_DIR)/tests:$(PYTHONPATH)


include $(shell cocotb-config --makefiles)/Makefile.sim
