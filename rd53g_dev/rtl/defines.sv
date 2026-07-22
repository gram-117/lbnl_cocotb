
//-----------------------------------------------------------------------------------------------------
// [Filename]       defines.sv [PACKAGE]
// [Project]        HL-LHC ATLAS/CMS pixel readout chip 
// [Author]         -
// [Language]       SystemVerilog 2012 [IEEE Std. 1800-2012]
// [Created]        Jan 30, 2017
// [Modified]       -
// [Description]    Contains all the defines used in the project.
//
// [Notes]          Removed all legacy CBA defines and CBA/DBA switches, now assumes final chip
//                  with DBA. Aurora k-codes moved into rtl/eoc/aurora/AuroraDefines.sv
//
// [Status]         devel
//-----------------------------------------------------------------------------------------------------


`ifndef RTL_PKG   // include guard
`define RTL_PKG

//
// Dependencies:
//
// n/a
//


////////////////////////////////
//   **DO NOT UNCOMMENT !**   //
////////////////////////////////

`define ATLAS_CHIP      // **IMPORTANT: only for reference, this macro is properly set by tools according to the LHC_EXPERIMENT environment variable ! KEEP COMMENTED OUT !
//`define CMS_CHIP


//////////////////////////
//   pixel-array size   //
//////////////////////////
//
// Ref. to Table 1 of the "RD53B Design Requirements" document for all details about ATLAS/CMS chips dimensions
//
// https://cdsweb.cern.ch/record/2632187
//
//
// **NOTE: Only for RTL simulations purpose, during implementation CORE_COLUMNS and CORE_ROWS
//         are set during top-level synthesis by using:
//
//         linux% make top [cols=n | cols=all] [rows=m | rows=all] [exp=ATLAS | exp=CMS]
//

// synopsys translate_off

   `ifdef ATLAS_CHIP                        // ATLAS final chip
      `define LHC_EXPERIMENT  "ATLAS"
      `define CORE_COLUMNS     50           // 50 x 8 = 400 pixel columns (20 mm padframe width)
      `define CORE_ROWS        48           // 48 x 8 = 384 pixel rows
    `endif


   `ifdef SMALLERCHIP 
        `define  ROWS 4 
   `endif

// synopsys translate_on


// **REM: each core contains 8x8 pixels
`define PIXEL_COLUMNS   (`CORE_COLUMNS)*8
`define PIXEL_ROWS         (`CORE_ROWS)*8


// number of bits required to address all core-rows
//`define CORE_ROW_ADDRESS_BITS   $clog2(`CORE_ROWS)      // **NOTE: 6-bit to address 48 (42) core-rows in ATLAS (CMS) chip
`define CORE_ROW_ADDRESS_BITS   6 // number of bits to address all cores (not just row 64)

// number of bits required to address a per-pixel Pixel Configuration Register (PCR)
`define PCR_ADDRESS_BITS   (`CORE_ROW_ADDRESS_BITS + 4 + 2)                          // **NOTE: 12-bit, 6-bit for the core + 4-bit to address the pixel-region + 2-bit pixel

// **UNUSED
//`define  TOT_BITS           4
//`define  PIXEL_REGIONS     16
//`define  PIXELS_PER_REGION  4
//`define  PIXEL_REGION_ADDRESS_BITS   $clog2(`PIXEL_REGIONS)

/////////////////////////////////////
// - Global Configuration defines  //
/////////////////////////////////////
//
// GCR_SIZE has to be the TOTAL number of registers including PIX_PORTAL
`define GCR_SIZE           256                   // There are always 256 registers. They are padded with some dummy TMR registers to always reach 256
//
`define PIX_MAGIC_NUMBER   16'b1001110011100010  // Used for Pixel default configuration
`define PIX_MAGIC_NUMBER_B 16'b0110001100011101  // ~PIX_MAGIC_NUMBER
`define GCR_MAGIC_NUMBER   16'b1010110001110101  // Used for GlobalConfiguration default values
`define GCR_MAGIC_NUMBER_B 16'b0101001110001010  // ~GCR_MAGIC_NUMBER   

/////////////////////////////////////
//   data-readout control macros   //
/////////////////////////////////////

//
// **REM: pixels are LOGICALLY organized into 1x4 pixel regions with distributed (per-pixel) ToT memories
//        and a common per-region latency buffer and trigger-matching logic/arbitration; the basic PHYSICAL
//        replica-unit layout is instead a "pixels core" of 8x8 pixels instantiating 16 pixel regions 1x4 

`define LATENCY_COUNTER_BITS   9   // max. trigger latency is 512 BX clock cycles, 12.8 us
`define TRIG_ID_BITS   8           // trigger number

`define LATENCY_MEM_DEPTH   8      // max. number of 4-bit ToT codes saved per-pixel and stored during trigger latency

`define REGION_ADDRESS_BITS   (`CORE_ROW_ADDRESS_BITS + 4)   // 6-bit core-row address + 4-bit pixel-region address to identify a pixel-region out of 16 in a single core 
`define REGION_DATA_BITS      16                             // 4-bit ToT code per-pixel x 4 pixels per-region

// NEW GRAMMY;
`define CORE_ADDR_BITS 6 // assuming 64 cores for now
// 4 magic number is region address
`define PACKET_SIZE (`CORE_ADDR_BITS + `TRIG_ID_BITS + 4 +`REGION_DATA_BITS) // 6 + 8 + 16 = 30
`define NETWORK_MEM_DEPTH 5 // tbd


////////////////////////////////////////////////////////////////////////////
//   simulation control macros (should be moved into simulation area ?)   //
////////////////////////////////////////////////////////////////////////////

// enable/disable clock jitter simulation in PLL  => waiting for Aurora RX to support jitter on PLL clock ...
//`define ADD_PLL_JITTER

`endif   // RTL_PKG
