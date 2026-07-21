module DigitalCoreStub (
    // Hits from testbench
    input  wire [63:0] AnaHit,

    // DIFF front-end
    inout  wire [7:0] VBP_PREAMP_A,
    inout  wire [7:0] VBP_PREAMP_B,
    inout  wire [7:0] VBN_COMP,
    inout  wire [7:0] VBN_PRECOMP,
    inout  wire [7:0] VTH1,
    inout  wire [7:0] VTH2,
    inout  wire [7:0] VBN_LCC,
    inout  wire [7:0] VBP_VFF,
    inout  wire [7:0] VCTRL_CF0,
    inout  wire [7:0] VCTRL_LCC,
    inout  wire [7:0] VCAL_HI,
    inout  wire [7:0] VCAL_MI,

    // Clock and reset
    input  wire ClkIn,
    output wire ClkOut,

    input  wire ResetIn_b,
    output wire ResetOut_b,

    // Configuration
    input  wire [5:0] CoreRowAddrIn,
    output wire [5:0] CoreRowAddrOut,

    // Calibration
    input  wire CalEdgeIn,
    output wire CalEdgeOut,

    input  wire CalAuxIn,
    output wire CalAuxOut,

    input  wire [`LATENCY_COUNTER_BITS-1:0] LatCntIn,
    output wire [`LATENCY_COUNTER_BITS-1:0] LatCntOut,
    input  wire [`LATENCY_COUNTER_BITS-1:0] LatCntReqIn,
    output wire [`LATENCY_COUNTER_BITS-1:0] LatCntReqOut,

    input  wire TrigIn,
    output wire TrigOut,

    input  wire TrigClearIn,
    output wire TrigClearOut,

    output wire [`REGION_DATA_BITS-1:0] RegionDataTrig,
    output wire [`REGION_DATA_BITS-1:0] RegionDataOut
);


endmodule