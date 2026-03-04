// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtop.h for the primary calling header

#include "Vtop__pch.h"

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtop___024root___dump_triggers__ico(const VlUnpacked<QData/*63:0*/, 2> &triggers, const std::string &tag);
#endif  // VL_DEBUG

void Vtop___024root___eval_triggers__ico(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_triggers__ico\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VicoTriggered[1U] = ((0xfffffffffffffffeULL 
                                      & vlSelfRef.__VicoTriggered[1U]) 
                                     | (IData)((IData)(vlSelfRef.__VicoFirstIteration)));
    vlSelfRef.__VicoTriggered[0U] = (QData)((IData)(
                                                    ((vlSelfRef.__Vtrigprevexpr___TOP__PixelLogic__DOT__TotMemory__DOT__tot_mem_out__1.neq(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem_out) 
                                                      << 1U) 
                                                     | ((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__fall_phase) 
                                                        != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__PixelLogic__DOT__TotCounter__DOT__fall_phase__1)))));
    vlSelfRef.__Vtrigprevexpr___TOP__PixelLogic__DOT__TotCounter__DOT__fall_phase__1 
        = vlSelfRef.PixelLogic__DOT__TotCounter__DOT__fall_phase;
    vlSelfRef.__Vtrigprevexpr___TOP__PixelLogic__DOT__TotMemory__DOT__tot_mem_out__1.assign(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem_out);
    if (VL_UNLIKELY(((1U & (~ (IData)(vlSelfRef.__VicoDidInit)))))) {
        vlSelfRef.__VicoDidInit = 1U;
        vlSelfRef.__VicoTriggered[0U] = (1ULL | vlSelfRef.__VicoTriggered[0U]);
        vlSelfRef.__VicoTriggered[0U] = (2ULL | vlSelfRef.__VicoTriggered[0U]);
    }
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vtop___024root___dump_triggers__ico(vlSelfRef.__VicoTriggered, "ico"s);
    }
#endif
}

bool Vtop___024root___trigger_anySet__ico(const VlUnpacked<QData/*63:0*/, 2> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___trigger_anySet__ico\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        if (in[n]) {
            return (1U);
        }
        n = ((IData)(1U) + n);
    } while ((2U > n));
    return (0U);
}

void Vtop___024root___ico_sequent__TOP__0(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___ico_sequent__TOP__0\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT____VdfgRegularize_hbb16befd_0_0;
    PixelLogic__DOT__TotMemory__DOT____VdfgRegularize_hbb16befd_0_0 = 0;
    // Body
    vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__pix_state_idle 
        = (0U == (IData)(vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__state_sr));
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__rcnt_q 
        = ((((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff5__DOT__q) 
             << 5U) | (((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff4__DOT__q) 
                        << 4U) | ((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff3__DOT__q) 
                                  << 3U))) | (((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff2__DOT__q) 
                                               << 2U) 
                                              | (((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff1__DOT__q) 
                                                  << 1U) 
                                                 | (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff0__DOT__q))));
    vlSelfRef.PixelLogic__DOT__Hit = vlSelfRef.Hit;
    vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__CG_tot_rst__DOT__Enable 
        = (1U & (~ ((IData)(vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__state_sr) 
                    >> 1U)));
    vlSelfRef.PixelLogic__DOT__Reset_b = vlSelfRef.Reset_b;
    vlSelfRef.PixelLogic__DOT__TotMemReadAddr = vlSelfRef.TotMemReadAddr;
    vlSelfRef.PixelLogic__DOT__TotMemWriteAddr = vlSelfRef.TotMemWriteAddr;
    vlSelfRef.PixelLogic__DOT__HitLeAny = vlSelfRef.HitLeAny;
    vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__HitLe 
        = (2U == (IData)(vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__state_sr));
    vlSelfRef.PixelLogic__DOT__PixelClkGated = vlSelfRef.PixelClkGated;
    vlSelfRef.PixelLogic__DOT__Tot6to4Mapping = vlSelfRef.Tot6to4Mapping;
    vlSelfRef.PixelLogic__DOT__TotDualEdgeCount = vlSelfRef.TotDualEdgeCount;
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff5__DOT__qn 
        = (1U & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff5__DOT__q)));
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff4__DOT__qn 
        = (1U & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff4__DOT__q)));
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff3__DOT__qn 
        = (1U & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff3__DOT__q)));
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff2__DOT__qn 
        = (1U & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff2__DOT__q)));
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff1__DOT__qn 
        = (1U & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff1__DOT__q)));
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff0__DOT__qn 
        = (1U & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff0__DOT__q)));
    vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__HitTe 
        = (1U == (IData)(vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__state_sr));
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff2__DOT__nclk 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__rcnt_q) 
                 >> 1U));
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff3__DOT__nclk 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__rcnt_q) 
                 >> 2U));
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff4__DOT__nclk 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__rcnt_q) 
                 >> 3U));
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff5__DOT__nclk 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__rcnt_q) 
                 >> 4U));
    vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__Hit 
        = vlSelfRef.PixelLogic__DOT__Hit;
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__Hit 
        = vlSelfRef.PixelLogic__DOT__Hit;
    vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__Reset_b 
        = vlSelfRef.PixelLogic__DOT__Reset_b;
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__Reset_b 
        = vlSelfRef.PixelLogic__DOT__Reset_b;
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemReadAddr 
        = vlSelfRef.PixelLogic__DOT__TotMemReadAddr;
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemWriteAddr 
        = vlSelfRef.PixelLogic__DOT__TotMemWriteAddr;
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__HitLeAny 
        = vlSelfRef.PixelLogic__DOT__HitLeAny;
    vlSelfRef.PixelLogic__DOT__HitLe = vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__HitLe;
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__PixelClkGated 
        = vlSelfRef.PixelLogic__DOT__PixelClkGated;
    vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__PixelClkGated 
        = vlSelfRef.PixelLogic__DOT__PixelClkGated;
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__PixelClkGated 
        = vlSelfRef.PixelLogic__DOT__PixelClkGated;
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__Tot6to4Mapping 
        = vlSelfRef.PixelLogic__DOT__Tot6to4Mapping;
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__TotDualEdgeCount 
        = vlSelfRef.PixelLogic__DOT__TotDualEdgeCount;
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__rcnt_qn 
        = ((((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff5__DOT__qn) 
             << 5U) | (((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff4__DOT__qn) 
                        << 4U) | ((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff3__DOT__qn) 
                                  << 3U))) | (((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff2__DOT__qn) 
                                               << 2U) 
                                              | (((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff1__DOT__qn) 
                                                  << 1U) 
                                                 | (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff0__DOT__qn))));
    vlSelfRef.PixelLogic__DOT__HitTe = vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__HitTe;
    vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__pix_rst 
        = ((~ ((IData)(vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__state_sr) 
               >> 1U)) & (IData)(vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__Reset_b));
    vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__PixelClkEn 
        = (1U & ((~ ((IData)(vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__Reset_b) 
                     & (IData)(vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__pix_state_idle))) 
                 | (IData)(vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__hit_rise)));
    vlSelfRef.HitLe = vlSelfRef.PixelLogic__DOT__HitLe;
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__HitLe 
        = vlSelfRef.PixelLogic__DOT__HitLe;
    vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__CG_tot_rst__DOT__ClkIn 
        = vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__PixelClkGated;
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__init_tot_cnt 
        = (0x0000003fU & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__rcnt_qn)));
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__HitTe 
        = vlSelfRef.PixelLogic__DOT__HitTe;
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__HitTe 
        = vlSelfRef.PixelLogic__DOT__HitTe;
    vlSelfRef.PixelLogic__DOT__PixelClkEn = vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__PixelClkEn;
    PixelLogic__DOT__TotMemory__DOT____VdfgRegularize_hbb16befd_0_0 
        = ((~ (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__HitLe)) 
           & (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__HitLeAny));
    vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__CG_tot_rst__DOT__ck_inb 
        = (1U & (~ (IData)(vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__CG_tot_rst__DOT__ClkIn)));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__latch_hit 
        = ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem_write_addr_reg) 
           & (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__HitTe));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__latch_hit 
        = (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem_write_addr_reg) 
            >> 1U) & (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__HitTe));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__latch_hit 
        = (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem_write_addr_reg) 
            >> 2U) & (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__HitTe));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__latch_hit 
        = (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem_write_addr_reg) 
            >> 3U) & (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__HitTe));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__latch_hit 
        = (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem_write_addr_reg) 
            >> 4U) & (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__HitTe));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__latch_hit 
        = (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem_write_addr_reg) 
            >> 5U) & (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__HitTe));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__latch_hit 
        = (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem_write_addr_reg) 
            >> 6U) & (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__HitTe));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__latch_hit 
        = (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem_write_addr_reg) 
            >> 7U) & (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__HitTe));
    vlSelfRef.PixelClkEn = vlSelfRef.PixelLogic__DOT__PixelClkEn;
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__latch_empty_hit 
        = ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemWriteAddr) 
           & (IData)(PixelLogic__DOT__TotMemory__DOT____VdfgRegularize_hbb16befd_0_0));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__latch_empty_hit 
        = (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemWriteAddr) 
            >> 1U) & (IData)(PixelLogic__DOT__TotMemory__DOT____VdfgRegularize_hbb16befd_0_0));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__latch_empty_hit 
        = (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemWriteAddr) 
            >> 2U) & (IData)(PixelLogic__DOT__TotMemory__DOT____VdfgRegularize_hbb16befd_0_0));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__latch_empty_hit 
        = (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemWriteAddr) 
            >> 3U) & (IData)(PixelLogic__DOT__TotMemory__DOT____VdfgRegularize_hbb16befd_0_0));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__latch_empty_hit 
        = (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemWriteAddr) 
            >> 4U) & (IData)(PixelLogic__DOT__TotMemory__DOT____VdfgRegularize_hbb16befd_0_0));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__latch_empty_hit 
        = (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemWriteAddr) 
            >> 5U) & (IData)(PixelLogic__DOT__TotMemory__DOT____VdfgRegularize_hbb16befd_0_0));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__latch_empty_hit 
        = (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemWriteAddr) 
            >> 6U) & (IData)(PixelLogic__DOT__TotMemory__DOT____VdfgRegularize_hbb16befd_0_0));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__latch_empty_hit 
        = (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemWriteAddr) 
            >> 7U) & (IData)(PixelLogic__DOT__TotMemory__DOT____VdfgRegularize_hbb16befd_0_0));
    if (vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__CG_tot_rst__DOT__ck_inb) {
        vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__CG_tot_rst__DOT__enl 
            = vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__CG_tot_rst__DOT__Enable;
    }
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__latch_condition 
        = ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__latch_empty_hit) 
           | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__latch_hit));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__latch_condition 
        = ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__latch_empty_hit) 
           | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__latch_hit));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__latch_condition 
        = ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__latch_empty_hit) 
           | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__latch_hit));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__latch_condition 
        = ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__latch_empty_hit) 
           | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__latch_hit));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__latch_condition 
        = ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__latch_empty_hit) 
           | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__latch_hit));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__latch_condition 
        = ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__latch_empty_hit) 
           | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__latch_hit));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__latch_condition 
        = ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__latch_empty_hit) 
           | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__latch_hit));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__latch_condition 
        = ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__latch_empty_hit) 
           | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__latch_hit));
    vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__CG_tot_rst__DOT__ClkOut 
        = ((IData)(vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__CG_tot_rst__DOT__ClkIn) 
           & (IData)(vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__CG_tot_rst__DOT__enl));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__latch_en 
        = (1U & ((~ (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__latch_condition)) 
                 | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__PixelClkGated)));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__latch_en 
        = (1U & ((~ (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__latch_condition)) 
                 | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__PixelClkGated)));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__latch_en 
        = (1U & ((~ (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__latch_condition)) 
                 | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__PixelClkGated)));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__latch_en 
        = (1U & ((~ (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__latch_condition)) 
                 | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__PixelClkGated)));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__latch_en 
        = (1U & ((~ (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__latch_condition)) 
                 | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__PixelClkGated)));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__latch_en 
        = (1U & ((~ (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__latch_condition)) 
                 | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__PixelClkGated)));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__latch_en 
        = (1U & ((~ (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__latch_condition)) 
                 | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__PixelClkGated)));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__latch_en 
        = (1U & ((~ (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__latch_condition)) 
                 | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__PixelClkGated)));
    vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__CntRst 
        = vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__CG_tot_rst__DOT__ClkOut;
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__MultiBitLatch__DOT__EN 
        = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__latch_en;
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__MultiBitLatch__DOT__EN 
        = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__latch_en;
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__MultiBitLatch__DOT__EN 
        = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__latch_en;
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__MultiBitLatch__DOT__EN 
        = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__latch_en;
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__MultiBitLatch__DOT__EN 
        = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__latch_en;
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__MultiBitLatch__DOT__EN 
        = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__latch_en;
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__MultiBitLatch__DOT__EN 
        = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__latch_en;
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__MultiBitLatch__DOT__EN 
        = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__latch_en;
    vlSelfRef.PixelLogic__DOT__CntRst = vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__CntRst;
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__CntRst 
        = vlSelfRef.PixelLogic__DOT__CntRst;
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__cnt_rst_b 
        = (1U & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__CntRst)));
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff0__DOT__nrst 
        = vlSelfRef.PixelLogic__DOT__TotCounter__DOT__cnt_rst_b;
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff1__DOT__nrst 
        = vlSelfRef.PixelLogic__DOT__TotCounter__DOT__cnt_rst_b;
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff2__DOT__nrst 
        = vlSelfRef.PixelLogic__DOT__TotCounter__DOT__cnt_rst_b;
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff3__DOT__nrst 
        = vlSelfRef.PixelLogic__DOT__TotCounter__DOT__cnt_rst_b;
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff4__DOT__nrst 
        = vlSelfRef.PixelLogic__DOT__TotCounter__DOT__cnt_rst_b;
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff5__DOT__nrst 
        = vlSelfRef.PixelLogic__DOT__TotCounter__DOT__cnt_rst_b;
}

void Vtop___024root___ico_comb__TOP__0(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___ico_comb__TOP__0\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__tot_cnt_stageone 
        = ((0x0000003eU & (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__init_tot_cnt)) 
           | (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__TotDualEdgeCount)
                     ? (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__fall_phase)
                     : (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__init_tot_cnt))));
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__tot_cnt_int 
        = (0x0000000fU & (((0U != (7U & ((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__tot_cnt_stageone) 
                                         >> 3U))) & (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__Tot6to4Mapping))
                           ? (8U | (7U & (((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__tot_cnt_stageone) 
                                           >> 2U) - (IData)(2U))))
                           : (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__tot_cnt_stageone)));
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__TotOverflow 
        = (7U == (7U & ((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__tot_cnt_int) 
                        >> 1U)));
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__TotCnt 
        = ((7U == (7U & ((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__tot_cnt_int) 
                         >> 1U))) ? (0x0000000eU & (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__tot_cnt_int))
            : (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__tot_cnt_int));
    vlSelfRef.PixelLogic__DOT__tot_overflow = vlSelfRef.PixelLogic__DOT__TotCounter__DOT__TotOverflow;
    if (vlSelfRef.PixelLogic__DOT__TotCounter__DOT__Reset_b) {
        if ((((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__Hit) 
              & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__TotOverflow))) 
             & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__HitTe)))) {
            vlSelfRef.PixelLogic__DOT__TotCounter__DOT__cnt_clk 
                = vlSelfRef.PixelLogic__DOT__TotCounter__DOT__PixelClkGated;
        }
    } else {
        vlSelfRef.PixelLogic__DOT__TotCounter__DOT__cnt_clk = 0U;
    }
    vlSelfRef.PixelLogic__DOT__tot_cnt = vlSelfRef.PixelLogic__DOT__TotCounter__DOT__TotCnt;
    vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__TotOverflow 
        = vlSelfRef.PixelLogic__DOT__tot_overflow;
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__fall_phase 
        = vlSelfRef.PixelLogic__DOT__TotCounter__DOT__cnt_clk;
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff0__DOT__nclk 
        = (1U & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__cnt_clk)));
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff1_clk 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__TotDualEdgeCount)
                  ? (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__cnt_clk)
                  : (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__rcnt_q)));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemDataIn 
        = vlSelfRef.PixelLogic__DOT__tot_cnt;
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff1__DOT__nclk 
        = vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff1_clk;
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__latch_input 
        = ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__latch_hit)
            ? (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemDataIn)
            : 0x0fU);
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__latch_input 
        = ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__latch_hit)
            ? (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemDataIn)
            : 0x0fU);
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__latch_input 
        = ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__latch_hit)
            ? (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemDataIn)
            : 0x0fU);
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__latch_input 
        = ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__latch_hit)
            ? (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemDataIn)
            : 0x0fU);
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__latch_input 
        = ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__latch_hit)
            ? (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemDataIn)
            : 0x0fU);
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__latch_input 
        = ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__latch_hit)
            ? (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemDataIn)
            : 0x0fU);
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__latch_input 
        = ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__latch_hit)
            ? (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemDataIn)
            : 0x0fU);
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__latch_input 
        = ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__latch_hit)
            ? (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemDataIn)
            : 0x0fU);
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__MultiBitLatch__DOT__D0 
        = (1U & (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__latch_input));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__MultiBitLatch__DOT__D1 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__latch_input) 
                 >> 1U));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__MultiBitLatch__DOT__D2 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__latch_input) 
                 >> 2U));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__MultiBitLatch__DOT__D3 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__latch_input) 
                 >> 3U));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__MultiBitLatch__DOT__D0 
        = (1U & (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__latch_input));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__MultiBitLatch__DOT__D1 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__latch_input) 
                 >> 1U));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__MultiBitLatch__DOT__D2 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__latch_input) 
                 >> 2U));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__MultiBitLatch__DOT__D3 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__latch_input) 
                 >> 3U));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__MultiBitLatch__DOT__D0 
        = (1U & (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__latch_input));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__MultiBitLatch__DOT__D1 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__latch_input) 
                 >> 1U));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__MultiBitLatch__DOT__D2 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__latch_input) 
                 >> 2U));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__MultiBitLatch__DOT__D3 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__latch_input) 
                 >> 3U));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__MultiBitLatch__DOT__D0 
        = (1U & (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__latch_input));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__MultiBitLatch__DOT__D1 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__latch_input) 
                 >> 1U));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__MultiBitLatch__DOT__D2 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__latch_input) 
                 >> 2U));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__MultiBitLatch__DOT__D3 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__latch_input) 
                 >> 3U));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__MultiBitLatch__DOT__D0 
        = (1U & (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__latch_input));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__MultiBitLatch__DOT__D1 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__latch_input) 
                 >> 1U));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__MultiBitLatch__DOT__D2 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__latch_input) 
                 >> 2U));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__MultiBitLatch__DOT__D3 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__latch_input) 
                 >> 3U));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__MultiBitLatch__DOT__D0 
        = (1U & (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__latch_input));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__MultiBitLatch__DOT__D1 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__latch_input) 
                 >> 1U));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__MultiBitLatch__DOT__D2 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__latch_input) 
                 >> 2U));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__MultiBitLatch__DOT__D3 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__latch_input) 
                 >> 3U));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__MultiBitLatch__DOT__D0 
        = (1U & (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__latch_input));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__MultiBitLatch__DOT__D1 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__latch_input) 
                 >> 1U));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__MultiBitLatch__DOT__D2 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__latch_input) 
                 >> 2U));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__MultiBitLatch__DOT__D3 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__latch_input) 
                 >> 3U));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__MultiBitLatch__DOT__D0 
        = (1U & (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__latch_input));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__MultiBitLatch__DOT__D1 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__latch_input) 
                 >> 1U));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__MultiBitLatch__DOT__D2 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__latch_input) 
                 >> 2U));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__MultiBitLatch__DOT__D3 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__latch_input) 
                 >> 3U));
    if ((1U & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__MultiBitLatch__DOT__EN)))) {
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__MultiBitLatch__DOT__Q0 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__MultiBitLatch__DOT__D0;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__MultiBitLatch__DOT__Q1 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__MultiBitLatch__DOT__D1;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__MultiBitLatch__DOT__Q2 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__MultiBitLatch__DOT__D2;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__MultiBitLatch__DOT__Q3 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__MultiBitLatch__DOT__D3;
    }
    if ((1U & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__MultiBitLatch__DOT__EN)))) {
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__MultiBitLatch__DOT__Q0 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__MultiBitLatch__DOT__D0;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__MultiBitLatch__DOT__Q1 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__MultiBitLatch__DOT__D1;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__MultiBitLatch__DOT__Q2 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__MultiBitLatch__DOT__D2;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__MultiBitLatch__DOT__Q3 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__MultiBitLatch__DOT__D3;
    }
    if ((1U & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__MultiBitLatch__DOT__EN)))) {
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__MultiBitLatch__DOT__Q0 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__MultiBitLatch__DOT__D0;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__MultiBitLatch__DOT__Q1 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__MultiBitLatch__DOT__D1;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__MultiBitLatch__DOT__Q2 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__MultiBitLatch__DOT__D2;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__MultiBitLatch__DOT__Q3 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__MultiBitLatch__DOT__D3;
    }
    if ((1U & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__MultiBitLatch__DOT__EN)))) {
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__MultiBitLatch__DOT__Q0 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__MultiBitLatch__DOT__D0;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__MultiBitLatch__DOT__Q1 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__MultiBitLatch__DOT__D1;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__MultiBitLatch__DOT__Q2 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__MultiBitLatch__DOT__D2;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__MultiBitLatch__DOT__Q3 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__MultiBitLatch__DOT__D3;
    }
    if ((1U & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__MultiBitLatch__DOT__EN)))) {
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__MultiBitLatch__DOT__Q0 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__MultiBitLatch__DOT__D0;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__MultiBitLatch__DOT__Q1 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__MultiBitLatch__DOT__D1;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__MultiBitLatch__DOT__Q2 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__MultiBitLatch__DOT__D2;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__MultiBitLatch__DOT__Q3 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__MultiBitLatch__DOT__D3;
    }
    if ((1U & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__MultiBitLatch__DOT__EN)))) {
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__MultiBitLatch__DOT__Q0 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__MultiBitLatch__DOT__D0;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__MultiBitLatch__DOT__Q1 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__MultiBitLatch__DOT__D1;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__MultiBitLatch__DOT__Q2 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__MultiBitLatch__DOT__D2;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__MultiBitLatch__DOT__Q3 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__MultiBitLatch__DOT__D3;
    }
    if ((1U & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__MultiBitLatch__DOT__EN)))) {
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__MultiBitLatch__DOT__Q0 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__MultiBitLatch__DOT__D0;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__MultiBitLatch__DOT__Q1 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__MultiBitLatch__DOT__D1;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__MultiBitLatch__DOT__Q2 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__MultiBitLatch__DOT__D2;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__MultiBitLatch__DOT__Q3 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__MultiBitLatch__DOT__D3;
    }
    if ((1U & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__MultiBitLatch__DOT__EN)))) {
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__MultiBitLatch__DOT__Q0 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__MultiBitLatch__DOT__D0;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__MultiBitLatch__DOT__Q1 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__MultiBitLatch__DOT__D1;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__MultiBitLatch__DOT__Q2 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__MultiBitLatch__DOT__D2;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__MultiBitLatch__DOT__Q3 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__MultiBitLatch__DOT__D3;
    }
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem[0U] 
        = ((((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__MultiBitLatch__DOT__Q3) 
             << 3U) | ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__MultiBitLatch__DOT__Q2) 
                       << 2U)) | (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__MultiBitLatch__DOT__Q1) 
                                   << 1U) | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__MultiBitLatch__DOT__Q0)));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem[1U] 
        = ((((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__MultiBitLatch__DOT__Q3) 
             << 3U) | ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__MultiBitLatch__DOT__Q2) 
                       << 2U)) | (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__MultiBitLatch__DOT__Q1) 
                                   << 1U) | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__MultiBitLatch__DOT__Q0)));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem[2U] 
        = ((((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__MultiBitLatch__DOT__Q3) 
             << 3U) | ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__MultiBitLatch__DOT__Q2) 
                       << 2U)) | (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__MultiBitLatch__DOT__Q1) 
                                   << 1U) | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__MultiBitLatch__DOT__Q0)));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem[3U] 
        = ((((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__MultiBitLatch__DOT__Q3) 
             << 3U) | ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__MultiBitLatch__DOT__Q2) 
                       << 2U)) | (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__MultiBitLatch__DOT__Q1) 
                                   << 1U) | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__MultiBitLatch__DOT__Q0)));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem[4U] 
        = ((((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__MultiBitLatch__DOT__Q3) 
             << 3U) | ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__MultiBitLatch__DOT__Q2) 
                       << 2U)) | (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__MultiBitLatch__DOT__Q1) 
                                   << 1U) | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__MultiBitLatch__DOT__Q0)));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem[5U] 
        = ((((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__MultiBitLatch__DOT__Q3) 
             << 3U) | ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__MultiBitLatch__DOT__Q2) 
                       << 2U)) | (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__MultiBitLatch__DOT__Q1) 
                                   << 1U) | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__MultiBitLatch__DOT__Q0)));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem[6U] 
        = ((((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__MultiBitLatch__DOT__Q3) 
             << 3U) | ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__MultiBitLatch__DOT__Q2) 
                       << 2U)) | (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__MultiBitLatch__DOT__Q1) 
                                   << 1U) | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__MultiBitLatch__DOT__Q0)));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem[7U] 
        = ((((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__MultiBitLatch__DOT__Q3) 
             << 3U) | ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__MultiBitLatch__DOT__Q2) 
                       << 2U)) | (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__MultiBitLatch__DOT__Q1) 
                                   << 1U) | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__MultiBitLatch__DOT__Q0)));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem_out[0U] 
        = (vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem[0U] 
           & (- (IData)((1U & (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemReadAddr)))));
}

void Vtop___024root___ico_comb__TOP__1(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___ico_comb__TOP__1\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem_out[1U] 
        = ((vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem[1U] 
            & (- (IData)((1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemReadAddr) 
                                >> 1U))))) | vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem_out[0U]);
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem_out[2U] 
        = ((vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem[2U] 
            & (- (IData)((1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemReadAddr) 
                                >> 2U))))) | vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem_out[1U]);
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem_out[3U] 
        = ((vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem[3U] 
            & (- (IData)((1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemReadAddr) 
                                >> 3U))))) | vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem_out[2U]);
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem_out[4U] 
        = ((vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem[4U] 
            & (- (IData)((1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemReadAddr) 
                                >> 4U))))) | vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem_out[3U]);
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem_out[5U] 
        = ((vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem[5U] 
            & (- (IData)((1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemReadAddr) 
                                >> 5U))))) | vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem_out[4U]);
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem_out[6U] 
        = ((vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem[6U] 
            & (- (IData)((1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemReadAddr) 
                                >> 6U))))) | vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem_out[5U]);
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem_out[7U] 
        = ((vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem[7U] 
            & (- (IData)((1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemReadAddr) 
                                >> 7U))))) | vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem_out[6U]);
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemDataOut 
        = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem_out[7U];
    vlSelfRef.PixelLogic__DOT__TotMemDataOut = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemDataOut;
    vlSelfRef.TotMemDataOut = vlSelfRef.PixelLogic__DOT__TotMemDataOut;
}

void Vtop___024root___eval_ico(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_ico\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VicoTriggered[1U])) {
        Vtop___024root___ico_sequent__TOP__0(vlSelf);
    }
    if ((1ULL & (vlSelfRef.__VicoTriggered[1U] | vlSelfRef.__VicoTriggered[0U]))) {
        Vtop___024root___ico_comb__TOP__0(vlSelf);
    }
    if (((1ULL & vlSelfRef.__VicoTriggered[1U]) | (3ULL 
                                                   & vlSelfRef.__VicoTriggered[0U]))) {
        Vtop___024root___ico_comb__TOP__1(vlSelf);
    }
}

bool Vtop___024root___eval_phase__ico(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_phase__ico\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VicoExecute;
    // Body
    Vtop___024root___eval_triggers__ico(vlSelf);
    __VicoExecute = Vtop___024root___trigger_anySet__ico(vlSelfRef.__VicoTriggered);
    if (__VicoExecute) {
        Vtop___024root___eval_ico(vlSelf);
    }
    return (__VicoExecute);
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtop___024root___dump_triggers__act(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag);
#endif  // VL_DEBUG

void Vtop___024root___eval_triggers__act(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_triggers__act\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VactTriggered[0U] = (QData)((IData)(
                                                    (((((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__PixelClkGated) 
                                                        & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__PixelLogic__DOT__TotMemory__DOT__PixelClkGated__0))) 
                                                       << 0x00000011U) 
                                                      | (((~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff5__DOT__nrst)) 
                                                          & (IData)(vlSelfRef.__Vtrigprevexpr___TOP__PixelLogic__DOT__TotCounter__DOT__ff5__DOT__nrst__0)) 
                                                         << 0x00000010U)) 
                                                     | ((((((((~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff5__DOT__nclk)) 
                                                              & (IData)(vlSelfRef.__Vtrigprevexpr___TOP__PixelLogic__DOT__TotCounter__DOT__ff5__DOT__nclk__0)) 
                                                             << 3U) 
                                                            | (((~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff4__DOT__nrst)) 
                                                                & (IData)(vlSelfRef.__Vtrigprevexpr___TOP__PixelLogic__DOT__TotCounter__DOT__ff4__DOT__nrst__0)) 
                                                               << 2U)) 
                                                           | ((((~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff4__DOT__nclk)) 
                                                                & (IData)(vlSelfRef.__Vtrigprevexpr___TOP__PixelLogic__DOT__TotCounter__DOT__ff4__DOT__nclk__0)) 
                                                               << 1U) 
                                                              | ((~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff3__DOT__nrst)) 
                                                                 & (IData)(vlSelfRef.__Vtrigprevexpr___TOP__PixelLogic__DOT__TotCounter__DOT__ff3__DOT__nrst__0)))) 
                                                          << 0x0000000cU) 
                                                         | ((((((~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff3__DOT__nclk)) 
                                                                & (IData)(vlSelfRef.__Vtrigprevexpr___TOP__PixelLogic__DOT__TotCounter__DOT__ff3__DOT__nclk__0)) 
                                                               << 3U) 
                                                              | (((~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff2__DOT__nrst)) 
                                                                  & (IData)(vlSelfRef.__Vtrigprevexpr___TOP__PixelLogic__DOT__TotCounter__DOT__ff2__DOT__nrst__0)) 
                                                                 << 2U)) 
                                                             | ((((~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff2__DOT__nclk)) 
                                                                  & (IData)(vlSelfRef.__Vtrigprevexpr___TOP__PixelLogic__DOT__TotCounter__DOT__ff2__DOT__nclk__0)) 
                                                                 << 1U) 
                                                                | ((~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff1__DOT__nrst)) 
                                                                   & (IData)(vlSelfRef.__Vtrigprevexpr___TOP__PixelLogic__DOT__TotCounter__DOT__ff1__DOT__nrst__0)))) 
                                                            << 8U)) 
                                                        | (((((((~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff1__DOT__nclk)) 
                                                                & (IData)(vlSelfRef.__Vtrigprevexpr___TOP__PixelLogic__DOT__TotCounter__DOT__ff1__DOT__nclk__0)) 
                                                               << 3U) 
                                                              | (((~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff0__DOT__nrst)) 
                                                                  & (IData)(vlSelfRef.__Vtrigprevexpr___TOP__PixelLogic__DOT__TotCounter__DOT__ff0__DOT__nrst__0)) 
                                                                 << 2U)) 
                                                             | ((((~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff0__DOT__nclk)) 
                                                                  & (IData)(vlSelfRef.__Vtrigprevexpr___TOP__PixelLogic__DOT__TotCounter__DOT__ff0__DOT__nclk__0)) 
                                                                 << 1U) 
                                                                | ((IData)(vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__PixelClkGated) 
                                                                   & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__PixelLogic__DOT__PixelCTRL__DOT__PixelClkGated__0))))) 
                                                            << 4U) 
                                                           | (((((~ (IData)(vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__pix_rst)) 
                                                                 & (IData)(vlSelfRef.__Vtrigprevexpr___TOP__PixelLogic__DOT__PixelCTRL__DOT__pix_rst__0)) 
                                                                << 3U) 
                                                               | (((IData)(vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__Hit) 
                                                                   & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__PixelLogic__DOT__PixelCTRL__DOT__Hit__0))) 
                                                                  << 2U)) 
                                                              | ((vlSelfRef.__Vtrigprevexpr___TOP__PixelLogic__DOT__TotMemory__DOT__tot_mem_out__2.neq(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem_out) 
                                                                  << 1U) 
                                                                 | ((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__fall_phase) 
                                                                    != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__PixelLogic__DOT__TotCounter__DOT__fall_phase__2)))))))));
    vlSelfRef.__Vtrigprevexpr___TOP__PixelLogic__DOT__TotCounter__DOT__fall_phase__2 
        = vlSelfRef.PixelLogic__DOT__TotCounter__DOT__fall_phase;
    vlSelfRef.__Vtrigprevexpr___TOP__PixelLogic__DOT__TotMemory__DOT__tot_mem_out__2.assign(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem_out);
    vlSelfRef.__Vtrigprevexpr___TOP__PixelLogic__DOT__PixelCTRL__DOT__Hit__0 
        = vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__Hit;
    vlSelfRef.__Vtrigprevexpr___TOP__PixelLogic__DOT__PixelCTRL__DOT__pix_rst__0 
        = vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__pix_rst;
    vlSelfRef.__Vtrigprevexpr___TOP__PixelLogic__DOT__PixelCTRL__DOT__PixelClkGated__0 
        = vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__PixelClkGated;
    vlSelfRef.__Vtrigprevexpr___TOP__PixelLogic__DOT__TotCounter__DOT__ff0__DOT__nclk__0 
        = vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff0__DOT__nclk;
    vlSelfRef.__Vtrigprevexpr___TOP__PixelLogic__DOT__TotCounter__DOT__ff0__DOT__nrst__0 
        = vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff0__DOT__nrst;
    vlSelfRef.__Vtrigprevexpr___TOP__PixelLogic__DOT__TotCounter__DOT__ff1__DOT__nclk__0 
        = vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff1__DOT__nclk;
    vlSelfRef.__Vtrigprevexpr___TOP__PixelLogic__DOT__TotCounter__DOT__ff1__DOT__nrst__0 
        = vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff1__DOT__nrst;
    vlSelfRef.__Vtrigprevexpr___TOP__PixelLogic__DOT__TotCounter__DOT__ff2__DOT__nclk__0 
        = vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff2__DOT__nclk;
    vlSelfRef.__Vtrigprevexpr___TOP__PixelLogic__DOT__TotCounter__DOT__ff2__DOT__nrst__0 
        = vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff2__DOT__nrst;
    vlSelfRef.__Vtrigprevexpr___TOP__PixelLogic__DOT__TotCounter__DOT__ff3__DOT__nclk__0 
        = vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff3__DOT__nclk;
    vlSelfRef.__Vtrigprevexpr___TOP__PixelLogic__DOT__TotCounter__DOT__ff3__DOT__nrst__0 
        = vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff3__DOT__nrst;
    vlSelfRef.__Vtrigprevexpr___TOP__PixelLogic__DOT__TotCounter__DOT__ff4__DOT__nclk__0 
        = vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff4__DOT__nclk;
    vlSelfRef.__Vtrigprevexpr___TOP__PixelLogic__DOT__TotCounter__DOT__ff4__DOT__nrst__0 
        = vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff4__DOT__nrst;
    vlSelfRef.__Vtrigprevexpr___TOP__PixelLogic__DOT__TotCounter__DOT__ff5__DOT__nclk__0 
        = vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff5__DOT__nclk;
    vlSelfRef.__Vtrigprevexpr___TOP__PixelLogic__DOT__TotCounter__DOT__ff5__DOT__nrst__0 
        = vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff5__DOT__nrst;
    vlSelfRef.__Vtrigprevexpr___TOP__PixelLogic__DOT__TotMemory__DOT__PixelClkGated__0 
        = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__PixelClkGated;
    if (VL_UNLIKELY(((1U & (~ (IData)(vlSelfRef.__VactDidInit)))))) {
        vlSelfRef.__VactDidInit = 1U;
        vlSelfRef.__VactTriggered[0U] = (1ULL | vlSelfRef.__VactTriggered[0U]);
        vlSelfRef.__VactTriggered[0U] = (2ULL | vlSelfRef.__VactTriggered[0U]);
    }
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vtop___024root___dump_triggers__act(vlSelfRef.__VactTriggered, "act"s);
    }
#endif
}

bool Vtop___024root___trigger_anySet__act(const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___trigger_anySet__act\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        if (in[n]) {
            return (1U);
        }
        n = ((IData)(1U) + n);
    } while ((1U > n));
    return (0U);
}

void Vtop___024root___eval_act(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_act\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VactTriggered[0U])) {
        Vtop___024root___ico_comb__TOP__0(vlSelf);
    }
    if ((3ULL & vlSelfRef.__VactTriggered[0U])) {
        Vtop___024root___ico_comb__TOP__1(vlSelf);
    }
}

void Vtop___024root___nba_sequent__TOP__0(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__0\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff0__DOT__q 
        = ((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff0__DOT__nrst) 
           & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff0__DOT__q)));
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff0__DOT__qn 
        = (1U & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff0__DOT__q)));
}

void Vtop___024root___nba_sequent__TOP__1(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__1\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff1__DOT__q 
        = ((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff1__DOT__nrst) 
           & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff1__DOT__q)));
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff1__DOT__qn 
        = (1U & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff1__DOT__q)));
}

void Vtop___024root___nba_sequent__TOP__2(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__2\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff2__DOT__q 
        = ((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff2__DOT__nrst) 
           & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff2__DOT__q)));
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff2__DOT__qn 
        = (1U & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff2__DOT__q)));
}

void Vtop___024root___nba_sequent__TOP__3(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__3\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff3__DOT__q 
        = ((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff3__DOT__nrst) 
           & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff3__DOT__q)));
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff3__DOT__qn 
        = (1U & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff3__DOT__q)));
}

void Vtop___024root___nba_sequent__TOP__4(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__4\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff4__DOT__q 
        = ((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff4__DOT__nrst) 
           & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff4__DOT__q)));
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff4__DOT__qn 
        = (1U & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff4__DOT__q)));
}

void Vtop___024root___nba_sequent__TOP__5(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__5\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__state_sr 
        = ((IData)(vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__Reset_b)
            ? ((((IData)(vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__hit_rise) 
                 | (((IData)(vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__Hit) 
                     & ((IData)(vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__state_sr) 
                        >> 1U)) & (~ (IData)(vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__TotOverflow)))) 
                << 1U) | (1U & ((IData)(vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__state_sr) 
                                >> 1U))) : 0U);
    vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__pix_state_idle 
        = (0U == (IData)(vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__state_sr));
    vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__CG_tot_rst__DOT__Enable 
        = (1U & (~ ((IData)(vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__state_sr) 
                    >> 1U)));
    vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__HitLe 
        = (2U == (IData)(vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__state_sr));
    vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__HitTe 
        = (1U == (IData)(vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__state_sr));
    if (vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__CG_tot_rst__DOT__ck_inb) {
        vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__CG_tot_rst__DOT__enl 
            = vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__CG_tot_rst__DOT__Enable;
    }
    vlSelfRef.PixelLogic__DOT__HitLe = vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__HitLe;
    vlSelfRef.PixelLogic__DOT__HitTe = vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__HitTe;
    vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__CG_tot_rst__DOT__ClkOut 
        = ((IData)(vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__CG_tot_rst__DOT__ClkIn) 
           & (IData)(vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__CG_tot_rst__DOT__enl));
    vlSelfRef.HitLe = vlSelfRef.PixelLogic__DOT__HitLe;
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__HitTe 
        = vlSelfRef.PixelLogic__DOT__HitTe;
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__HitTe 
        = vlSelfRef.PixelLogic__DOT__HitTe;
    vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__CntRst 
        = vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__CG_tot_rst__DOT__ClkOut;
    vlSelfRef.PixelLogic__DOT__CntRst = vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__CntRst;
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__CntRst 
        = vlSelfRef.PixelLogic__DOT__CntRst;
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__cnt_rst_b 
        = (1U & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__CntRst)));
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff0__DOT__nrst 
        = vlSelfRef.PixelLogic__DOT__TotCounter__DOT__cnt_rst_b;
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff1__DOT__nrst 
        = vlSelfRef.PixelLogic__DOT__TotCounter__DOT__cnt_rst_b;
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff2__DOT__nrst 
        = vlSelfRef.PixelLogic__DOT__TotCounter__DOT__cnt_rst_b;
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff3__DOT__nrst 
        = vlSelfRef.PixelLogic__DOT__TotCounter__DOT__cnt_rst_b;
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff4__DOT__nrst 
        = vlSelfRef.PixelLogic__DOT__TotCounter__DOT__cnt_rst_b;
}

void Vtop___024root___nba_sequent__TOP__6(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__6\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff5__DOT__q 
        = vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff5__DOT__nrst;
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff5__DOT__qn 
        = (1U & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff5__DOT__q)));
}

void Vtop___024root___nba_sequent__TOP__7(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__7\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if (vlSelfRef.PixelLogic__DOT__TotMemory__DOT__HitLe) {
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem_write_addr_reg 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemWriteAddr;
    }
}

void Vtop___024root___nba_sequent__TOP__8(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__8\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__hit_rise 
        = vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__pix_rst;
}

void Vtop___024root___nba_sequent__TOP__9(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__9\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ PixelLogic__DOT__TotMemory__DOT____VdfgRegularize_hbb16befd_0_0;
    PixelLogic__DOT__TotMemory__DOT____VdfgRegularize_hbb16befd_0_0 = 0;
    // Body
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff5__DOT__nrst 
        = vlSelfRef.PixelLogic__DOT__TotCounter__DOT__cnt_rst_b;
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__HitLe 
        = vlSelfRef.PixelLogic__DOT__HitLe;
    vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__pix_rst 
        = ((~ ((IData)(vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__state_sr) 
               >> 1U)) & (IData)(vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__Reset_b));
    PixelLogic__DOT__TotMemory__DOT____VdfgRegularize_hbb16befd_0_0 
        = ((~ (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__HitLe)) 
           & (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__HitLeAny));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__latch_empty_hit 
        = ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemWriteAddr) 
           & (IData)(PixelLogic__DOT__TotMemory__DOT____VdfgRegularize_hbb16befd_0_0));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__latch_empty_hit 
        = (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemWriteAddr) 
            >> 1U) & (IData)(PixelLogic__DOT__TotMemory__DOT____VdfgRegularize_hbb16befd_0_0));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__latch_empty_hit 
        = (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemWriteAddr) 
            >> 2U) & (IData)(PixelLogic__DOT__TotMemory__DOT____VdfgRegularize_hbb16befd_0_0));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__latch_empty_hit 
        = (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemWriteAddr) 
            >> 3U) & (IData)(PixelLogic__DOT__TotMemory__DOT____VdfgRegularize_hbb16befd_0_0));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__latch_empty_hit 
        = (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemWriteAddr) 
            >> 4U) & (IData)(PixelLogic__DOT__TotMemory__DOT____VdfgRegularize_hbb16befd_0_0));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__latch_empty_hit 
        = (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemWriteAddr) 
            >> 5U) & (IData)(PixelLogic__DOT__TotMemory__DOT____VdfgRegularize_hbb16befd_0_0));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__latch_empty_hit 
        = (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemWriteAddr) 
            >> 6U) & (IData)(PixelLogic__DOT__TotMemory__DOT____VdfgRegularize_hbb16befd_0_0));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__latch_empty_hit 
        = (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemWriteAddr) 
            >> 7U) & (IData)(PixelLogic__DOT__TotMemory__DOT____VdfgRegularize_hbb16befd_0_0));
}

void Vtop___024root___nba_comb__TOP__0(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_comb__TOP__0\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__rcnt_q 
        = ((((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff5__DOT__q) 
             << 5U) | (((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff4__DOT__q) 
                        << 4U) | ((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff3__DOT__q) 
                                  << 3U))) | (((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff2__DOT__q) 
                                               << 2U) 
                                              | (((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff1__DOT__q) 
                                                  << 1U) 
                                                 | (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff0__DOT__q))));
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__rcnt_qn 
        = ((((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff5__DOT__qn) 
             << 5U) | (((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff4__DOT__qn) 
                        << 4U) | ((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff3__DOT__qn) 
                                  << 3U))) | (((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff2__DOT__qn) 
                                               << 2U) 
                                              | (((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff1__DOT__qn) 
                                                  << 1U) 
                                                 | (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff0__DOT__qn))));
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff2__DOT__nclk 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__rcnt_q) 
                 >> 1U));
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff3__DOT__nclk 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__rcnt_q) 
                 >> 2U));
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff4__DOT__nclk 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__rcnt_q) 
                 >> 3U));
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff5__DOT__nclk 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__rcnt_q) 
                 >> 4U));
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__init_tot_cnt 
        = (0x0000003fU & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__rcnt_qn)));
}

void Vtop___024root___nba_comb__TOP__1(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_comb__TOP__1\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__latch_hit 
        = ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem_write_addr_reg) 
           & (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__HitTe));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__latch_hit 
        = (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem_write_addr_reg) 
            >> 1U) & (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__HitTe));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__latch_hit 
        = (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem_write_addr_reg) 
            >> 2U) & (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__HitTe));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__latch_hit 
        = (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem_write_addr_reg) 
            >> 3U) & (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__HitTe));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__latch_hit 
        = (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem_write_addr_reg) 
            >> 4U) & (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__HitTe));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__latch_hit 
        = (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem_write_addr_reg) 
            >> 5U) & (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__HitTe));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__latch_hit 
        = (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem_write_addr_reg) 
            >> 6U) & (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__HitTe));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__latch_hit 
        = (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem_write_addr_reg) 
            >> 7U) & (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__HitTe));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__latch_condition 
        = ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__latch_empty_hit) 
           | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__latch_hit));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__latch_condition 
        = ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__latch_empty_hit) 
           | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__latch_hit));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__latch_condition 
        = ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__latch_empty_hit) 
           | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__latch_hit));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__latch_condition 
        = ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__latch_empty_hit) 
           | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__latch_hit));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__latch_condition 
        = ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__latch_empty_hit) 
           | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__latch_hit));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__latch_condition 
        = ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__latch_empty_hit) 
           | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__latch_hit));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__latch_condition 
        = ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__latch_empty_hit) 
           | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__latch_hit));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__latch_condition 
        = ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__latch_empty_hit) 
           | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__latch_hit));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__latch_en 
        = (1U & ((~ (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__latch_condition)) 
                 | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__PixelClkGated)));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__latch_en 
        = (1U & ((~ (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__latch_condition)) 
                 | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__PixelClkGated)));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__latch_en 
        = (1U & ((~ (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__latch_condition)) 
                 | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__PixelClkGated)));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__latch_en 
        = (1U & ((~ (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__latch_condition)) 
                 | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__PixelClkGated)));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__latch_en 
        = (1U & ((~ (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__latch_condition)) 
                 | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__PixelClkGated)));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__latch_en 
        = (1U & ((~ (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__latch_condition)) 
                 | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__PixelClkGated)));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__latch_en 
        = (1U & ((~ (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__latch_condition)) 
                 | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__PixelClkGated)));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__latch_en 
        = (1U & ((~ (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__latch_condition)) 
                 | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__PixelClkGated)));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__MultiBitLatch__DOT__EN 
        = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__latch_en;
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__MultiBitLatch__DOT__EN 
        = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__latch_en;
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__MultiBitLatch__DOT__EN 
        = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__latch_en;
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__MultiBitLatch__DOT__EN 
        = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__latch_en;
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__MultiBitLatch__DOT__EN 
        = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__latch_en;
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__MultiBitLatch__DOT__EN 
        = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__latch_en;
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__MultiBitLatch__DOT__EN 
        = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__latch_en;
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__MultiBitLatch__DOT__EN 
        = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__latch_en;
}

void Vtop___024root___nba_comb__TOP__2(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_comb__TOP__2\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__PixelClkEn 
        = (1U & ((~ ((IData)(vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__Reset_b) 
                     & (IData)(vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__pix_state_idle))) 
                 | (IData)(vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__hit_rise)));
    vlSelfRef.PixelLogic__DOT__PixelClkEn = vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__PixelClkEn;
    vlSelfRef.PixelClkEn = vlSelfRef.PixelLogic__DOT__PixelClkEn;
}

void Vtop___024root___nba_comb__TOP__3(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_comb__TOP__3\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__tot_cnt_stageone 
        = ((0x0000003eU & (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__init_tot_cnt)) 
           | (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__TotDualEdgeCount)
                     ? (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__fall_phase)
                     : (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__init_tot_cnt))));
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__tot_cnt_int 
        = (0x0000000fU & (((0U != (7U & ((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__tot_cnt_stageone) 
                                         >> 3U))) & (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__Tot6to4Mapping))
                           ? (8U | (7U & (((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__tot_cnt_stageone) 
                                           >> 2U) - (IData)(2U))))
                           : (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__tot_cnt_stageone)));
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__TotOverflow 
        = (7U == (7U & ((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__tot_cnt_int) 
                        >> 1U)));
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__TotCnt 
        = ((7U == (7U & ((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__tot_cnt_int) 
                         >> 1U))) ? (0x0000000eU & (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__tot_cnt_int))
            : (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__tot_cnt_int));
    vlSelfRef.PixelLogic__DOT__tot_overflow = vlSelfRef.PixelLogic__DOT__TotCounter__DOT__TotOverflow;
    vlSelfRef.PixelLogic__DOT__tot_cnt = vlSelfRef.PixelLogic__DOT__TotCounter__DOT__TotCnt;
    vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__TotOverflow 
        = vlSelfRef.PixelLogic__DOT__tot_overflow;
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemDataIn 
        = vlSelfRef.PixelLogic__DOT__tot_cnt;
}

void Vtop___024root___nba_comb__TOP__4(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_comb__TOP__4\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if (vlSelfRef.PixelLogic__DOT__TotCounter__DOT__Reset_b) {
        if ((((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__Hit) 
              & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__TotOverflow))) 
             & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__HitTe)))) {
            vlSelfRef.PixelLogic__DOT__TotCounter__DOT__cnt_clk 
                = vlSelfRef.PixelLogic__DOT__TotCounter__DOT__PixelClkGated;
        }
    } else {
        vlSelfRef.PixelLogic__DOT__TotCounter__DOT__cnt_clk = 0U;
    }
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__fall_phase 
        = vlSelfRef.PixelLogic__DOT__TotCounter__DOT__cnt_clk;
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff0__DOT__nclk 
        = (1U & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__cnt_clk)));
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff1_clk 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__TotDualEdgeCount)
                  ? (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__cnt_clk)
                  : (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__rcnt_q)));
    vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff1__DOT__nclk 
        = vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff1_clk;
}

void Vtop___024root___nba_comb__TOP__5(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_comb__TOP__5\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__latch_input 
        = ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__latch_hit)
            ? (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemDataIn)
            : 0x0fU);
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__latch_input 
        = ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__latch_hit)
            ? (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemDataIn)
            : 0x0fU);
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__latch_input 
        = ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__latch_hit)
            ? (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemDataIn)
            : 0x0fU);
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__latch_input 
        = ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__latch_hit)
            ? (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemDataIn)
            : 0x0fU);
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__latch_input 
        = ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__latch_hit)
            ? (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemDataIn)
            : 0x0fU);
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__latch_input 
        = ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__latch_hit)
            ? (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemDataIn)
            : 0x0fU);
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__latch_input 
        = ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__latch_hit)
            ? (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemDataIn)
            : 0x0fU);
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__latch_input 
        = ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__latch_hit)
            ? (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemDataIn)
            : 0x0fU);
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__MultiBitLatch__DOT__D0 
        = (1U & (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__latch_input));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__MultiBitLatch__DOT__D1 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__latch_input) 
                 >> 1U));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__MultiBitLatch__DOT__D2 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__latch_input) 
                 >> 2U));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__MultiBitLatch__DOT__D3 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__latch_input) 
                 >> 3U));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__MultiBitLatch__DOT__D0 
        = (1U & (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__latch_input));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__MultiBitLatch__DOT__D1 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__latch_input) 
                 >> 1U));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__MultiBitLatch__DOT__D2 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__latch_input) 
                 >> 2U));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__MultiBitLatch__DOT__D3 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__latch_input) 
                 >> 3U));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__MultiBitLatch__DOT__D0 
        = (1U & (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__latch_input));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__MultiBitLatch__DOT__D1 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__latch_input) 
                 >> 1U));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__MultiBitLatch__DOT__D2 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__latch_input) 
                 >> 2U));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__MultiBitLatch__DOT__D3 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__latch_input) 
                 >> 3U));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__MultiBitLatch__DOT__D0 
        = (1U & (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__latch_input));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__MultiBitLatch__DOT__D1 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__latch_input) 
                 >> 1U));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__MultiBitLatch__DOT__D2 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__latch_input) 
                 >> 2U));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__MultiBitLatch__DOT__D3 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__latch_input) 
                 >> 3U));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__MultiBitLatch__DOT__D0 
        = (1U & (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__latch_input));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__MultiBitLatch__DOT__D1 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__latch_input) 
                 >> 1U));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__MultiBitLatch__DOT__D2 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__latch_input) 
                 >> 2U));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__MultiBitLatch__DOT__D3 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__latch_input) 
                 >> 3U));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__MultiBitLatch__DOT__D0 
        = (1U & (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__latch_input));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__MultiBitLatch__DOT__D1 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__latch_input) 
                 >> 1U));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__MultiBitLatch__DOT__D2 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__latch_input) 
                 >> 2U));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__MultiBitLatch__DOT__D3 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__latch_input) 
                 >> 3U));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__MultiBitLatch__DOT__D0 
        = (1U & (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__latch_input));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__MultiBitLatch__DOT__D1 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__latch_input) 
                 >> 1U));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__MultiBitLatch__DOT__D2 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__latch_input) 
                 >> 2U));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__MultiBitLatch__DOT__D3 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__latch_input) 
                 >> 3U));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__MultiBitLatch__DOT__D0 
        = (1U & (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__latch_input));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__MultiBitLatch__DOT__D1 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__latch_input) 
                 >> 1U));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__MultiBitLatch__DOT__D2 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__latch_input) 
                 >> 2U));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__MultiBitLatch__DOT__D3 
        = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__latch_input) 
                 >> 3U));
    if ((1U & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__MultiBitLatch__DOT__EN)))) {
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__MultiBitLatch__DOT__Q0 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__MultiBitLatch__DOT__D0;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__MultiBitLatch__DOT__Q1 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__MultiBitLatch__DOT__D1;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__MultiBitLatch__DOT__Q2 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__MultiBitLatch__DOT__D2;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__MultiBitLatch__DOT__Q3 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__MultiBitLatch__DOT__D3;
    }
    if ((1U & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__MultiBitLatch__DOT__EN)))) {
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__MultiBitLatch__DOT__Q0 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__MultiBitLatch__DOT__D0;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__MultiBitLatch__DOT__Q1 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__MultiBitLatch__DOT__D1;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__MultiBitLatch__DOT__Q2 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__MultiBitLatch__DOT__D2;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__MultiBitLatch__DOT__Q3 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__MultiBitLatch__DOT__D3;
    }
    if ((1U & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__MultiBitLatch__DOT__EN)))) {
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__MultiBitLatch__DOT__Q0 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__MultiBitLatch__DOT__D0;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__MultiBitLatch__DOT__Q1 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__MultiBitLatch__DOT__D1;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__MultiBitLatch__DOT__Q2 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__MultiBitLatch__DOT__D2;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__MultiBitLatch__DOT__Q3 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__MultiBitLatch__DOT__D3;
    }
    if ((1U & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__MultiBitLatch__DOT__EN)))) {
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__MultiBitLatch__DOT__Q0 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__MultiBitLatch__DOT__D0;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__MultiBitLatch__DOT__Q1 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__MultiBitLatch__DOT__D1;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__MultiBitLatch__DOT__Q2 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__MultiBitLatch__DOT__D2;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__MultiBitLatch__DOT__Q3 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__MultiBitLatch__DOT__D3;
    }
    if ((1U & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__MultiBitLatch__DOT__EN)))) {
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__MultiBitLatch__DOT__Q0 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__MultiBitLatch__DOT__D0;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__MultiBitLatch__DOT__Q1 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__MultiBitLatch__DOT__D1;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__MultiBitLatch__DOT__Q2 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__MultiBitLatch__DOT__D2;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__MultiBitLatch__DOT__Q3 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__MultiBitLatch__DOT__D3;
    }
    if ((1U & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__MultiBitLatch__DOT__EN)))) {
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__MultiBitLatch__DOT__Q0 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__MultiBitLatch__DOT__D0;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__MultiBitLatch__DOT__Q1 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__MultiBitLatch__DOT__D1;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__MultiBitLatch__DOT__Q2 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__MultiBitLatch__DOT__D2;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__MultiBitLatch__DOT__Q3 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__MultiBitLatch__DOT__D3;
    }
    if ((1U & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__MultiBitLatch__DOT__EN)))) {
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__MultiBitLatch__DOT__Q0 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__MultiBitLatch__DOT__D0;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__MultiBitLatch__DOT__Q1 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__MultiBitLatch__DOT__D1;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__MultiBitLatch__DOT__Q2 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__MultiBitLatch__DOT__D2;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__MultiBitLatch__DOT__Q3 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__MultiBitLatch__DOT__D3;
    }
    if ((1U & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__MultiBitLatch__DOT__EN)))) {
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__MultiBitLatch__DOT__Q0 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__MultiBitLatch__DOT__D0;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__MultiBitLatch__DOT__Q1 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__MultiBitLatch__DOT__D1;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__MultiBitLatch__DOT__Q2 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__MultiBitLatch__DOT__D2;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__MultiBitLatch__DOT__Q3 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__MultiBitLatch__DOT__D3;
    }
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem[0U] 
        = ((((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__MultiBitLatch__DOT__Q3) 
             << 3U) | ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__MultiBitLatch__DOT__Q2) 
                       << 2U)) | (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__MultiBitLatch__DOT__Q1) 
                                   << 1U) | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__MultiBitLatch__DOT__Q0)));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem[1U] 
        = ((((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__MultiBitLatch__DOT__Q3) 
             << 3U) | ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__MultiBitLatch__DOT__Q2) 
                       << 2U)) | (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__MultiBitLatch__DOT__Q1) 
                                   << 1U) | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__MultiBitLatch__DOT__Q0)));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem[2U] 
        = ((((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__MultiBitLatch__DOT__Q3) 
             << 3U) | ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__MultiBitLatch__DOT__Q2) 
                       << 2U)) | (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__MultiBitLatch__DOT__Q1) 
                                   << 1U) | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__MultiBitLatch__DOT__Q0)));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem[3U] 
        = ((((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__MultiBitLatch__DOT__Q3) 
             << 3U) | ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__MultiBitLatch__DOT__Q2) 
                       << 2U)) | (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__MultiBitLatch__DOT__Q1) 
                                   << 1U) | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__MultiBitLatch__DOT__Q0)));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem[4U] 
        = ((((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__MultiBitLatch__DOT__Q3) 
             << 3U) | ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__MultiBitLatch__DOT__Q2) 
                       << 2U)) | (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__MultiBitLatch__DOT__Q1) 
                                   << 1U) | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__MultiBitLatch__DOT__Q0)));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem[5U] 
        = ((((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__MultiBitLatch__DOT__Q3) 
             << 3U) | ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__MultiBitLatch__DOT__Q2) 
                       << 2U)) | (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__MultiBitLatch__DOT__Q1) 
                                   << 1U) | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__MultiBitLatch__DOT__Q0)));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem[6U] 
        = ((((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__MultiBitLatch__DOT__Q3) 
             << 3U) | ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__MultiBitLatch__DOT__Q2) 
                       << 2U)) | (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__MultiBitLatch__DOT__Q1) 
                                   << 1U) | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__MultiBitLatch__DOT__Q0)));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem[7U] 
        = ((((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__MultiBitLatch__DOT__Q3) 
             << 3U) | ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__MultiBitLatch__DOT__Q2) 
                       << 2U)) | (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__MultiBitLatch__DOT__Q1) 
                                   << 1U) | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__MultiBitLatch__DOT__Q0)));
    vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem_out[0U] 
        = (vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem[0U] 
           & (- (IData)((1U & (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemReadAddr)))));
}

void Vtop___024root___eval_nba(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_nba\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __Vinline__nba_sequent__TOP__9_PixelLogic__DOT__TotMemory__DOT____VdfgRegularize_hbb16befd_0_0;
    __Vinline__nba_sequent__TOP__9_PixelLogic__DOT__TotMemory__DOT____VdfgRegularize_hbb16befd_0_0 = 0;
    // Body
    if ((0x0000000000000060ULL & vlSelfRef.__VnbaTriggered[0U])) {
        vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff0__DOT__q 
            = ((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff0__DOT__nrst) 
               & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff0__DOT__q)));
        vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff0__DOT__qn 
            = (1U & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff0__DOT__q)));
    }
    if ((0x0000000000000180ULL & vlSelfRef.__VnbaTriggered[0U])) {
        vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff1__DOT__q 
            = ((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff1__DOT__nrst) 
               & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff1__DOT__q)));
        vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff1__DOT__qn 
            = (1U & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff1__DOT__q)));
    }
    if ((0x0000000000000600ULL & vlSelfRef.__VnbaTriggered[0U])) {
        vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff2__DOT__q 
            = ((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff2__DOT__nrst) 
               & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff2__DOT__q)));
        vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff2__DOT__qn 
            = (1U & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff2__DOT__q)));
    }
    if ((0x0000000000001800ULL & vlSelfRef.__VnbaTriggered[0U])) {
        vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff3__DOT__q 
            = ((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff3__DOT__nrst) 
               & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff3__DOT__q)));
        vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff3__DOT__qn 
            = (1U & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff3__DOT__q)));
    }
    if ((0x0000000000006000ULL & vlSelfRef.__VnbaTriggered[0U])) {
        vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff4__DOT__q 
            = ((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff4__DOT__nrst) 
               & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff4__DOT__q)));
        vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff4__DOT__qn 
            = (1U & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff4__DOT__q)));
    }
    if ((0x0000000000000010ULL & vlSelfRef.__VnbaTriggered[0U])) {
        vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__state_sr 
            = ((IData)(vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__Reset_b)
                ? ((((IData)(vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__hit_rise) 
                     | (((IData)(vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__Hit) 
                         & ((IData)(vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__state_sr) 
                            >> 1U)) & (~ (IData)(vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__TotOverflow)))) 
                    << 1U) | (1U & ((IData)(vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__state_sr) 
                                    >> 1U))) : 0U);
        vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__pix_state_idle 
            = (0U == (IData)(vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__state_sr));
        vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__CG_tot_rst__DOT__Enable 
            = (1U & (~ ((IData)(vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__state_sr) 
                        >> 1U)));
        vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__HitLe 
            = (2U == (IData)(vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__state_sr));
        vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__HitTe 
            = (1U == (IData)(vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__state_sr));
        if (vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__CG_tot_rst__DOT__ck_inb) {
            vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__CG_tot_rst__DOT__enl 
                = vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__CG_tot_rst__DOT__Enable;
        }
        vlSelfRef.PixelLogic__DOT__HitLe = vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__HitLe;
        vlSelfRef.PixelLogic__DOT__HitTe = vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__HitTe;
        vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__CG_tot_rst__DOT__ClkOut 
            = ((IData)(vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__CG_tot_rst__DOT__ClkIn) 
               & (IData)(vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__CG_tot_rst__DOT__enl));
        vlSelfRef.HitLe = vlSelfRef.PixelLogic__DOT__HitLe;
        vlSelfRef.PixelLogic__DOT__TotCounter__DOT__HitTe 
            = vlSelfRef.PixelLogic__DOT__HitTe;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__HitTe 
            = vlSelfRef.PixelLogic__DOT__HitTe;
        vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__CntRst 
            = vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__CG_tot_rst__DOT__ClkOut;
        vlSelfRef.PixelLogic__DOT__CntRst = vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__CntRst;
        vlSelfRef.PixelLogic__DOT__TotCounter__DOT__CntRst 
            = vlSelfRef.PixelLogic__DOT__CntRst;
        vlSelfRef.PixelLogic__DOT__TotCounter__DOT__cnt_rst_b 
            = (1U & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__CntRst)));
        vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff0__DOT__nrst 
            = vlSelfRef.PixelLogic__DOT__TotCounter__DOT__cnt_rst_b;
        vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff1__DOT__nrst 
            = vlSelfRef.PixelLogic__DOT__TotCounter__DOT__cnt_rst_b;
        vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff2__DOT__nrst 
            = vlSelfRef.PixelLogic__DOT__TotCounter__DOT__cnt_rst_b;
        vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff3__DOT__nrst 
            = vlSelfRef.PixelLogic__DOT__TotCounter__DOT__cnt_rst_b;
        vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff4__DOT__nrst 
            = vlSelfRef.PixelLogic__DOT__TotCounter__DOT__cnt_rst_b;
    }
    if ((0x0000000000018000ULL & vlSelfRef.__VnbaTriggered[0U])) {
        vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff5__DOT__q 
            = vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff5__DOT__nrst;
        vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff5__DOT__qn 
            = (1U & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff5__DOT__q)));
    }
    if ((0x0000000000020000ULL & vlSelfRef.__VnbaTriggered[0U])) {
        if (vlSelfRef.PixelLogic__DOT__TotMemory__DOT__HitLe) {
            vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem_write_addr_reg 
                = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemWriteAddr;
        }
    }
    if ((0x000000000000000cULL & vlSelfRef.__VnbaTriggered[0U])) {
        vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__hit_rise 
            = vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__pix_rst;
    }
    if ((0x0000000000000010ULL & vlSelfRef.__VnbaTriggered[0U])) {
        vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff5__DOT__nrst 
            = vlSelfRef.PixelLogic__DOT__TotCounter__DOT__cnt_rst_b;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__HitLe 
            = vlSelfRef.PixelLogic__DOT__HitLe;
        vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__pix_rst 
            = ((~ ((IData)(vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__state_sr) 
                   >> 1U)) & (IData)(vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__Reset_b));
        __Vinline__nba_sequent__TOP__9_PixelLogic__DOT__TotMemory__DOT____VdfgRegularize_hbb16befd_0_0 
            = ((~ (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__HitLe)) 
               & (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__HitLeAny));
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__latch_empty_hit 
            = ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemWriteAddr) 
               & __Vinline__nba_sequent__TOP__9_PixelLogic__DOT__TotMemory__DOT____VdfgRegularize_hbb16befd_0_0);
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__latch_empty_hit 
            = (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemWriteAddr) 
                >> 1U) & __Vinline__nba_sequent__TOP__9_PixelLogic__DOT__TotMemory__DOT____VdfgRegularize_hbb16befd_0_0);
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__latch_empty_hit 
            = (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemWriteAddr) 
                >> 2U) & __Vinline__nba_sequent__TOP__9_PixelLogic__DOT__TotMemory__DOT____VdfgRegularize_hbb16befd_0_0);
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__latch_empty_hit 
            = (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemWriteAddr) 
                >> 3U) & __Vinline__nba_sequent__TOP__9_PixelLogic__DOT__TotMemory__DOT____VdfgRegularize_hbb16befd_0_0);
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__latch_empty_hit 
            = (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemWriteAddr) 
                >> 4U) & __Vinline__nba_sequent__TOP__9_PixelLogic__DOT__TotMemory__DOT____VdfgRegularize_hbb16befd_0_0);
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__latch_empty_hit 
            = (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemWriteAddr) 
                >> 5U) & __Vinline__nba_sequent__TOP__9_PixelLogic__DOT__TotMemory__DOT____VdfgRegularize_hbb16befd_0_0);
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__latch_empty_hit 
            = (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemWriteAddr) 
                >> 6U) & __Vinline__nba_sequent__TOP__9_PixelLogic__DOT__TotMemory__DOT____VdfgRegularize_hbb16befd_0_0);
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__latch_empty_hit 
            = (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemWriteAddr) 
                >> 7U) & __Vinline__nba_sequent__TOP__9_PixelLogic__DOT__TotMemory__DOT____VdfgRegularize_hbb16befd_0_0);
    }
    if ((0x000000000001ffe0ULL & vlSelfRef.__VnbaTriggered[0U])) {
        vlSelfRef.PixelLogic__DOT__TotCounter__DOT__rcnt_q 
            = ((((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff5__DOT__q) 
                 << 5U) | (((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff4__DOT__q) 
                            << 4U) | ((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff3__DOT__q) 
                                      << 3U))) | (((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff2__DOT__q) 
                                                   << 2U) 
                                                  | (((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff1__DOT__q) 
                                                      << 1U) 
                                                     | (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff0__DOT__q))));
        vlSelfRef.PixelLogic__DOT__TotCounter__DOT__rcnt_qn 
            = ((((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff5__DOT__qn) 
                 << 5U) | (((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff4__DOT__qn) 
                            << 4U) | ((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff3__DOT__qn) 
                                      << 3U))) | (((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff2__DOT__qn) 
                                                   << 2U) 
                                                  | (((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff1__DOT__qn) 
                                                      << 1U) 
                                                     | (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff0__DOT__qn))));
        vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff2__DOT__nclk 
            = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__rcnt_q) 
                     >> 1U));
        vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff3__DOT__nclk 
            = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__rcnt_q) 
                     >> 2U));
        vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff4__DOT__nclk 
            = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__rcnt_q) 
                     >> 3U));
        vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff5__DOT__nclk 
            = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__rcnt_q) 
                     >> 4U));
        vlSelfRef.PixelLogic__DOT__TotCounter__DOT__init_tot_cnt 
            = (0x0000003fU & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__rcnt_qn)));
    }
    if ((0x0000000000020010ULL & vlSelfRef.__VnbaTriggered[0U])) {
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__latch_hit 
            = ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem_write_addr_reg) 
               & (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__HitTe));
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__latch_hit 
            = (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem_write_addr_reg) 
                >> 1U) & (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__HitTe));
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__latch_hit 
            = (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem_write_addr_reg) 
                >> 2U) & (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__HitTe));
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__latch_hit 
            = (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem_write_addr_reg) 
                >> 3U) & (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__HitTe));
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__latch_hit 
            = (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem_write_addr_reg) 
                >> 4U) & (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__HitTe));
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__latch_hit 
            = (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem_write_addr_reg) 
                >> 5U) & (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__HitTe));
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__latch_hit 
            = (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem_write_addr_reg) 
                >> 6U) & (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__HitTe));
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__latch_hit 
            = (((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__tot_mem_write_addr_reg) 
                >> 7U) & (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__HitTe));
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__latch_condition 
            = ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__latch_empty_hit) 
               | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__latch_hit));
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__latch_condition 
            = ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__latch_empty_hit) 
               | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__latch_hit));
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__latch_condition 
            = ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__latch_empty_hit) 
               | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__latch_hit));
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__latch_condition 
            = ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__latch_empty_hit) 
               | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__latch_hit));
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__latch_condition 
            = ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__latch_empty_hit) 
               | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__latch_hit));
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__latch_condition 
            = ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__latch_empty_hit) 
               | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__latch_hit));
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__latch_condition 
            = ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__latch_empty_hit) 
               | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__latch_hit));
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__latch_condition 
            = ((IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__latch_empty_hit) 
               | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__latch_hit));
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__latch_en 
            = (1U & ((~ (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__latch_condition)) 
                     | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__PixelClkGated)));
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__latch_en 
            = (1U & ((~ (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__latch_condition)) 
                     | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__PixelClkGated)));
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__latch_en 
            = (1U & ((~ (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__latch_condition)) 
                     | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__PixelClkGated)));
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__latch_en 
            = (1U & ((~ (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__latch_condition)) 
                     | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__PixelClkGated)));
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__latch_en 
            = (1U & ((~ (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__latch_condition)) 
                     | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__PixelClkGated)));
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__latch_en 
            = (1U & ((~ (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__latch_condition)) 
                     | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__PixelClkGated)));
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__latch_en 
            = (1U & ((~ (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__latch_condition)) 
                     | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__PixelClkGated)));
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__latch_en 
            = (1U & ((~ (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__latch_condition)) 
                     | (IData)(vlSelfRef.PixelLogic__DOT__TotMemory__DOT__PixelClkGated)));
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__MultiBitLatch__DOT__EN 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__0__KET____DOT__latch_en;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__MultiBitLatch__DOT__EN 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__1__KET____DOT__latch_en;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__MultiBitLatch__DOT__EN 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__2__KET____DOT__latch_en;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__MultiBitLatch__DOT__EN 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__3__KET____DOT__latch_en;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__MultiBitLatch__DOT__EN 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__4__KET____DOT__latch_en;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__MultiBitLatch__DOT__EN 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__5__KET____DOT__latch_en;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__MultiBitLatch__DOT__EN 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__6__KET____DOT__latch_en;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__MultiBitLatch__DOT__EN 
            = vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMem__BRA__7__KET____DOT__latch_en;
    }
    if ((0x000000000000001cULL & vlSelfRef.__VnbaTriggered[0U])) {
        vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__PixelClkEn 
            = (1U & ((~ ((IData)(vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__Reset_b) 
                         & (IData)(vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__pix_state_idle))) 
                     | (IData)(vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__hit_rise)));
        vlSelfRef.PixelLogic__DOT__PixelClkEn = vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__PixelClkEn;
        vlSelfRef.PixelClkEn = vlSelfRef.PixelLogic__DOT__PixelClkEn;
    }
    if ((0x000000000001ffe1ULL & vlSelfRef.__VnbaTriggered[0U])) {
        vlSelfRef.PixelLogic__DOT__TotCounter__DOT__tot_cnt_stageone 
            = ((0x0000003eU & (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__init_tot_cnt)) 
               | (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__TotDualEdgeCount)
                         ? (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__fall_phase)
                         : (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__init_tot_cnt))));
        vlSelfRef.PixelLogic__DOT__TotCounter__DOT__tot_cnt_int 
            = (0x0000000fU & (((0U != (7U & ((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__tot_cnt_stageone) 
                                             >> 3U))) 
                               & (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__Tot6to4Mapping))
                               ? (8U | (7U & (((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__tot_cnt_stageone) 
                                               >> 2U) 
                                              - (IData)(2U))))
                               : (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__tot_cnt_stageone)));
        vlSelfRef.PixelLogic__DOT__TotCounter__DOT__TotOverflow 
            = (7U == (7U & ((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__tot_cnt_int) 
                            >> 1U)));
        vlSelfRef.PixelLogic__DOT__TotCounter__DOT__TotCnt 
            = ((7U == (7U & ((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__tot_cnt_int) 
                             >> 1U))) ? (0x0000000eU 
                                         & (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__tot_cnt_int))
                : (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__tot_cnt_int));
        vlSelfRef.PixelLogic__DOT__tot_overflow = vlSelfRef.PixelLogic__DOT__TotCounter__DOT__TotOverflow;
        vlSelfRef.PixelLogic__DOT__tot_cnt = vlSelfRef.PixelLogic__DOT__TotCounter__DOT__TotCnt;
        vlSelfRef.PixelLogic__DOT__PixelCTRL__DOT__TotOverflow 
            = vlSelfRef.PixelLogic__DOT__tot_overflow;
        vlSelfRef.PixelLogic__DOT__TotMemory__DOT__TotMemDataIn 
            = vlSelfRef.PixelLogic__DOT__tot_cnt;
    }
    if ((0x000000000001fff1ULL & vlSelfRef.__VnbaTriggered[0U])) {
        if (vlSelfRef.PixelLogic__DOT__TotCounter__DOT__Reset_b) {
            if ((((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__Hit) 
                  & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__TotOverflow))) 
                 & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__HitTe)))) {
                vlSelfRef.PixelLogic__DOT__TotCounter__DOT__cnt_clk 
                    = vlSelfRef.PixelLogic__DOT__TotCounter__DOT__PixelClkGated;
            }
        } else {
            vlSelfRef.PixelLogic__DOT__TotCounter__DOT__cnt_clk = 0U;
        }
        vlSelfRef.PixelLogic__DOT__TotCounter__DOT__fall_phase 
            = vlSelfRef.PixelLogic__DOT__TotCounter__DOT__cnt_clk;
        vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff0__DOT__nclk 
            = (1U & (~ (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__cnt_clk)));
        vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff1_clk 
            = (1U & ((IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__TotDualEdgeCount)
                      ? (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__cnt_clk)
                      : (IData)(vlSelfRef.PixelLogic__DOT__TotCounter__DOT__rcnt_q)));
        vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff1__DOT__nclk 
            = vlSelfRef.PixelLogic__DOT__TotCounter__DOT__ff1_clk;
    }
    if ((0x000000000003fff1ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vtop___024root___nba_comb__TOP__5(vlSelf);
    }
    if ((0x000000000003fff3ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vtop___024root___ico_comb__TOP__1(vlSelf);
    }
}

void Vtop___024root___trigger_orInto__act(VlUnpacked<QData/*63:0*/, 1> &out, const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___trigger_orInto__act\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        out[n] = (out[n] | in[n]);
        n = ((IData)(1U) + n);
    } while ((1U > n));
}

bool Vtop___024root___eval_phase__act(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_phase__act\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VactExecute;
    // Body
    Vtop___024root___eval_triggers__act(vlSelf);
    Vtop___024root___trigger_orInto__act(vlSelfRef.__VnbaTriggered, vlSelfRef.__VactTriggered);
    __VactExecute = Vtop___024root___trigger_anySet__act(vlSelfRef.__VactTriggered);
    if (__VactExecute) {
        Vtop___024root___eval_act(vlSelf);
    }
    return (__VactExecute);
}

void Vtop___024root___trigger_clear__act(VlUnpacked<QData/*63:0*/, 1> &out) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___trigger_clear__act\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        out[n] = 0ULL;
        n = ((IData)(1U) + n);
    } while ((1U > n));
}

bool Vtop___024root___eval_phase__nba(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_phase__nba\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VnbaExecute;
    // Body
    __VnbaExecute = Vtop___024root___trigger_anySet__act(vlSelfRef.__VnbaTriggered);
    if (__VnbaExecute) {
        Vtop___024root___eval_nba(vlSelf);
        Vtop___024root___trigger_clear__act(vlSelfRef.__VnbaTriggered);
    }
    return (__VnbaExecute);
}

void Vtop___024root___eval(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ __VicoIterCount;
    IData/*31:0*/ __VnbaIterCount;
    // Body
    __VicoIterCount = 0U;
    vlSelfRef.__VicoFirstIteration = 1U;
    do {
        if (VL_UNLIKELY(((0x00000064U < __VicoIterCount)))) {
#ifdef VL_DEBUG
            Vtop___024root___dump_triggers__ico(vlSelfRef.__VicoTriggered, "ico"s);
#endif
            VL_FATAL_MT("/home/grammy/lbnl/cocotb/testing_ITKpixel/rtl/PixelLogic.sv", 401, "", "DIDNOTCONVERGE: Input combinational region did not converge after '--converge-limit' of 100 tries");
        }
        __VicoIterCount = ((IData)(1U) + __VicoIterCount);
        vlSelfRef.__VicoPhaseResult = Vtop___024root___eval_phase__ico(vlSelf);
        vlSelfRef.__VicoFirstIteration = 0U;
    } while (vlSelfRef.__VicoPhaseResult);
    __VnbaIterCount = 0U;
    do {
        if (VL_UNLIKELY(((0x00000064U < __VnbaIterCount)))) {
#ifdef VL_DEBUG
            Vtop___024root___dump_triggers__act(vlSelfRef.__VnbaTriggered, "nba"s);
#endif
            VL_FATAL_MT("/home/grammy/lbnl/cocotb/testing_ITKpixel/rtl/PixelLogic.sv", 401, "", "DIDNOTCONVERGE: NBA region did not converge after '--converge-limit' of 100 tries");
        }
        __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
        vlSelfRef.__VactIterCount = 0U;
        do {
            if (VL_UNLIKELY(((0x00000064U < vlSelfRef.__VactIterCount)))) {
#ifdef VL_DEBUG
                Vtop___024root___dump_triggers__act(vlSelfRef.__VactTriggered, "act"s);
#endif
                VL_FATAL_MT("/home/grammy/lbnl/cocotb/testing_ITKpixel/rtl/PixelLogic.sv", 401, "", "DIDNOTCONVERGE: Active region did not converge after '--converge-limit' of 100 tries");
            }
            vlSelfRef.__VactIterCount = ((IData)(1U) 
                                         + vlSelfRef.__VactIterCount);
            vlSelfRef.__VactPhaseResult = Vtop___024root___eval_phase__act(vlSelf);
        } while (vlSelfRef.__VactPhaseResult);
        vlSelfRef.__VnbaPhaseResult = Vtop___024root___eval_phase__nba(vlSelf);
    } while (vlSelfRef.__VnbaPhaseResult);
}

#ifdef VL_DEBUG
void Vtop___024root___eval_debug_assertions(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_debug_assertions\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if (VL_UNLIKELY(((vlSelfRef.PixelClkGated & 0xfeU)))) {
        Verilated::overWidthError("PixelClkGated");
    }
    if (VL_UNLIKELY(((vlSelfRef.Reset_b & 0xfeU)))) {
        Verilated::overWidthError("Reset_b");
    }
    if (VL_UNLIKELY(((vlSelfRef.Tot6to4Mapping & 0xfeU)))) {
        Verilated::overWidthError("Tot6to4Mapping");
    }
    if (VL_UNLIKELY(((vlSelfRef.TotDualEdgeCount & 0xfeU)))) {
        Verilated::overWidthError("TotDualEdgeCount");
    }
    if (VL_UNLIKELY(((vlSelfRef.Hit & 0xfeU)))) {
        Verilated::overWidthError("Hit");
    }
    if (VL_UNLIKELY(((vlSelfRef.HitLeAny & 0xfeU)))) {
        Verilated::overWidthError("HitLeAny");
    }
}
#endif  // VL_DEBUG
