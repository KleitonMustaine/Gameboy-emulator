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
        case AM_R_D16:
            u16 lo = bus_read(ctx.regs.PC);
            emu_cycles(1);

            u16 hi = bus_read(ctx.regs.PC + 1);
            emu_cycles(1);

            ctx.fetch_data = lo | (hi << 8);

            ctx.regs.PC +=2;

            return;
        
        
        case AM_R_MR:
            ctx.fetch_data = bus_read(cpu_read_reg(ctx.cur_inst->reg_2));
            emu_cycles(1);
            return;


        default:
            printf("Unknown addressing mode! %d\n", ctx.cur_inst->mode);
            exit(-7);
            return;
    }

}
static void execute(){

    switch (ctx.cur_inst->type){
    case IN_LD:
        if(ctx.dest_is_mem){
            bus_write(ctx.mem_dest, ctx.fetch_data);
        }else{
            cpu_write_reg(ctx.cur_inst->reg_1, ctx.fetch_data);
        }
        return;
    
    case IN_NOP:
        return;
    
    case IN_ADC:
        u8 a = ctx.regs.a;
        u8 n = ctx.fetch_data;
        u8 carry_in = (ctx.regs.f & 0x10) ? 1 : 0;
            
        u16 result = (u16)a +(u16)n + carry_in;

        bool z = ((u8)result) == 0;
        bool n_flag = false;
        bool h = ((a & 0xF) + (n & 0xF) + carry_in) > 0xF;
        bool c = result > 0xFF;

        return;

    case IN_JP:
        ctx.regs.PC = ctx.fetch_data;
        emu_cycles(1);
        return;

    case IN_DI:
        return;
    
    case IN_CALL:
        ctx.regs.SP--;
        bus_write(ctx.regs.SP, (ctx.regs.PC >> 8) &0xFF);
        emu_cycles(1);

        ctx.regs.SP--;
        bus_write(ctx.regs.SP, ctx.regs.PC & 0xFF);
        emu_cycles(1);

        ctx.regs.PC = ctx.fetch_data;
        emu_cycles(1);
        return;
    
    default:
            printf("Instrucao nao implementada no execute! %s\n", inst_name(ctx.cur_inst->type));
            while(1) { delay(1000); }
    }


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