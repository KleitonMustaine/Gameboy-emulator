#include <instructions.h>
#include <cpu.h>

instruction instructions[0x100] = {
    [0x00] = {IN_NOP, AM_IMP},
    [0x01] = {IN_LD, AM_R_D16, RT_BC},
    [0x06] = {IN_LD, AM_R_D8, RT_B},
    [0xAF] = {IN_XOR, AM_R, RT_A},


    //0x1X
    [0x11] = {IN_LD, AM_R_D16, RT_DE},
    [0x12] = {IN_LD, AM_MR_R, RT_DE, RT_A},
    [0x15] = {IN_DEC, AM_R, RT_D},
    [0x16] = {IN_LD, AM_R_D8, RT_D},
    [0x1A] = {IN_LD, AM_R_MR, RT_A, RT_DE},
    [0x1E] = {IN_LD, AM_R_D8, RT_E},

    //0x2X
    [0x20] = {IN_JR, AM_D8, RT_NONE, RT_NONE, CT_NZ},
    [0x21] = {IN_LD, AM_R_D16, RT_HL},
    [0x22] = {IN_LD, AM_HLI_R, RT_HL, RT_A},
    [0x23] = {IN_INC, AM_R, RT_HL},
    [0x24] = {IN_INC, AM_R, RT_H},
    [0x25] = {IN_DEC, AM_R, RT_H},
    [0x26] = {IN_LD, AM_R_D8, RT_H},
    [0x27] = {IN_DAA},
    [0x28] = {IN_JR, AM_D8, RT_NONE, RT_NONE, CT_Z},
    [0x29] = {IN_ADD, AM_R_R, RT_HL, RT_HL},
    [0x2A] = {IN_LD, AM_R_HLI, RT_A, RT_HL},
    [0x2B] = {IN_DEC, AM_R, RT_HL},
    [0x2C] = {IN_INC, AM_R, RT_L},
    [0x2D] = {IN_DEC, AM_R, RT_L},
    [0x2E] = {IN_LD, AM_R_D8, RT_L},
    [0x2F] = {IN_CPL},
    //0x3X
    [0x31] = {IN_LD, AM_R_D16, RT_SP},
    [0x32] = {IN_LD, AM_HLD_R, RT_HL, RT_A},
    [0x35] = {IN_DEC, AM_MR, RT_HL},
    [0x36] = {IN_LD, AM_MR_D8, RT_HL},
    [0x3A] = {IN_LD, AM_R_HLD, RT_A, RT_HL},
    [0x3E] = {IN_LD, AM_R_D8, RT_A},


    [0x05] = {IN_DEC, AM_R, RT_B},

    [0x0E] = {IN_LD, AM_R_D8, RT_C},

    [0xAF] = {IN_XOR, AM_R, RT_A},

    [0xC3] = {IN_JP, AM_D16},

    [0xCE] = {IN_ADC, AM_R_D8, RT_A},

    [0x0B] = {IN_DEC, AM_R, RT_BC},

    [0x66] = {IN_LD, AM_R_MR, RT_H, RT_HL},

    [0xF3] = {IN_DI},

    [0x31] = {IN_LD, AM_R_D16, RT_SP},

    [0x16] = {IN_LD, AM_R_D8, RT_D},

    [0xCD] = {IN_CALL, AM_D16},

    [0x10] = {IN_STOP},

    [0x57] = {IN_LD, AM_R_R, RT_D, RT_A},

    [0x48] = {IN_LD, AM_R_R, RT_C, RT_B},

    [0x4F] = {IN_LD, AM_R_R, RT_C, RT_C},

    //0xEX
    [0xE2] = {IN_LD, AM_MR_R, RT_C, RT_A},
    [0xEA] = {IN_LD, AM_A16_R, RT_NONE, RT_A},
    [0xEE] = {IN_XOR, AM_D8, RT_A},
    
    //0xFX
    [0xF2] = {IN_LD, AM_R_MR, RT_A, RT_C},
    [0xF3] = {IN_DI},
    [0xFA] = {IN_LD, AM_R_A16, RT_A},


}; 

static const reg_type reg_lookup[8] = {
    RT_B, RT_C, RT_D, RT_E, RT_H, RT_L, RT_HL, RT_A
};

void init_regular_inst(){

    for(int op = 0x40; op <= 0x7F; op++){

        if(op == 0x76){
            instructions[op] = (instruction) {IN_HALT};
            continue;
        }
        reg_type d = reg_lookup[(op >> 3) & 7];
        reg_type s = reg_lookup[op & 7];
        if(d == RT_HL){
            instructions[op] = (instruction) {IN_LD, AM_MR_R, RT_HL, s};
        }
        else if(s == RT_HL){
            instructions[op] = (instruction) {IN_LD, AM_R_MR, d, RT_HL};
        }
        else{
            instructions[op] = (instruction) {IN_LD, AM_R_R, d, s};
        }

    }

}

static const in_type alu[8] = {
    IN_ADD,IN_ADC,IN_SUB,IN_SBC,IN_AND,IN_XOR,IN_OR,IN_CP
};

void init_alu_inst(){

    for(int op = 0x80; op <= 0xBF; op++){
        in_type t = alu[(op >> 3) & 7];
        reg_type s = reg_lookup[op & 7];
        if(s == RT_HL){
            instructions[op] = (instruction){t, AM_R_MR, RT_A, RT_HL};
        }else{
            instructions[op] = (instruction){t, AM_R_R,  RT_A, s};
        }
    }

}

static char *inst_lookup[] = {
    "<NONE>", "NOP",  "LD",   "INC",  "DEC",  "RLCA", "ADD",  "RRCA",
    "STOP",   "RLA",  "JR",   "RRA",  "DAA",  "CPL",  "SCF",  "CCF",
    "HALT",   "ADC",  "SUB",  "SBC",  "AND",  "XOR",  "OR",   "CP",
    "POP",    "JP",   "PUSH", "RET",  "CB",   "CALL", "RETI", "LDH",
    "JPHL",   "DI",   "EI",   "RST",  "ERR",
    "RLC",    "RRC",  "RL",   "RR",   "SLA",  "SRA",  "SWAP", "SRL",
    "BIT",    "RES",  "SET"
};

static char *rt_lookup[] = {
    "<NONE>", "A", "F", "B", "C", "D", "E", "H", "L",
    "AF", "BC", "DE", "HL", "SP", "PC"
};

char *inst_name(in_type t)  { return inst_lookup[t]; }
char *reg_name(reg_type rt) { return rt_lookup[rt];  }

instruction *instruction_by_opcode(u8 opcode) {
    if(instructions[opcode].type == IN_NONE){
        return NULL;
    }
    return &instructions[opcode];
}

// os 11 opcodes que nao existem no hardware travam a CPU de verdade
static bool is_illegal(u8 op) {
    switch (op) {
        case 0xD3: case 0xDB: case 0xDD:
        case 0xE3: case 0xE4: case 0xEB: case 0xEC: case 0xED:
        case 0xF4: case 0xFC: case 0xFD:
            return true;
        default:
            return false;
    }
}

// nenhum mnemonico passa de 4 chars (RLCA, SWAP, JPHL...), entao celulas
// de 5 sempre deixam pelo menos um espaco entre colunas
#define CELL "%-5s"

void inst_coverage(void) {
    int feitos = 0;
    int total  = 0;

    printf("\n== cobertura de opcodes ==\n\n");

    printf("     ");
    for (int lo = 0; lo < 16; lo++) {
        printf("%-5X", lo);
    }
    printf("\n");

    for (int hi = 0; hi < 16; hi++) {
        printf("%X_   ", hi);

        for (int lo = 0; lo < 16; lo++) {
            u8 op = (hi << 4) | lo;

            if (is_illegal(op)) {
                printf(CELL, "--");
                continue;
            }

            total++;

            if (instructions[op].type != IN_NONE) {
                feitos++;
                printf(CELL, inst_name(instructions[op].type));
            } else {
                printf(CELL, ".");
            }
        }

        printf("\n");
    }

    printf("\n%d/%d implementados (%d%%)\n\n", feitos, total, feitos * 100 / total);
}