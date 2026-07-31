#include <cpu.h>
#include <bus.h>
#include <emu.h>

void cpu_set_flags(cpu_context *ctx,char z, char n, char h, char c){
    if(z != -1){
        BIT_SET(ctx->regs.f, 7, z);
    }
    if(n != -1){
        BIT_SET(ctx->regs.f, 6, n);
    }
    if(h != -1){
        BIT_SET(ctx->regs.f, 5, h);
    }
    if(c != -1){
        BIT_SET(ctx->regs.f, 4, c);
    }
}


static void proc_none(cpu_context *ctx){
    printf("Invalid Instruction!\n");
    exit(-7);
}

static bool check_cond(cpu_context *ctx){
    bool z = CPU_FLAG_Z;
    bool c = CPU_FLAG_C;

    switch (ctx->cur_inst->cond){
    case CT_NONE:
            return true;
    case CT_C:
        return c;
    case CT_NC:
        return !c;
    case CT_Z:
        return z;
    case CT_NZ:
        return !z;    
    default:
        printf("Invalid Flag!\n");
        break;
    }
    return false;
}

static void proc_jp(cpu_context *ctx){
    if(check_cond(ctx)){
        ctx->regs.PC = ctx->fetch_data;
        emu_cycles(1);
    }
}

static void proc_ld(cpu_context *ctx){
        if(ctx->dest_is_mem){
            
            if(ctx->cur_inst->reg_2 >= RT_AF){
                bus_write16(ctx->mem_dest, ctx->fetch_data);
                emu_cycles(1);
            }else{
                bus_write(ctx->mem_dest, ctx->fetch_data);
            }
            emu_cycles(1);
            return;
        }

        if(ctx->cur_inst->mode == AM_HL_SPR){
            u8 hflag = (cpu_read_reg(ctx->cur_inst->reg_2) & 0xF) + 
                (ctx->fetch_data & 0xF) >= 0x10;

            u8 cflag = (cpu_read_reg(ctx->cur_inst->reg_2) & 0xFF) + 
                (ctx->fetch_data & 0xFF) >= 0x100;
            
            cpu_set_flags(ctx, 0, 0, hflag, cflag);
            cpu_write_reg(ctx->cur_inst->reg_1, cpu_read_reg(ctx->cur_inst->reg_2) + 
                (char)ctx->fetch_data);
        }
        
        
        else{
            cpu_write_reg(ctx->cur_inst->reg_1, ctx->fetch_data);
        }
        return;
}

static void proc_add(cpu_context *ctx){
    reg_type rt = ctx->cur_inst->reg_1;
    u16 cur = cpu_read_reg(rt);
    u16 n = ctx->fetch_data;

    u16 result;
    char z, h, c;

    if(rt == RT_SP){

        result = cur + (int8_t)n;
        z = 0;
        h = (cur & 0xF)  + (n & 0xF)  >= 0x10;
        c = (cur & 0xFF) + (n & 0xFF) >= 0x100;
        emu_cycles(1);
    }
    else if(rt >= RT_AF){
        result = cur + n;
        z = -1;
        h = (cur & 0xFFF) + (n & 0xFFF) >= 0x1000;
        c = ((u32)cur + (u32)n) >= 0x10000;
        emu_cycles(1);
    }
    else{
        result = cur + n;
        z = (result & 0xFF) == 0;
        h = (cur & 0xF)  + (n & 0xF)  >= 0x10;
        c = (cur & 0xFF) + (n & 0xFF) >= 0x100;
    }

    cpu_write_reg(rt, result);
    cpu_set_flags(ctx, z, 0, h, c);
}

static void proc_adc(cpu_context *ctx){
        u8 a = ctx->regs.a;
        u8 n = ctx->fetch_data;
        u8 carry_in = (ctx->regs.f & 0x10) ? 1 : 0;
            
        u16 result = (u16)a +(u16)n + carry_in;

        bool z = ((u8)result) == 0;
        bool n_flag = false;
        bool h = ((a & 0xF) + (n & 0xF) + carry_in) > 0xF;
        bool c = result > 0xFF;
        ctx->regs.a = result;
        cpu_set_flags(ctx, z, n_flag, h, c);

        return;
}

static void proc_call(cpu_context *ctx){
        ctx->regs.SP--;
        bus_write(ctx->regs.SP, (ctx->regs.PC >> 8) &0xFF);
        emu_cycles(1);

        ctx->regs.SP--;
        bus_write(ctx->regs.SP, ctx->regs.PC & 0xFF);
        emu_cycles(1);

        ctx->regs.PC = ctx->fetch_data;
        emu_cycles(1);
        return;
}

static void proc_di(cpu_context *ctx){
    ctx->int_master_enabled = false;
}

static void proc_nop(cpu_context *ctx){
    return;
}


static void proc_xor(cpu_context *ctx){
    ctx->regs.a ^= ctx->fetch_data & 0xFF;
    cpu_set_flags(ctx, ctx->regs.a == 0, 0, 0, 0);
}


static IN_PROC processors[]= {

    [IN_NONE] = proc_none,
    [IN_LD] = proc_ld,
    [IN_JP] = proc_jp,
    [IN_ADD] = proc_add,
    [IN_ADC] = proc_adc,
    [IN_CALL] = proc_call,
    [IN_DI] = proc_di,
    [IN_NOP] = proc_nop,
    [IN_XOR] = proc_xor,

};


IN_PROC inst_get_processor(in_type type){
    return processors[type];
}
