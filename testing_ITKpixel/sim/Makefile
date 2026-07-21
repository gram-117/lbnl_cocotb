

SIM ?= icarus
TOPLEVEL_LANG ?= verilog # just set to verilog, sv works 

# SIM ?= verilator #
# TOPLEVEL_LANG ?= verilog

# project dir = one level above this sim/ directory
PROJ_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST)))/..)

# HDL sources
VERILOG_SOURCES := $(addprefix $(PROJ_DIR)/rtl/,$(shell cat $(PROJ_DIR)/sim/files.f))
# VERILOG_SOURCES := $(addprefix $(PROJ_DIR)/rtl/,$(shell cat $(PROJ_DIR)/sim/files_stub.f))

# # Top-level module name in the HDL
# # Python test module (file name without .py)

MODULE := net_core_top
TOPLEVEL := NetworkedCore

# MODULE := pixel_core_test_top
# TOPLEVEL := DigitalCore

# MODULE := pixelregion_test_top
# TOPLEVEL := PixelRegionLogic

# # known working
# MODULE := pixel_test_top
# TOPLEVEL := PixelLogic

# TOPLEVEL := LatencyMem
# MODULE := LatencyMem_test_top

# Tests live in ../tests, so add that to PYTHONPATH
export PYTHONPATH := $(PROJ_DIR)/tests:$(PYTHONPATH)


include $(shell cocotb-config --makefiles)/Makefile.sim

ifeq ($(SIM),verilator)
  VERILATOR_ARGS += --sv
  EXTRA_ARGS += --trace --trace-fst --trace-structs
  VERILATOR_ARGS += -Wno-fatal -Wno-UNOPTFLAT
endif
# "below makefile sim so no overrride"
.PRECIOUS: results.xml