
//-----------------------------------------------------------------------------------------------------
// [Filename]       PixelLogic.sv [RTL]
// [Project]        HL-LHC ATLAS/CMS pixel readout chip
// [Author]         -
// [Language]       SystemVerilog 2012 [IEEE Std. 1800-2012]
// [Created]        Code derived and adapted from FE-I4 and FE65_P2 projects
// [Modified]       -
// 
// [Clock]          - PixelClkGated: Region-level gated clock activated either by HitLeAny or by PixelClkEn
// [Reset]          - Reset_b:       Synchronous active low    **WARN: This is used as an asynchronous reset
//
// [Description]    Per-pixel hit/ToT control logic and ToT buffer. The module:
//
//                     - registers the hit pulse either coming from analog front-end output or from
//                       digital injection
//                     - compute the ToT with a 6-bit ripple counter with selectable 40 MHz resolution
//                       or 80 MHz resolution (dual-edge counting)
//                     - optionally, performs the 6b/4b ToT conversion with dual-slope mapping function
//                     - read/write ToT codes to/from the per-pixel ToT memory
//
// [Notes]          ToT memory uses custom 4-bit latches. Hit registering logic derived from CBA, now
//                  uses ASYNCHRONOUS hit flags (hit_rise) to remove digital deadtime and to enable clock-gating 
//                  as soon as ToT overfow is reached (was not feasible using synchronous logic).
//
// [Status]         devel
//-----------------------------------------------------------------------------------------------------

// GRAMMY: ASSUME ATLAS USE FOR ALL NOT CMS


`ifndef PIXEL_LOGIC__SV   // include guard
`define PIXEL_LOGIC__SV

//
// Dependencies:
//
//`include "rtl/common/defines.sv"
//`include "rtl/common/CgWrapper.v"         // hard-coded clock-gating module
//`include "rtl/models/models.sv"           // for the full-custom 4-bit multi-bit latch LNQD1shrinkX4
// ^ GRAMMY: dont have access to need to make my own 

// MODULES THAT WERE REIMPLEMENTED:

// TFF from bottom of this file

// CKLNQD1 cg_cell (.TE (1'b0), .E (Enable), .CP (ClkIn), .Q(ClkOut)); from cgwrapper

// // use custom 4-bit latch            => **CHANGED** to use new cell-based multibit latch
            // LNQD1shrinkX4_V2   MultiBitLatch (

            //     .EN (       latch_en ),
            //     .D0 ( latch_input[0] ),
            //     .D1 ( latch_input[1] ),
            //     .D2 ( latch_input[2] ),
            //     .D3 ( latch_input[3] ),
            //     .Q0 (  tot_mem[k][0] ),
            //     .Q1 (  tot_mem[k][1] ),
            //     .Q2 (  tot_mem[k][2] ),
            //     .Q3 (  tot_mem[k][3] )
            
            // ) ;
//

`timescale 1ns / 1ps
//`include "timescale.v"

//
// ooooooooo.    o8o                        oooo    .oooooo.   ooooooooooooo ooooooooo.   ooooo        
// `888   `Y88.  `"'                        `888   d8P'  `Y8b  8'   888   `8 `888   `Y88. `888'        
//  888   .d88' oooo  oooo    ooo  .ooooo.   888  888               888       888   .d88'  888         
//  888ooo88P'  `888   `88b..8P'  d88' `88b  888  888               888       888ooo88P'   888         
//  888          888     Y888'    888ooo888  888  888               888       888`88b.     888         
//  888          888   .o8"'88b   888    .o  888  `88b    ooo       888       888  `88b.   888       o 
// o888o        o888o o88'   888o `Y8bod8P' o888o  `Y8bood8P'      o888o     o888o  o888o o888ooooood8 
//                                                                                                    

module PixelCTRL (

    input  wire PixelClkGated,  // region-level gated clock activated either by HitLeAny or by PixelClkEn
    input  wire Reset_b,        // global asynchronous reset, active low
    input  wire Hit,            // hit pulse, either from analog front-end or from digital injection
`ifdef CMS_CHIP
    input  wire HitSampleMode,  // hit sample mode: edge-sensitive aka "asynchronous" (0) or level-sensitive aka "synchronous" (1)
`endif
    input  wire TotOverflow,    // ToT count-overflow, used fo clock-gating
    output wire HitLe,          // hit pulse, either from analog front-end or from digital injection
    output wire HitTe,          // leading-edge of the hit pulse dectected (single pulse)
    output wire CntRst,         // reset ToT counter based on pixel state
    output wire PixelClkEn      // per-pixel clock-gating enable generated from hit (clock-gating is then common to all pixels in the pixel-region)

    ) ;


    // Input capture
    // ASSUMPTION: Hit rises before (posedge Clk) minus hold time.
    // This FF is set by the rising edge of Hit input and cleaned on first PixelClkGated

    logic hit_rise ;

    // Active-low reset for edge-detector, either from global Reset or from pixel FSM (see below)
    wire pix_rst ;

    //  Pixel state idle flag (when both hit-registering Flops outputs are 2'b00)
    wire pix_state_idle ;

    //
    // Hit-triggered registering FlipFlop.
    // Generates the hit_rise flag @(posedge Hit) starting from the analog front-end DISC output pulse
    //
    wire rst_FF ;   // reset signal for the first hit-triggered FlipFlop, active-low (changes from asynchronous vs. synchronous operating modes)

    always_ff @(posedge Hit, negedge pix_rst) begin
        if(pix_rst == 1'b0) begin
            hit_rise <= 1'b0 ;
        end
        else begin
            hit_rise <= 1'b1 ;
        end
    end   // always_ff

    //
    // Hit-sampling shift register (changes from asynchronous vs. synchronous operating modes)
    // Signal from hit is syncronized by state_sr[1]
    // Measurement phase is started only by hit_rise
    //
    logic [1:0] state_sr ;

`ifdef ATLAS_CHIP   //______________________________________________________

    // **NOTE: original code as in ItkPixV1 (asynchronous-only hit-sampling mode, no HitSampleMode configuration bit, later requested by CMS)

    always_ff @( posedge PixelClkGated ) begin
        if(~Reset_b) begin
            state_sr <= 2'b0 ;
        end
        else begin
            state_sr <= { ( hit_rise || (Hit && state_sr[1] && !TotOverflow)), state_sr[1]} ;
        end
    end   // always_ff

    assign pix_rst = Reset_b & (~state_sr[1]) ;


// CAN BE IGNORED
`elsif CMS_CHIP   //_________________________________________________________

    CG_MOD CG_rstFF (.ClkIn(PixelClkGated), .Enable (hit_rise), .ClkOut(rst_FF)) ;   

    always_ff @( posedge PixelClkGated ) begin
        if(~Reset_b) begin
            state_sr <= 2'b0 ;
        end
        else begin
            state_sr <= { ( hit_rise  && (Hit || !HitSampleMode))  || (Hit && state_sr[1] && !TotOverflow), state_sr[1]} ;
        end
    end   // always_ff

    assign pix_rst = Reset_b & (~rst_FF) & (~state_sr[1]) ;
	
`endif   //__________________________________________________________________


    //  per-region clock-gating flag
    assign PixelClkEn = hit_rise | ~Reset_b | ~pix_state_idle ;   // enable the clock for the pixel region if 1) no global reset OR 2) a hit arrives OR 3) pixel is countig ToT

    //
    //  leading-edge/trailing-edge write flags
    //
    assign HitLe = (state_sr == 2'b10);    // Active at begin of Hit pulse
    assign HitTe = (state_sr == 2'b01);    // Active at end of Hit pulse
    assign pix_state_idle = (state_sr == 2'b00);

    //
    //  generate reset signals for ToT counter
    //
    CG_MOD CG_tot_rst (.ClkIn(PixelClkGated), .Enable(~state_sr[1]), .ClkOut(CntRst));

endmodule: PixelCTRL


//
// ooooooooooooo               .     .oooooo.                                         .                      
// 8'   888   `8             .o8    d8P'  `Y8b                                      .o8                      
//      888       .ooooo.  .o888oo 888           .ooooo.  oooo  oooo  ooo. .oo.   .o888oo  .ooooo.  oooo d8b 
//      888      d88' `88b   888   888          d88' `88b `888  `888  `888P"Y88b    888   d88' `88b `888""8P 
//      888      888   888   888   888          888   888  888   888   888   888    888   888ooo888  888     
//      888      888   888   888 . `88b    ooo  888   888  888   888   888   888    888 . 888    .o  888     
//     o888o     `Y8bod8P'   "888"  `Y8bood8P'  `Y8bod8P'  `V88V"V8P' o888o o888o   "888" `Y8bod8P' d888b    
//                                                                                                          
module TotCounter (

    input  wire PixelClkGated,
    input  wire Hit,
    input  wire HitTe,
    input  wire CntRst,
    input  wire Reset_b, // added
    input  wire TotDualEdgeCount,
    input  wire Tot6to4Mapping,
    output wire [3:0] TotCnt,
    output wire TotOverflow

    ) ;

    // Counter reset - Active low
    wire cnt_rst_b;
    assign cnt_rst_b = ~CntRst;

    // Counter clock gating. Output clock level is hold on falling adge of Disc
    reg cnt_clk; // ISSUE WITH COMB LOGIC LOOP DEPENCENCY!!!!!!!!

    // MODIFIED TO ADD RST
    always_latch begin
    if (!Reset_b) begin
        cnt_clk = 1'b0; // changed to <- inside always comb lbock update back to = bcs verilator
    end
    else if(Hit && !TotOverflow && !HitTe) begin 
           cnt_clk = PixelClkGated;
    end
    end
    // ^^ was originally LHQD2 cnt clock latch instead using given code !!!!!
    //LHQD1 cnt_clk_latch (.E(Hit && !TotOverflow && !HitTe), .D(PixelClkGated), .Q(cnt_clk) ) ;
    //LHQD2 cnt_clk_latch (.E(Hit && !TotOverflow && !HitTe), .D(PixelClkGated), .Q(cnt_clk) ) ;      // fix residual max_cap/max_tran DRVs

    // 6-bits counter
    // in dual edge mode input clock is forwarded to bit 1 FF becouse output bit 0 comes from Disc falling edge phase
    wire [5:0]  init_tot_cnt, rcnt_q, rcnt_qn;

    wire ff1_clk;

    // **BUG: wrong ToT for short hits in asynchronous sampling mode and 80 MHz counting (ref. to https://gitlab.cern.ch/rd53/RD53B/-/issues/347)
    //assign ff1_clk = TotDualEdgeCount ? ~cnt_clk  : rcnt_q[0];

    // **FIX [Flavio]
    assign ff1_clk = TotDualEdgeCount ? cnt_clk  : rcnt_q[0] ;


    // Ripple counter
    TFF_NCLK_NRST ff0(.q(rcnt_q[0]), .qn(rcnt_qn[0]), .nclk(~cnt_clk),  .nrst(cnt_rst_b));
    TFF_NCLK_NRST ff1(.q(rcnt_q[1]), .qn(rcnt_qn[1]), .nclk(ff1_clk),   .nrst(cnt_rst_b));
    TFF_NCLK_NRST ff2(.q(rcnt_q[2]), .qn(rcnt_qn[2]), .nclk(rcnt_q[1]), .nrst(cnt_rst_b));
    TFF_NCLK_NRST ff3(.q(rcnt_q[3]), .qn(rcnt_qn[3]), .nclk(rcnt_q[2]), .nrst(cnt_rst_b));
    TFF_NCLK_NRST ff4(.q(rcnt_q[4]), .qn(rcnt_qn[4]), .nclk(rcnt_q[3]), .nrst(cnt_rst_b));
    SFF_NCLK_NRST ff5(.q(rcnt_q[5]), .qn(rcnt_qn[5]), .nclk(rcnt_q[4]), .nrst(cnt_rst_b));

    assign init_tot_cnt = ~rcnt_qn;

    // Disc falling edge phase
    wire fall_phase;

    // **BUG: wrong ToT for short hits in asynchronous sampling mode and 80 MHz counting (ref. to https://gitlab.cern.ch/rd53/RD53B/-/issues/347)
    //assign fall_phase = ~cnt_clk ;

    // **FIX [Flavio]
    assign fall_phase = cnt_clk ;

    // Output of counter (stage one)
    // in dual edge mode bit 0 is the Disc falling edge phase
    wire [5:0] tot_cnt_stageone;
    // modify for verilator. tacking on LSB later
    // assign tot_cnt_stageone = {init_tot_cnt[5:1], (TotDualEdgeCount ? fall_phase : init_tot_cnt[0]) };
    assign tot_cnt_stageone = {init_tot_cnt[5:1], 1'b0}; // temp 0


    // icarus is complaining about this part: TRYING TO REPLACE
    //sorry: constant selects in always_* processes are not currently supported (all bits will be included).
    // 6-to-4 bit converter
    reg [3:0] tot_cnt_int; // use just for tot overflow, lsb doesnt matter
    reg [3:0] with_fall_phase;  // seperate for tot out
    always_comb begin // replaced * with comb
        if(|tot_cnt_stageone[5:3] && Tot6to4Mapping) begin// or all bits in stgone %% if 6to4enable
            tot_cnt_int[3:0] = {1'b1, 3'(tot_cnt_stageone[5:2] - 4'h2)};
            // this looks like shit but its verilators fault account for falling phase lsb
            if (fall_phase) begin
                with_fall_phase = {1'b1, 3'(tot_cnt_stageone[5:2] - 4'h1)}; 
            end
            else begin
                with_fall_phase = {1'b1, 3'(tot_cnt_stageone[5:2] - 4'h2)}; 
            end
        end
        else begin
            tot_cnt_int[3:0] = tot_cnt_stageone[3:0];
            with_fall_phase = {tot_cnt_stageone[3:1], 1'b1}; // holy shit *** take a look
        end
    end

    

    // Overflow detector
    assign TotOverflow = (tot_cnt_int[3:1] == 3'b111); // x=='d14 || x=='d15

    // exclude 0x0f from output encoding: Reserved for "No data present" code.

    assign TotCnt = (&(with_fall_phase[3:1]) ? {with_fall_phase[3:1], 1'b0} : 
            with_fall_phase);
    // ^ if all ones then make the last 1 0
    
endmodule: TotCounter

//
// ooooooooooooo               .   ooo        ooooo                                                            
// 8'   888   `8             .o8   `88.       .888'                                                            
//      888       .ooooo.  .o888oo  888b     d'888   .ooooo.  ooo. .oo.  .oo.    .ooooo.  oooo d8b oooo    ooo 
//      888      d88' `88b   888    8 Y88. .P  888  d88' `88b `888P"Y88bP"Y88b  d88' `88b `888""8P  `88.  .8'  
//      888      888   888   888    8  `888'   888  888ooo888  888   888   888  888   888  888       `88..8'   
//      888      888   888   888 .  8    Y     888  888    .o  888   888   888  888   888  888        `888'    
//     o888o     `Y8bod8P'   "888" o8o        o888o `Y8bod8P' o888o o888o o888o `Y8bod8P' d888b        .8'     
//                                                                                                 .o..P'      
//                                                                                                 `Y8P'       
//                                                                                                            
module TotMemory (

    input  wire PixelClkGated,        
    input  wire HitLe,
    input  wire HitTe,
    input  wire HitLeAny,
    input  wire [`LATENCY_MEM_DEPTH-1:0] TotMemWriteAddr,
    input  wire [`LATENCY_MEM_DEPTH-1:0] TotMemReadAddr,
    input  wire [3:0] TotMemDataIn,
    output wire [3:0] TotMemDataOut

    ) ;
    
       
    //
    // register the write pointer into a bank of FlipFlops
    //
    logic [`LATENCY_MEM_DEPTH-1:0] tot_mem_write_addr_reg ;

    always_ff @ (posedge PixelClkGated) begin
        if(HitLe) begin
            tot_mem_write_addr_reg[`LATENCY_MEM_DEPTH-1:0] <= TotMemWriteAddr[`LATENCY_MEM_DEPTH-1:0] ;
        end
    end

    //
    // ToT memory using optimized custom 4-bit latches
    //
    logic [3:0] tot_mem [`LATENCY_MEM_DEPTH-1:0] ;

    //////////////////////////////////////
    //   per-pixel ToT latency memory   //
    //////////////////////////////////////    
    // REASON MEM TAkes SO LONG TO UPDATE: 
    // latch condition: needs TE to go high (sync) then clock to go low (falling edge)
    generate
        genvar k;
        for (k=0; k<`LATENCY_MEM_DEPTH; k=k+1)  begin : TotMem

            wire latch_empty_hit ;
            assign latch_empty_hit = TotMemWriteAddr[k] & HitLeAny & (~HitLe) ;   // no hit information to save (write address and HitLeAny from pixel-region but no hit in this pixel)

            wire latch_hit ;
            assign latch_hit = tot_mem_write_addr_reg[k] & HitTe ;

            wire [3:0] latch_input ;
            assign latch_input[3:0] = (latch_hit == 1'b1) ? TotMemDataIn[3:0] : 4'd15 ;   // to latch empty hit (the ToT counter resets at 0, already a valid ToT value)

            wire latch_condition ;
            assign latch_condition = latch_hit | latch_empty_hit ;

            wire latch_en ;
            assign latch_en = PixelClkGated | ~latch_condition ;   // **WARN: negative-level sensitive !

            // use custom 4-bit latch            => **CHANGED** to use new cell-based multibit latch
            LNQD1shrinkX4_V2   MultiBitLatch (

                .EN (      latch_en ), // active low? needs clock to be low, HitTe
                .D0 ( latch_input[0] ),
                .D1 ( latch_input[1] ),
                .D2 ( latch_input[2] ),
                .D3 ( latch_input[3] ),
                .Q0 (  tot_mem[k][0] ),
                .Q1 (  tot_mem[k][1] ),
                .Q2 (  tot_mem[k][2] ),
                .Q3 (  tot_mem[k][3] )
            
            ) ;

        end : TotMem
    endgenerate

    //////////////////////
    //   data readout   //
    //////////////////////

    // wire [3:0] tot_mem_out [`LATENCY_MEM_DEPTH-1:0] ;

    // generate
    //    genvar m ;

    //    for(m = 0; m < `LATENCY_MEM_DEPTH; m++) begin : TotMemOut

    //       if(m == 0)
    //          assign tot_mem_out[m] = tot_mem[m] & {4{TotMemReadAddr[m]}} ;
    //       else
    //          assign tot_mem_out[m] = ( tot_mem[m] & {4{TotMemReadAddr[m]}} ) | tot_mem_out[m-1] ;

    //    end
    // endgenerate 
    
    // // output data
    // assign TotMemDataOut[3:0] = tot_mem_out[`LATENCY_MEM_DEPTH-1][3:0] ;

    // ^^ replicate the above code
    logic [3:0] tot_mem_dataout;

    always_comb begin
    tot_mem_dataout = 4'b0;
    for (int m = 0; m < `LATENCY_MEM_DEPTH; m++) begin
        tot_mem_dataout |= (tot_mem[m] & {4{TotMemReadAddr[m]}});
    end
    end

    assign TotMemDataOut[3:0] = tot_mem_dataout;
        
endmodule: TotMemory

//
// ooooooooo.    o8o                        oooo  ooooo                              o8o            
// `888   `Y88.  `"'                        `888  `888'                              `"'            
//  888   .d88' oooo  oooo    ooo  .ooooo.   888   888          .ooooo.   .oooooooo oooo   .ooooo.  
//  888ooo88P'  `888   `88b..8P'  d88' `88b  888   888         d88' `88b 888' `88b  `888  d88' `"Y8 
//  888          888     Y888'    888ooo888  888   888         888   888 888   888   888  888       
//  888          888   .o8"'88b   888    .o  888   888       o 888   888 `88bod8P'   888  888   .o8 
// o888o        o888o o88'   888o `Y8bod8P' o888o o888ooooood8 `Y8bod8P' `8oooooo.  o888o `Y8bod8P' 
//                                                                       d"     YD                  
//                                                                       "Y88888P'                  
//                                                                                                 
module PixelLogic (
    // clock and reset
    output logic PixelClkEn,                                // per-pixel clock-gating enable generated either from hit (clock-gating is then common to all pixels in the pixel-region)
    input  wire PixelClkGated,                              // region-level gated clock activated either by HitLeAny or by PixelClkEn
    input  wire Reset_b,                                    // global asynchronous reset, active low

    // global configuration
    input  wire Tot6to4Mapping,                             // enable/disable 6b/4b ToT encoding with dual-slope mapping
    input  wire TotDualEdgeCount,                           // enable/disable ToT counting at 80 MHz using dual-edge 

// `ifdef CMS_CHIP
//     input  wire HitSampleMode,                              // choose hit sampling mode between edge-sensitive aka "asynchronous" (0) and level-sensitive aka "synchronous" (1)
// `endif

    // hit/ToT control logic
    input  wire Hit,                                        // hit pulse, either from analog front-end or from digital injection
    output wire HitLe,                                      // leading-edge of the hit pulse dectected (single pulse)
    input  wire HitLeAny,                                   // OR of HitLe flags from all 1x4 pixels, generated in the 1x4 pixel-region, used to enable clock-gating
 
    // write/read to/from ToT memory (LATENCY_MEM currently set to 8!!!!)
    input  wire [`LATENCY_MEM_DEPTH-1:0] TotMemWriteAddr,   // write pointer from the common latency buffer: in which memory slot the ToT must be saved (same for all pixels)
    input  wire [`LATENCY_MEM_DEPTH-1:0] TotMemReadAddr,    // read pointer from the common latency buffer: from which memory location ToT is retrieved when a trigger arrives (same for all pixels)
    output wire [3:0] TotMemDataOut                         // 4-bit ToT value extracted from ToT latency memory
    ) ;
    
    ///////////////////////////////////////////////////////////////
    //            **ASYNCHRONOUS** hit registering logic
    ///////////////////////////////////////////////////////////////
    //                    _____________________________
    //   ________________/                             \___________   Hit from discriminator
    //                    _
    //   ________________/ \_______________________________________   hit_rise => set @(posedge Hit), clear on first PixelClkGated rise edge
    //                    _________________________________
    //   ________________/                                 \_______   PixelClkEn  => asynch. asserted @(posedge Hit) but synch. de-asserted @(posedge PixelClkGated) 
    //                      _   _   _   _   _   _   _   _   _
    //   __________________/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_____   PixelClkGated
    //                      ____
    //   __________________/    \__________________________________   HitLe => synch. one-clock-pulse
    //                                                  ___
    //   ______________________________________________/   \_______   HitTe => synch. one-clock-pulse
    //   __________________                             ___________
    //                     \___________________________/              pix_rst => synch.
    //                      _   _   _   _   _   _   _   
    //   __________________/ \_/ \_/ \_/ \_/ \_/ \_/ \_____________   cnt_clk = PixelClkGated latched by (Hit && !TotOverflow)

    wire tot_overflow;
    wire HitTe;
    wire CntRst;

    PixelCTRL PixelCTRL(

        .PixelClkGated ( PixelClkGated ),
        .Reset_b       (       Reset_b ),
        .Hit           (           Hit ),
    `ifdef CMS_CHIP
        .HitSampleMode ( HitSampleMode ),
    `endif
        .TotOverflow   (  tot_overflow ),
        .HitLe         (         HitLe ),
        .HitTe         (         HitTe ),
        .CntRst        (        CntRst ),
        .PixelClkEn    (    PixelClkEn )        
    ) ; 

 
    /////////////////////////////////////////////////////////////////////////////////////
    //   ToT counter with selectable dual-edge counting and 6b/4b dual-slop encoding   // 
    /////////////////////////////////////////////////////////////////////////////////////
    wire [3:0] tot_cnt ;
    
    TotCounter   TotCounter (

        .PixelClkGated    (    PixelClkGated ),
        .Hit              (              Hit ),
        .HitTe            (            HitTe ),
        .CntRst           (           CntRst ),
        .Reset_b          (           Reset_b),
        .TotDualEdgeCount ( TotDualEdgeCount ),
        .Tot6to4Mapping   (   Tot6to4Mapping ),
        .TotCnt           (     tot_cnt[3:0] ),
        .TotOverflow      (     tot_overflow )
    ) ;

    //////////////////////////////////////
    //   per-pixel ToT latency memory   //
    //////////////////////////////////////

    TotMemory   TotMemory (
        .PixelClkGated   (                           PixelClkGated ),
        .HitLe           (                                   HitLe ),
        .HitTe           (                                   HitTe ), 
        .HitLeAny        (                                HitLeAny ),
        .TotMemWriteAddr ( TotMemWriteAddr[`LATENCY_MEM_DEPTH-1:0] ),
        .TotMemReadAddr  (  TotMemReadAddr[`LATENCY_MEM_DEPTH-1:0] ),
        .TotMemDataIn    (                            tot_cnt[3:0] ),
        .TotMemDataOut   (                      TotMemDataOut[3:0] )
    ) ;

endmodule : PixelLogic

// //////////////////////////////////////
// //   custom Toggle FlipFlop (TFF)   //
// //////////////////////////////////////
// module TFF_NCLK_NRST (
//     input  wire nclk,   // negative edge-triggered FF
//     input  wire nrst,   // asynchronous reset, active-low
//     output logic q,
//     output logic qn
//     ) ;

//     DFNCND1 q_reg (.CDN(nrst), .CPN(nclk), .D(qn), .Q(q), .QN(qn));

//    // synopsys dc_script_begin
//    // set_dont_touch q
//    // synopsys dc_script_end

//     /*always_ff @(negedge nclk or negedge nrst) begin
//         if(~nrst)
//             q <= 1'b0 ;
//         else
//             q <= !q ;
//     end
//     *
//      endmodule : TFF_NCLK_NRST

//////////////////////////////////////
//   custom Set 1 FlipFlop (SFF)   //
//////////////////////////////////////
module SFF_NCLK_NRST(
    input wire nclk, // negative edge-triggered FF
    input wire nrst, // asynchronous reset, active-low
    output logic q,
    output logic qn
    );

    assign qn = !q;
    
    always @ (negedge nclk or negedge nrst)
        if(~nrst)
            q <= 1'b0;
        else
            q <= 1;
endmodule


`endif   // PIXEL_LOGIC__SV
