#include <cpu.h>
#include <bus.h>
#include <emu.h>

cpu_context ctx;

void cpu_init(){

    ctx.regs.PC = 0x100;

}

static void fetch_instruction(){

    ctx.cur_opcode = bus_read(ctx.regs.PC++);
    ctx.cur_inst = instruction_by_opcode(ctx.cur_opcode);

    if(ctx.cur_inst == NULL){
        printf("Unknown Instruction! %02X\n",ctx.cur_opcode);
        while(1) { delay(1000); }
    }

}
static void fetch_data(){
    ctx.mem_dest = 0;
    ctx.dest_is_mem = false;

    switch(ctx.cur_inst->mode){
        case AM_IMP:
            return;

        case AM_R:
            ctx.fetch_data = cpu_read_reg(ctx.cur_inst->reg_1);
            return;

        case AM_R_D8:
            ctx.fetch_data = bus_read(ctx.regs.PC);
            emu_cycles(1);
            ctx.regs.PC++;
            return;

        case AM_D16:
            u16 lo = bus_read(ctx.regs.PC +1);
            emu_cycles(1);

            u16 hi = bus_read(ctx.regs.PC +1);
            emu_cycles(1);

            ctx.fetch_data = lo | (hi << 8);

            ctx.regs.PC +=2;

            return;
        
        default:
            printf("Unknown addressing mode! %d\n", ctx.cur_inst->mode);
            exit(-7);
            return;
    }

}
static void execute(){
    printf("Executing instruction: %02X    PC:  %04X\n", ctx.cur_opcode,ctx.regs.PC);
    printf("SHIT");
}



bool cpu_step(){

    u16 pc = ctx.regs.PC;

    if(!ctx.halted){

        fetch_instruction();
        fetch_data();
    }


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


}

    execute();

    return true;


}