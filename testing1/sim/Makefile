# testing1/sim/Makefile

SIM ?= icarus
TOPLEVEL_LANG ?= verilog

# project dir = one level above this sim/ directory
PROJ_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST)))/..)

# HDL sources
VERILOG_SOURCES := $(PROJ_DIR)/rtl/my_design.sv

# Top-level module name in the HDL
TOPLEVEL := my_design

# Python test module (file name without .py)
MODULE := test_my_design

# Tests live in ../tests, so add that to PYTHONPATH
export PYTHONPATH := $(PROJ_DIR)/tests:$(PYTHONPATH)

include $(shell cocotb-config --makefiles)/Makefile.sim
