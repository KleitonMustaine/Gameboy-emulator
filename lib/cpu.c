#include <cpu.h>
#include <bus.h>
#include <emu.h>

cpu_context ctx;

void cpu_init(){

    ctx.regs.PC = 0x100;
    ctx.regs.a = 0x01;

}

static void fetch_instruction(){

    ctx.cur_opcode = bus_read(ctx.regs.PC++);
    ctx.cur_inst = instruction_by_opcode(ctx.cur_opcode);
}
static void execute(){

    IN_PROC proc = inst_get_processor(ctx.cur_inst->type);

    if(!proc){
        fflush(stdout);
        fprintf(stderr, "Sem processor para %s (opcode %02X) em PC=%04X\n",
            inst_name(ctx.cur_inst->type), ctx.cur_opcode, ctx.regs.PC - 1);
        exit(-5);
    }

    proc(&ctx);

}

bool cpu_step(){

    u16 pc = ctx.regs.PC;

    if(!ctx.halted){

        fetch_instruction();
        fetch_data();
    }
    printf("%04X: %7s (%02X %02X %02X) A: %02X B: %02X C: %02X\n",
        pc, inst_name(ctx.cur_inst->type), ctx.cur_opcode,
        bus_read(pc + 1), bus_read(pc + 2), ctx.regs.a, ctx.regs.b, ctx.regs.c);

    if (ctx.cur_inst->type == IN_NONE) {
        printf("\n>>> opcode nao implementado: 0x%02X  em PC=0x%04X\n",
            ctx.cur_opcode, pc);
        printf("    tabela gbdev: linha %X_  coluna _%X\n",
            ctx.cur_opcode >> 4, ctx.cur_opcode & 0x0F);
        return false;


}

    execute();

    return true;


}