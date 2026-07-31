#include <instructions.h>
#include <cpu.h>

instruction instructions[0x100] = {
    [0x00] = {IN_NOP, AM_IMP},

    [0x01] = {IN_LD, AM_R_D16, RT_BC},

    [0x05] = {IN_DEC, AM_R, RT_B},

    [0x0E] = {IN_LD, AM_R_D8, RT_C},

    [0xAF] = {IN_XOR, AM_R, RT_A},

    [0xC3] = {IN_JP, AM_D16},

    [0xCE] = {IN_ADC, AM_R_D8, RT_A},

    [0x0B] = {IN_DEC, AM_MR, RT_BC},

    [0x66] = {IN_LD, AM_R_MR, RT_H, RT_HL},

    [0xF3] = {IN_DI},

    [0x31] = {IN_LD, AM_R_D16, RT_SP},

    [0x16] = {IN_LD, AM_R_D8, RT_D},

    [0xCD] = {IN_CALL, AM_D16},



}; 

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

void inst_coverage(void) {
    int feitos = 0;
    int total  = 0;

    printf("\n== cobertura de opcodes ==\n");
    printf("    0 1 2 3 4 5 6 7 8 9 A B C D E F\n");

    for (int hi = 0; hi < 16; hi++) {
        printf("%X_  ", hi);

        for (int lo = 0; lo < 16; lo++) {
            u8 op = (hi << 4) | lo;

            if (is_illegal(op)) {
                printf("x ");
                continue;
            }

            total++;

            if (instructions[op].type != IN_NONE) {
                feitos++;
                printf("# ");
            } else {
                printf(". ");
            }
        }

        printf("\n");
    }

    printf("\n%d/%d implementados (%d%%)\n\n", feitos, total, feitos * 100 / total);
}