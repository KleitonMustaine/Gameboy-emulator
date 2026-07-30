#include <cpu.h>
#include <bus.h>

static cpu_context ctx;

static void fetch_instruction(void);
static void fetch_data(void);
static void execute(void);

void cpu_init(){


}
bool cpu_step(){

    u16 pc = ctx.regs.PC;

    fetch_instruction();
    fetch_data();

    printf("%04X:  %-4s %-9s  (%02X %02X %02X)  A:%02X F:%c%c%c%c "
           "BC:%02X%02X DE:%02X%02X HL:%02X%02X SP:%04X\n",
        pc,
        inst_name(ctx.cur_inst->type),
        "",                              // aqui vão os operandos, ver abaixo
        ctx.cur_opcode, bus_read(pc + 1), bus_read(pc + 2),
        ctx.regs.a,
        (ctx.regs.f & 0x80) ? 'Z' : '-',
        (ctx.regs.f & 0x40) ? 'N' : '-',
        (ctx.regs.f & 0x20) ? 'H' : '-',
        (ctx.regs.f & 0x10) ? 'C' : '-',
        ctx.regs.b, ctx.regs.c,
        ctx.regs.d, ctx.regs.e,
        ctx.regs.h, ctx.regs.l,
        ctx.regs.SP);

        if (ctx.cur_inst->type == IN_NONE) {
        printf("\n>>> opcode nao implementado: 0x%02X  em PC=0x%04X\n",
            ctx.cur_opcode, pc);
        printf("    tabela gbdev: linha %X_  coluna _%X\n",
            ctx.cur_opcode >> 4, ctx.cur_opcode & 0x0F);
        return false;

        return true;
}

    execute();


}