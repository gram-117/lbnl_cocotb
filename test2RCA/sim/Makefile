# RCA/sim/Makefile

SIM ?= icarus
TOPLEVEL_LANG ?= verilog

# project dir = one level above this sim/ directory
PROJ_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST)))/..)

# HDL sources
VERILOG_SOURCES := $(addprefix $(PROJ_DIR)/rtl/,$(shell cat $(PROJ_DIR)/rtl/files.f))

# Top-level module name in the HDL
TOPLEVEL := ripple_carry_addr

# Python test module (file name without .py)
MODULE := rca_test

# Tests live in ../tests, so add that to PYTHONPATH
export PYTHONPATH := $(PROJ_DIR)/tests:$(PYTHONPATH)

include $(shell cocotb-config --makefiles)/Makefile.sim
