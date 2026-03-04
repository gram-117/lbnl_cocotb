// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table internal header
//
// Internal details; most calling programs do not need this header,
// unless using verilator public meta comments.

#ifndef VERILATED_VTOP__SYMS_H_
#define VERILATED_VTOP__SYMS_H_  // guard

#include "verilated.h"

// INCLUDE MODEL CLASS

#include "Vtop.h"

// INCLUDE MODULE CLASSES
#include "Vtop___024root.h"

// DPI TYPES for DPI Export callbacks (Internal use)

// SYMS CLASS (contains all model state)
class alignas(VL_CACHE_LINE_BYTES) Vtop__Syms final : public VerilatedSyms {
  public:
    // INTERNAL STATE
    Vtop* const __Vm_modelp;
    VlDeleter __Vm_deleter;
    bool __Vm_didInit = false;

    // MODULE INSTANCE STATE
    Vtop___024root                 TOP;

    // SCOPE NAMES
    VerilatedScope* __Vscopep_PixelLogic;
    VerilatedScope* __Vscopep_PixelLogic__PixelCTRL;
    VerilatedScope* __Vscopep_PixelLogic__PixelCTRL__CG_tot_rst;
    VerilatedScope* __Vscopep_PixelLogic__TotCounter;
    VerilatedScope* __Vscopep_PixelLogic__TotCounter__ff0;
    VerilatedScope* __Vscopep_PixelLogic__TotCounter__ff1;
    VerilatedScope* __Vscopep_PixelLogic__TotCounter__ff2;
    VerilatedScope* __Vscopep_PixelLogic__TotCounter__ff3;
    VerilatedScope* __Vscopep_PixelLogic__TotCounter__ff4;
    VerilatedScope* __Vscopep_PixelLogic__TotCounter__ff5;
    VerilatedScope* __Vscopep_PixelLogic__TotMemory;
    VerilatedScope* __Vscopep_PixelLogic__TotMemory__TotMem__BRA__0__KET__;
    VerilatedScope* __Vscopep_PixelLogic__TotMemory__TotMem__BRA__0__KET____MultiBitLatch;
    VerilatedScope* __Vscopep_PixelLogic__TotMemory__TotMem__BRA__1__KET__;
    VerilatedScope* __Vscopep_PixelLogic__TotMemory__TotMem__BRA__1__KET____MultiBitLatch;
    VerilatedScope* __Vscopep_PixelLogic__TotMemory__TotMem__BRA__2__KET__;
    VerilatedScope* __Vscopep_PixelLogic__TotMemory__TotMem__BRA__2__KET____MultiBitLatch;
    VerilatedScope* __Vscopep_PixelLogic__TotMemory__TotMem__BRA__3__KET__;
    VerilatedScope* __Vscopep_PixelLogic__TotMemory__TotMem__BRA__3__KET____MultiBitLatch;
    VerilatedScope* __Vscopep_PixelLogic__TotMemory__TotMem__BRA__4__KET__;
    VerilatedScope* __Vscopep_PixelLogic__TotMemory__TotMem__BRA__4__KET____MultiBitLatch;
    VerilatedScope* __Vscopep_PixelLogic__TotMemory__TotMem__BRA__5__KET__;
    VerilatedScope* __Vscopep_PixelLogic__TotMemory__TotMem__BRA__5__KET____MultiBitLatch;
    VerilatedScope* __Vscopep_PixelLogic__TotMemory__TotMem__BRA__6__KET__;
    VerilatedScope* __Vscopep_PixelLogic__TotMemory__TotMem__BRA__6__KET____MultiBitLatch;
    VerilatedScope* __Vscopep_PixelLogic__TotMemory__TotMem__BRA__7__KET__;
    VerilatedScope* __Vscopep_PixelLogic__TotMemory__TotMem__BRA__7__KET____MultiBitLatch;
    VerilatedScope* __Vscopep_TOP;

    // SCOPE HIERARCHY
    VerilatedHierarchy __Vhier;

    // CONSTRUCTORS
    Vtop__Syms(VerilatedContext* contextp, const char* namep, Vtop* modelp);
    ~Vtop__Syms();

    // METHODS
    const char* name() const { return TOP.vlNamep; }
};

#endif  // guard
