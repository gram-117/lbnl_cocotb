// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vtop.h for the primary calling header

#ifndef VERILATED_VTOP___024ROOT_H_
#define VERILATED_VTOP___024ROOT_H_  // guard

#include "verilated.h"


class Vtop__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vtop___024root final {
  public:

    // DESIGN SPECIFIC STATE
    // Anonymous structures to workaround compiler member-count bugs
    struct {
        VL_IN8(PixelClkGated,0,0);
        VL_IN8(Reset_b,0,0);
        VL_IN8(Tot6to4Mapping,0,0);
        VL_IN8(TotDualEdgeCount,0,0);
        VL_IN8(Hit,0,0);
        VL_IN8(HitLeAny,0,0);
        VL_OUT8(PixelClkEn,0,0);
        VL_OUT8(HitLe,0,0);
        VL_IN8(TotMemWriteAddr,7,0);
        VL_IN8(TotMemReadAddr,7,0);
        VL_OUT8(TotMemDataOut,3,0);
        CData/*0:0*/ PixelLogic__DOT__PixelClkEn;
        CData/*0:0*/ PixelLogic__DOT__PixelClkGated;
        CData/*0:0*/ PixelLogic__DOT__Reset_b;
        CData/*0:0*/ PixelLogic__DOT__Tot6to4Mapping;
        CData/*0:0*/ PixelLogic__DOT__TotDualEdgeCount;
        CData/*0:0*/ PixelLogic__DOT__Hit;
        CData/*0:0*/ PixelLogic__DOT__HitLe;
        CData/*0:0*/ PixelLogic__DOT__HitLeAny;
        CData/*7:0*/ PixelLogic__DOT__TotMemWriteAddr;
        CData/*7:0*/ PixelLogic__DOT__TotMemReadAddr;
        CData/*3:0*/ PixelLogic__DOT__TotMemDataOut;
        CData/*0:0*/ PixelLogic__DOT__tot_overflow;
        CData/*0:0*/ PixelLogic__DOT__HitTe;
        CData/*0:0*/ PixelLogic__DOT__CntRst;
        CData/*3:0*/ PixelLogic__DOT__tot_cnt;
        CData/*0:0*/ PixelLogic__DOT__PixelCTRL__DOT__PixelClkGated;
        CData/*0:0*/ PixelLogic__DOT__PixelCTRL__DOT__Reset_b;
        CData/*0:0*/ PixelLogic__DOT__PixelCTRL__DOT__Hit;
        CData/*0:0*/ PixelLogic__DOT__PixelCTRL__DOT__TotOverflow;
        CData/*0:0*/ PixelLogic__DOT__PixelCTRL__DOT__HitLe;
        CData/*0:0*/ PixelLogic__DOT__PixelCTRL__DOT__HitTe;
        CData/*0:0*/ PixelLogic__DOT__PixelCTRL__DOT__CntRst;
        CData/*0:0*/ PixelLogic__DOT__PixelCTRL__DOT__PixelClkEn;
        CData/*0:0*/ PixelLogic__DOT__PixelCTRL__DOT__hit_rise;
        CData/*0:0*/ PixelLogic__DOT__PixelCTRL__DOT__pix_rst;
        CData/*0:0*/ PixelLogic__DOT__PixelCTRL__DOT__pix_state_idle;
        CData/*0:0*/ PixelLogic__DOT__PixelCTRL__DOT__rst_FF;
        CData/*1:0*/ PixelLogic__DOT__PixelCTRL__DOT__state_sr;
        CData/*0:0*/ PixelLogic__DOT__PixelCTRL__DOT__CG_tot_rst__DOT__ClkIn;
        CData/*0:0*/ PixelLogic__DOT__PixelCTRL__DOT__CG_tot_rst__DOT__Enable;
        CData/*0:0*/ PixelLogic__DOT__PixelCTRL__DOT__CG_tot_rst__DOT__ClkOut;
        CData/*0:0*/ PixelLogic__DOT__PixelCTRL__DOT__CG_tot_rst__DOT__ck_inb;
        CData/*0:0*/ PixelLogic__DOT__PixelCTRL__DOT__CG_tot_rst__DOT__enl;
        CData/*0:0*/ PixelLogic__DOT__TotCounter__DOT__PixelClkGated;
        CData/*0:0*/ PixelLogic__DOT__TotCounter__DOT__Hit;
        CData/*0:0*/ PixelLogic__DOT__TotCounter__DOT__HitTe;
        CData/*0:0*/ PixelLogic__DOT__TotCounter__DOT__CntRst;
        CData/*0:0*/ PixelLogic__DOT__TotCounter__DOT__Reset_b;
        CData/*0:0*/ PixelLogic__DOT__TotCounter__DOT__TotDualEdgeCount;
        CData/*0:0*/ PixelLogic__DOT__TotCounter__DOT__Tot6to4Mapping;
        CData/*3:0*/ PixelLogic__DOT__TotCounter__DOT__TotCnt;
        CData/*0:0*/ PixelLogic__DOT__TotCounter__DOT__TotOverflow;
        CData/*0:0*/ PixelLogic__DOT__TotCounter__DOT__cnt_rst_b;
        CData/*0:0*/ PixelLogic__DOT__TotCounter__DOT__cnt_clk;
        CData/*5:0*/ PixelLogic__DOT__TotCounter__DOT__init_tot_cnt;
        CData/*5:0*/ PixelLogic__DOT__TotCounter__DOT__rcnt_q;
        CData/*5:0*/ PixelLogic__DOT__TotCounter__DOT__rcnt_qn;
        CData/*0:0*/ PixelLogic__DOT__TotCounter__DOT__ff1_clk;
        CData/*0:0*/ PixelLogic__DOT__TotCounter__DOT__fall_phase;
        CData/*5:0*/ PixelLogic__DOT__TotCounter__DOT__tot_cnt_stageone;
        CData/*3:0*/ PixelLogic__DOT__TotCounter__DOT__tot_cnt_int;
        CData/*0:0*/ PixelLogic__DOT__TotCounter__DOT__ff0__DOT__nclk;
        CData/*0:0*/ PixelLogic__DOT__TotCounter__DOT__ff0__DOT__nrst;
    };
    struct {
        CData/*0:0*/ PixelLogic__DOT__TotCounter__DOT__ff0__DOT__q;
        CData/*0:0*/ PixelLogic__DOT__TotCounter__DOT__ff0__DOT__qn;
        CData/*0:0*/ PixelLogic__DOT__TotCounter__DOT__ff1__DOT__nclk;
        CData/*0:0*/ PixelLogic__DOT__TotCounter__DOT__ff1__DOT__nrst;
        CData/*0:0*/ PixelLogic__DOT__TotCounter__DOT__ff1__DOT__q;
        CData/*0:0*/ PixelLogic__DOT__TotCounter__DOT__ff1__DOT__qn;
        CData/*0:0*/ PixelLogic__DOT__TotCounter__DOT__ff2__DOT__nclk;
        CData/*0:0*/ PixelLogic__DOT__TotCounter__DOT__ff2__DOT__nrst;
        CData/*0:0*/ PixelLogic__DOT__TotCounter__DOT__ff2__DOT__q;
        CData/*0:0*/ PixelLogic__DOT__TotCounter__DOT__ff2__DOT__qn;
        CData/*0:0*/ PixelLogic__DOT__TotCounter__DOT__ff3__DOT__nclk;
        CData/*0:0*/ PixelLogic__DOT__TotCounter__DOT__ff3__DOT__nrst;
        CData/*0:0*/ PixelLogic__DOT__TotCounter__DOT__ff3__DOT__q;
        CData/*0:0*/ PixelLogic__DOT__TotCounter__DOT__ff3__DOT__qn;
        CData/*0:0*/ PixelLogic__DOT__TotCounter__DOT__ff4__DOT__nclk;
        CData/*0:0*/ PixelLogic__DOT__TotCounter__DOT__ff4__DOT__nrst;
        CData/*0:0*/ PixelLogic__DOT__TotCounter__DOT__ff4__DOT__q;
        CData/*0:0*/ PixelLogic__DOT__TotCounter__DOT__ff4__DOT__qn;
        CData/*0:0*/ PixelLogic__DOT__TotCounter__DOT__ff5__DOT__nclk;
        CData/*0:0*/ PixelLogic__DOT__TotCounter__DOT__ff5__DOT__nrst;
        CData/*0:0*/ PixelLogic__DOT__TotCounter__DOT__ff5__DOT__q;
        CData/*0:0*/ PixelLogic__DOT__TotCounter__DOT__ff5__DOT__qn;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__PixelClkGated;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__HitLe;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__HitTe;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__HitLeAny;
        CData/*7:0*/ PixelLogic__DOT__TotMemory__DOT__TotMemWriteAddr;
        CData/*7:0*/ PixelLogic__DOT__TotMemory__DOT__TotMemReadAddr;
        CData/*3:0*/ PixelLogic__DOT__TotMemory__DOT__TotMemDataIn;
        CData/*3:0*/ PixelLogic__DOT__TotMemory__DOT__TotMemDataOut;
        CData/*7:0*/ PixelLogic__DOT__TotMemory__DOT__tot_mem_write_addr_reg;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__latch_empty_hit;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__latch_hit;
        CData/*3:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__latch_input;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__latch_condition;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__latch_en;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__latch_empty_hit;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__latch_hit;
        CData/*3:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__latch_input;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__latch_condition;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__latch_en;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__latch_empty_hit;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__latch_hit;
        CData/*3:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__latch_input;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__latch_condition;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__latch_en;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__latch_empty_hit;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__latch_hit;
        CData/*3:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__latch_input;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__latch_condition;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__latch_en;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__latch_empty_hit;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__latch_hit;
        CData/*3:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__latch_input;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__latch_condition;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__latch_en;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__latch_empty_hit;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__latch_hit;
        CData/*3:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__latch_input;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__latch_condition;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__latch_en;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__latch_empty_hit;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__latch_hit;
        CData/*3:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__latch_input;
    };
    struct {
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__latch_condition;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__latch_en;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__latch_empty_hit;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__latch_hit;
        CData/*3:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__latch_input;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__latch_condition;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__latch_en;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__MultiBitLatch__DOT__EN;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__MultiBitLatch__DOT__D0;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__MultiBitLatch__DOT__D1;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__MultiBitLatch__DOT__D2;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__MultiBitLatch__DOT__D3;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__MultiBitLatch__DOT__Q0;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__MultiBitLatch__DOT__Q1;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__MultiBitLatch__DOT__Q2;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__MultiBitLatch__DOT__Q3;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__MultiBitLatch__DOT__EN;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__MultiBitLatch__DOT__D0;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__MultiBitLatch__DOT__D1;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__MultiBitLatch__DOT__D2;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__MultiBitLatch__DOT__D3;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__MultiBitLatch__DOT__Q0;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__MultiBitLatch__DOT__Q1;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__MultiBitLatch__DOT__Q2;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__MultiBitLatch__DOT__Q3;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__MultiBitLatch__DOT__EN;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__MultiBitLatch__DOT__D0;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__MultiBitLatch__DOT__D1;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__MultiBitLatch__DOT__D2;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__MultiBitLatch__DOT__D3;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__MultiBitLatch__DOT__Q0;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__MultiBitLatch__DOT__Q1;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__MultiBitLatch__DOT__Q2;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__MultiBitLatch__DOT__Q3;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__MultiBitLatch__DOT__EN;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__MultiBitLatch__DOT__D0;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__MultiBitLatch__DOT__D1;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__MultiBitLatch__DOT__D2;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__MultiBitLatch__DOT__D3;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__MultiBitLatch__DOT__Q0;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__MultiBitLatch__DOT__Q1;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__MultiBitLatch__DOT__Q2;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__MultiBitLatch__DOT__Q3;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__MultiBitLatch__DOT__EN;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__MultiBitLatch__DOT__D0;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__MultiBitLatch__DOT__D1;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__MultiBitLatch__DOT__D2;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__MultiBitLatch__DOT__D3;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__MultiBitLatch__DOT__Q0;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__MultiBitLatch__DOT__Q1;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__MultiBitLatch__DOT__Q2;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__MultiBitLatch__DOT__Q3;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__MultiBitLatch__DOT__EN;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__MultiBitLatch__DOT__D0;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__MultiBitLatch__DOT__D1;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__MultiBitLatch__DOT__D2;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__MultiBitLatch__DOT__D3;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__MultiBitLatch__DOT__Q0;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__MultiBitLatch__DOT__Q1;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__MultiBitLatch__DOT__Q2;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__MultiBitLatch__DOT__Q3;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__MultiBitLatch__DOT__EN;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__MultiBitLatch__DOT__D0;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__MultiBitLatch__DOT__D1;
    };
    struct {
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__MultiBitLatch__DOT__D2;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__MultiBitLatch__DOT__D3;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__MultiBitLatch__DOT__Q0;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__MultiBitLatch__DOT__Q1;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__MultiBitLatch__DOT__Q2;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__MultiBitLatch__DOT__Q3;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__MultiBitLatch__DOT__EN;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__MultiBitLatch__DOT__D0;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__MultiBitLatch__DOT__D1;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__MultiBitLatch__DOT__D2;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__MultiBitLatch__DOT__D3;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__MultiBitLatch__DOT__Q0;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__MultiBitLatch__DOT__Q1;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__MultiBitLatch__DOT__Q2;
        CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__MultiBitLatch__DOT__Q3;
        CData/*0:0*/ __Vtrigprevexpr___TOP__PixelLogic__DOT__TotCounter__DOT__fall_phase__0;
        CData/*0:0*/ __VstlDidInit;
        CData/*0:0*/ __VstlFirstIteration;
        CData/*0:0*/ __VstlPhaseResult;
        CData/*0:0*/ __Vtrigprevexpr___TOP__PixelLogic__DOT__TotCounter__DOT__fall_phase__1;
        CData/*0:0*/ __VicoDidInit;
        CData/*0:0*/ __VicoFirstIteration;
        CData/*0:0*/ __VicoPhaseResult;
        CData/*0:0*/ __Vtrigprevexpr___TOP__PixelLogic__DOT__TotCounter__DOT__fall_phase__2;
        CData/*0:0*/ __Vtrigprevexpr___TOP__PixelLogic__DOT__PixelCTRL__DOT__Hit__0;
        CData/*0:0*/ __Vtrigprevexpr___TOP__PixelLogic__DOT__PixelCTRL__DOT__pix_rst__0;
        CData/*0:0*/ __Vtrigprevexpr___TOP__PixelLogic__DOT__PixelCTRL__DOT__PixelClkGated__0;
        CData/*0:0*/ __Vtrigprevexpr___TOP__PixelLogic__DOT__TotCounter__DOT__ff0__DOT__nclk__0;
        CData/*0:0*/ __Vtrigprevexpr___TOP__PixelLogic__DOT__TotCounter__DOT__ff0__DOT__nrst__0;
        CData/*0:0*/ __Vtrigprevexpr___TOP__PixelLogic__DOT__TotCounter__DOT__ff1__DOT__nclk__0;
        CData/*0:0*/ __Vtrigprevexpr___TOP__PixelLogic__DOT__TotCounter__DOT__ff1__DOT__nrst__0;
        CData/*0:0*/ __Vtrigprevexpr___TOP__PixelLogic__DOT__TotCounter__DOT__ff2__DOT__nclk__0;
        CData/*0:0*/ __Vtrigprevexpr___TOP__PixelLogic__DOT__TotCounter__DOT__ff2__DOT__nrst__0;
        CData/*0:0*/ __Vtrigprevexpr___TOP__PixelLogic__DOT__TotCounter__DOT__ff3__DOT__nclk__0;
        CData/*0:0*/ __Vtrigprevexpr___TOP__PixelLogic__DOT__TotCounter__DOT__ff3__DOT__nrst__0;
        CData/*0:0*/ __Vtrigprevexpr___TOP__PixelLogic__DOT__TotCounter__DOT__ff4__DOT__nclk__0;
        CData/*0:0*/ __Vtrigprevexpr___TOP__PixelLogic__DOT__TotCounter__DOT__ff4__DOT__nrst__0;
        CData/*0:0*/ __Vtrigprevexpr___TOP__PixelLogic__DOT__TotCounter__DOT__ff5__DOT__nclk__0;
        CData/*0:0*/ __Vtrigprevexpr___TOP__PixelLogic__DOT__TotCounter__DOT__ff5__DOT__nrst__0;
        CData/*0:0*/ __Vtrigprevexpr___TOP__PixelLogic__DOT__TotMemory__DOT__PixelClkGated__0;
        CData/*0:0*/ __VactDidInit;
        CData/*0:0*/ __VactPhaseResult;
        CData/*0:0*/ __VnbaPhaseResult;
        IData/*31:0*/ __VactIterCount;
        VlUnpacked<CData/*3:0*/, 8> PixelLogic__DOT__TotMemory__DOT__tot_mem;
        VlUnpacked<CData/*3:0*/, 8> PixelLogic__DOT__TotMemory__DOT__tot_mem_out;
        VlUnpacked<QData/*63:0*/, 2> __VstlTriggered;
        VlUnpacked<CData/*3:0*/, 8> __Vtrigprevexpr___TOP__PixelLogic__DOT__TotMemory__DOT__tot_mem_out__0;
        VlUnpacked<QData/*63:0*/, 2> __VicoTriggered;
        VlUnpacked<CData/*3:0*/, 8> __Vtrigprevexpr___TOP__PixelLogic__DOT__TotMemory__DOT__tot_mem_out__1;
        VlUnpacked<QData/*63:0*/, 1> __VactTriggered;
        VlUnpacked<CData/*3:0*/, 8> __Vtrigprevexpr___TOP__PixelLogic__DOT__TotMemory__DOT__tot_mem_out__2;
        VlUnpacked<QData/*63:0*/, 1> __VnbaTriggered;
    };

    // INTERNAL VARIABLES
    Vtop__Syms* vlSymsp;
    const char* vlNamep;

    // CONSTRUCTORS
    Vtop___024root(Vtop__Syms* symsp, const char* namep);
    ~Vtop___024root();
    VL_UNCOPYABLE(Vtop___024root);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


#endif  // guard
