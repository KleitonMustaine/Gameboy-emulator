#include <cpu.h>

extern cpu_context ctx;

u16 reverse(u16 n) {
    return ((n & 0xFF00) >> 8) | ((n & 0x00FF) << 8);
}

u16 cpu_read_reg(reg_type rt) {
    switch(rt) {
        case RT_A: return ctx.regs.a;
        case RT_F: return ctx.regs.f;
        case RT_B: return ctx.regs.b;
        case RT_C: return ctx.regs.c;
        case RT_D: return ctx.regs.d;
        case RT_E: return ctx.regs.e;
        case RT_H: return ctx.regs.h;
        case RT_L: return ctx.regs.l;

        case RT_AF: return reverse(*((u16 *)&ctx.regs.a));
        case RT_BC: return reverse(*((u16 *)&ctx.regs.b));
        case RT_DE: return reverse(*((u16 *)&ctx.regs.d));
        case RT_HL: return reverse(*((u16 *)&ctx.regs.h));

        case RT_PC: return ctx.regs.PC;
        case RT_SP: return ctx.regs.SP;
        default: return 0;
    }
}

void cpu_write_reg(reg_type rt, u16 val) {
    switch(rt) {
        case RT_A: ctx.regs.a = val & 0xFF; return;
        case RT_F: ctx.regs.f = val & 0xFF; return;
        case RT_B: ctx.regs.b = val & 0xFF; return;
        case RT_C: ctx.regs.c = val & 0xFF; return;
        case RT_D: ctx.regs.d = val & 0xFF; return;
        case RT_E: ctx.regs.e = val & 0xFF; return;
        case RT_H: ctx.regs.h = val & 0xFF; return;
        case RT_L: ctx.regs.l = val & 0xFF; return;

        case RT_AF: ctx.regs.a = (val >> 8) & 0xFF; ctx.regs.f = val & 0xFF; return;
        case RT_BC: ctx.regs.b = (val >> 8) & 0xFF; ctx.regs.c = val & 0xFF; return;
        case RT_DE: ctx.regs.d = (val >> 8) & 0xFF; ctx.regs.e = val & 0xFF; return;
        case RT_HL: ctx.regs.h = (val >> 8) & 0xFF; ctx.regs.l = val & 0xFF; return;

        case RT_PC: ctx.regs.PC = val; return;
        case RT_SP: ctx.regs.SP = val; return;
        default: return;
    }
}