#!/bin/bash
# run_sim.sh

set -e  # exit on any error


make WAVES=1
gtkwave sim_build/PixelRegionLogic.fst