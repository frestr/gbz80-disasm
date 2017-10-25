#include <stdlib.h>
#include <stdio.h>
#include "disassembler.h"

static void print_line(char *op, uint8_t *code, uint8_t op_bytes, uint32_t pc)
{
    printf("0x%06x:    ", pc);
    printf("%02x ", code[0]);
    op_bytes > 1 ? printf("%02x ", code[1]) : printf("   ");
    op_bytes > 2 ? printf("%02x ", code[2]) : printf("   ");
    printf("    %s\n", op);
}

// Defined under disassemble
static void extended_ops(uint8_t code, char *op);

uint8_t disassemble(uint8_t *buf, uint32_t pc)
{
    uint8_t *code = &buf[pc];
    uint8_t op_bytes = 1; 
    char op[32];

    switch(code[0]) {
        case 0x00: snprintf(op, 32, "NOP"); break;
        case 0x01: snprintf(op, 32, "LD     BC,$%X%X", code[2], code[1]); op_bytes = 3; break;
        case 0x02: snprintf(op, 32, "LD     (BC),A"); break;
        case 0x03: snprintf(op, 32, "INC    BC"); break;
        case 0x04: snprintf(op, 32, "INC    B"); break;
        case 0x05: snprintf(op, 32, "DEC    B"); break;
        case 0x06: snprintf(op, 32, "LD     B,$%X", code[1]); op_bytes = 2; break;
        case 0x07: snprintf(op, 32, "RLC    A"); break;
        case 0x08: snprintf(op, 32, "LD     ($%X%X),SP", code[2], code[1]); op_bytes = 3; break;
        case 0x09: snprintf(op, 32, "ADD    HL,BC"); break;
        case 0x0A: snprintf(op, 32, "LD     A,(BC)"); break;
        case 0x0B: snprintf(op, 32, "DEC    BC"); break;
        case 0x0C: snprintf(op, 32, "INC    C"); break;
        case 0x0D: snprintf(op, 32, "DEC    C"); break;
        case 0x0E: snprintf(op, 32, "LD     C,$%X", code[1]); op_bytes = 2; break;
        case 0x0F: snprintf(op, 32, "RRC    A"); break;

        case 0x10: snprintf(op, 32, "STOP"); op_bytes = 2; break;
        case 0x11: snprintf(op, 32, "LD     DE,$%X%X", code[2], code[1]); op_bytes = 3; break;
        case 0x12: snprintf(op, 32, "LD     (DE),A"); break;
        case 0x13: snprintf(op, 32, "INC    DE"); break;
        case 0x14: snprintf(op, 32, "INC    D"); break;
        case 0x15: snprintf(op, 32, "DEC    D"); break;
        case 0x16: snprintf(op, 32, "LD     D,$%X", code[1]); op_bytes = 2; break;
        case 0x17: snprintf(op, 32, "RL     A"); break;
        case 0x18: snprintf(op, 32, "JR     %X", code[1]); op_bytes = 2; break;
        case 0x19: snprintf(op, 32, "ADD    HL,DE"); break;
        case 0x1A: snprintf(op, 32, "LD     A,(DE)"); break;
        case 0x1B: snprintf(op, 32, "DEC    DE"); break;
        case 0x1C: snprintf(op, 32, "INC    E"); break;
        case 0x1D: snprintf(op, 32, "DEC    E"); break;
        case 0x1E: snprintf(op, 32, "LD     E,$%X", code[1]); op_bytes = 2; break;
        case 0x1F: snprintf(op, 32, "RR     A"); break;

        case 0x20: snprintf(op, 32, "JR     NZ,$%X", code[1]); op_bytes = 2; break;
        case 0x21: snprintf(op, 32, "LD     HL,$%X%X", code[2], code[1]); op_bytes = 3; break;
        case 0x22: snprintf(op, 32, "LDI    (HL),A"); break;
        case 0x23: snprintf(op, 32, "INC    HL"); break;
        case 0x24: snprintf(op, 32, "INC    H"); break;
        case 0x25: snprintf(op, 32, "DEC    H"); break;
        case 0x26: snprintf(op, 32, "LD     H,$%X", code[1]); op_bytes = 2; break;
        case 0x27: snprintf(op, 32, "DAA"); break;
        case 0x28: snprintf(op, 32, "JR     Z,$%X", code[1]); break;
        case 0x29: snprintf(op, 32, "ADD    HL,HL"); break;
        case 0x2A: snprintf(op, 32, "LDI    A,(HL)"); break;
        case 0x2B: snprintf(op, 32, "DEC    HL"); break;
        case 0x2C: snprintf(op, 32, "INC    L"); break;
        case 0x2D: snprintf(op, 32, "DEC    L"); break;
        case 0x2E: snprintf(op, 32, "LD     L,$%X", code[1]); op_bytes = 2; break;
        case 0x2F: snprintf(op, 32, "CPL"); break;

        case 0x30: snprintf(op, 32, "JR     NC,$%X", code[1]); op_bytes = 2; break;
        case 0x31: snprintf(op, 32, "LD     SP,$%X%X", code[2], code[1]); op_bytes = 3; break;
        case 0x32: snprintf(op, 32, "LDD    (HL),A"); break;
        case 0x33: snprintf(op, 32, "INC    SP"); break;
        case 0x34: snprintf(op, 32, "INC    (HL)"); break;
        case 0x35: snprintf(op, 32, "DEC    (HL)"); break;
        case 0x36: snprintf(op, 32, "LD     (HL),$%X", code[1]); op_bytes = 2; break;
        case 0x37: snprintf(op, 32, "SCF"); break;
        case 0x38: snprintf(op, 32, "JR     C,$%X", code[1]); op_bytes = 2; break;
        case 0x39: snprintf(op, 32, "ADD    HL,SP"); break;
        case 0x3A: snprintf(op, 32, "LDD    A,(HL)"); break;
        case 0x3B: snprintf(op, 32, "DEC    SP"); break;
        case 0x3C: snprintf(op, 32, "INC    A"); break;
        case 0x3D: snprintf(op, 32, "DEC    A"); break;
        case 0x3E: snprintf(op, 32, "LD     A,$%X", code[1]); op_bytes = 2; break;
        case 0x3F: snprintf(op, 32, "CCF"); break;

        case 0x40: snprintf(op, 32, "LD     B,B"); break;
        case 0x41: snprintf(op, 32, "LD     B,C"); break;
        case 0x42: snprintf(op, 32, "LD     B,D"); break;
        case 0x43: snprintf(op, 32, "LD     B,E"); break;
        case 0x44: snprintf(op, 32, "LD     B,H"); break;
        case 0x45: snprintf(op, 32, "LD     B,L"); break;
        case 0x46: snprintf(op, 32, "LD     B,(HL)"); break;
        case 0x47: snprintf(op, 32, "LD     B,A"); break;
        case 0x48: snprintf(op, 32, "LD     C,B"); break;
        case 0x49: snprintf(op, 32, "LD     C,C"); break;
        case 0x4A: snprintf(op, 32, "LD     C,D"); break;
        case 0x4B: snprintf(op, 32, "LD     C,E"); break;
        case 0x4C: snprintf(op, 32, "LD     C,H"); break;
        case 0x4D: snprintf(op, 32, "LD     C,L"); break;
        case 0x4E: snprintf(op, 32, "LD     C,(HL)"); break;
        case 0x4F: snprintf(op, 32, "LD     C,A"); break;

        case 0x50: snprintf(op, 32, "LD     D,B"); break;
        case 0x51: snprintf(op, 32, "LD     D,C"); break;
        case 0x52: snprintf(op, 32, "LD     D,D"); break;
        case 0x53: snprintf(op, 32, "LD     D,E"); break;
        case 0x54: snprintf(op, 32, "LD     D,H"); break;
        case 0x55: snprintf(op, 32, "LD     D,L"); break;
        case 0x56: snprintf(op, 32, "LD     D,(HL)"); break;
        case 0x57: snprintf(op, 32, "LD     D,A"); break;
        case 0x58: snprintf(op, 32, "LD     E,B"); break;
        case 0x59: snprintf(op, 32, "LD     E,C"); break;
        case 0x5A: snprintf(op, 32, "LD     E,D"); break;
        case 0x5B: snprintf(op, 32, "LD     E,E"); break;
        case 0x5C: snprintf(op, 32, "LD     E,H"); break;
        case 0x5D: snprintf(op, 32, "LD     E,L"); break;
        case 0x5E: snprintf(op, 32, "LD     E,(HL)"); break;
        case 0x5F: snprintf(op, 32, "LD     E,A"); break;

        case 0x60: snprintf(op, 32, "LD     H,B"); break;
        case 0x61: snprintf(op, 32, "LD     H,C"); break;
        case 0x62: snprintf(op, 32, "LD     H,D"); break;
        case 0x63: snprintf(op, 32, "LD     H,E"); break;
        case 0x64: snprintf(op, 32, "LD     H,H"); break;
        case 0x65: snprintf(op, 32, "LD     H,L"); break;
        case 0x66: snprintf(op, 32, "LD     H,(HL)"); break;
        case 0x67: snprintf(op, 32, "LD     H,A"); break;
        case 0x68: snprintf(op, 32, "LD     L,B"); break;
        case 0x69: snprintf(op, 32, "LD     L,C"); break;
        case 0x6A: snprintf(op, 32, "LD     L,D"); break;
        case 0x6B: snprintf(op, 32, "LD     L,E"); break;
        case 0x6C: snprintf(op, 32, "LD     L,H"); break;
        case 0x6D: snprintf(op, 32, "LD     L,L"); break;
        case 0x6E: snprintf(op, 32, "LD     L,(HL)"); break;
        case 0x6F: snprintf(op, 32, "LD     L,A"); break;

        case 0x70: snprintf(op, 32, "LD     (HL),B"); break;
        case 0x71: snprintf(op, 32, "LD     (HL),C"); break;
        case 0x72: snprintf(op, 32, "LD     (HL),D"); break;
        case 0x73: snprintf(op, 32, "LD     (HL),E"); break;
        case 0x74: snprintf(op, 32, "LD     (HL),H"); break;
        case 0x75: snprintf(op, 32, "LD     (HL),L"); break;
        case 0x76: snprintf(op, 32, "HAL    "); break;
        case 0x77: snprintf(op, 32, "LD     (HL),A"); break;
        case 0x78: snprintf(op, 32, "LD     A,B"); break;
        case 0x79: snprintf(op, 32, "LD     A,C"); break;
        case 0x7A: snprintf(op, 32, "LD     A,D"); break;
        case 0x7B: snprintf(op, 32, "LD     A,E"); break;
        case 0x7C: snprintf(op, 32, "LD     A,H"); break;
        case 0x7D: snprintf(op, 32, "LD     A,L"); break;
        case 0x7E: snprintf(op, 32, "LD     A,(HL)"); break;
        case 0x7F: snprintf(op, 32, "LD     A,A"); break;

        case 0x80: snprintf(op, 32, "ADD    A,B"); break;
        case 0x81: snprintf(op, 32, "ADD    A,C"); break;
        case 0x82: snprintf(op, 32, "ADD    A,D"); break;
        case 0x83: snprintf(op, 32, "ADD    A,E"); break;
        case 0x84: snprintf(op, 32, "ADD    A,H"); break;
        case 0x85: snprintf(op, 32, "ADD    A,L"); break;
        case 0x86: snprintf(op, 32, "ADD    A,(HL)"); break;
        case 0x87: snprintf(op, 32, "ADD    A,A"); break;
        case 0x88: snprintf(op, 32, "ADC    A,B"); break;
        case 0x89: snprintf(op, 32, "ADC    A,C"); break;
        case 0x8A: snprintf(op, 32, "ADC    A,D"); break;
        case 0x8B: snprintf(op, 32, "ADC    A,E"); break;
        case 0x8C: snprintf(op, 32, "ADC    A,H"); break;
        case 0x8D: snprintf(op, 32, "ADC    A,L"); break;
        case 0x8E: snprintf(op, 32, "ADC    A,(HL)"); break;
        case 0x8F: snprintf(op, 32, "ADC    A,A"); break;

        case 0x90: snprintf(op, 32, "SUB    A,B"); break;
        case 0x91: snprintf(op, 32, "SUB    A,C"); break;
        case 0x92: snprintf(op, 32, "SUB    A,D"); break;
        case 0x93: snprintf(op, 32, "SUB    A,E"); break;
        case 0x94: snprintf(op, 32, "SUB    A,H"); break;
        case 0x95: snprintf(op, 32, "SUB    A,L"); break;
        case 0x96: snprintf(op, 32, "SUB    A,(HL)"); break;
        case 0x97: snprintf(op, 32, "SUB    A,A"); break;
        case 0x98: snprintf(op, 32, "SBC    A,B"); break;
        case 0x99: snprintf(op, 32, "SBC    A,C"); break;
        case 0x9A: snprintf(op, 32, "SBC    A,D"); break;
        case 0x9B: snprintf(op, 32, "SBC    A,E"); break;
        case 0x9C: snprintf(op, 32, "SBC    A,H"); break;
        case 0x9D: snprintf(op, 32, "SBC    A,L"); break;
        case 0x9E: snprintf(op, 32, "SBC    A,(HL)"); break;
        case 0x9F: snprintf(op, 32, "SBC    A,A"); break;

        case 0xA0: snprintf(op, 32, "AND    B"); break;
        case 0xA1: snprintf(op, 32, "AND    C"); break;
        case 0xA2: snprintf(op, 32, "AND    D"); break;
        case 0xA3: snprintf(op, 32, "AND    E"); break;
        case 0xA4: snprintf(op, 32, "AND    H"); break;
        case 0xA5: snprintf(op, 32, "AND    L"); break;
        case 0xA6: snprintf(op, 32, "AND    (HL)"); break;
        case 0xA7: snprintf(op, 32, "AND    A"); break;
        case 0xA8: snprintf(op, 32, "XOR    B"); break;
        case 0xA9: snprintf(op, 32, "XOR    C"); break;
        case 0xAA: snprintf(op, 32, "XOR    D"); break;
        case 0xAB: snprintf(op, 32, "XOR    E"); break;
        case 0xAC: snprintf(op, 32, "XOR    H"); break;
        case 0xAD: snprintf(op, 32, "XOR    L"); break;
        case 0xAE: snprintf(op, 32, "XOR    (HL)"); break;
        case 0xAF: snprintf(op, 32, "XOR    A"); break;

        case 0xB0: snprintf(op, 32, "OR     B"); break;
        case 0xB1: snprintf(op, 32, "OR     C"); break;
        case 0xB2: snprintf(op, 32, "OR     D"); break;
        case 0xB3: snprintf(op, 32, "OR     E"); break;
        case 0xB4: snprintf(op, 32, "OR     H"); break;
        case 0xB5: snprintf(op, 32, "OR     L"); break;
        case 0xB6: snprintf(op, 32, "OR     (HL)"); break;
        case 0xB7: snprintf(op, 32, "OR     A"); break;
        case 0xB8: snprintf(op, 32, "CP     B"); break;
        case 0xB9: snprintf(op, 32, "CP     C"); break;
        case 0xBA: snprintf(op, 32, "CP     D"); break;
        case 0xBB: snprintf(op, 32, "CP     E"); break;
        case 0xBC: snprintf(op, 32, "CP     H"); break;
        case 0xBD: snprintf(op, 32, "CP     L"); break;
        case 0xBE: snprintf(op, 32, "CP     (HL)"); break;
        case 0xBF: snprintf(op, 32, "CP     A"); break;

        case 0xC0: snprintf(op, 32, "RET    NZ"); break;
        case 0xC1: snprintf(op, 32, "POP    BC"); break;
        case 0xC2: snprintf(op, 32, "JP     NZ,$%X%X", code[2], code[1]); op_bytes = 3; break;
        case 0xC3: snprintf(op, 32, "JP     $%X%X", code[2], code[1]); op_bytes = 3; break;
        case 0xC4: snprintf(op, 32, "CALL   NZ,$%X%X", code[2], code[1]); op_bytes = 3; break;
        case 0xC5: snprintf(op, 32, "PUSH   BC"); break;
        case 0xC6: snprintf(op, 32, "ADD    A,$%X", code[1]); op_bytes = 2; break;
        case 0xC7: snprintf(op, 32, "RST    0"); break;
        case 0xC8: snprintf(op, 32, "RET    Z"); break;
        case 0xC9: snprintf(op, 32, "RET"); break;
        case 0xCA: snprintf(op, 32, "JP     Z,$%X%X", code[2], code[1]); op_bytes = 3; break;
        case 0xCB: extended_ops(code[1], op); op_bytes = 2; break;
        case 0xCC: snprintf(op, 32, "CALL   Z,$%X%X", code[2], code[1]); op_bytes = 3; break;
        case 0xCD: snprintf(op, 32, "CALL   $%X%X", code[2], code[1]); op_bytes = 3; break;
        case 0xCE: snprintf(op, 32, "ADC    A,$%X", code[1]); op_bytes = 2; break;
        case 0xCF: snprintf(op, 32, "RST    8"); break;

        case 0xD0: snprintf(op, 32, "RET    NC"); break;
        case 0xD1: snprintf(op, 32, "POP    DE"); break;
        case 0xD2: snprintf(op, 32, "JP     NC,$%X%X", code[2], code[1]); op_bytes = 3; break;
        case 0xD3: snprintf(op, 32, "XX"); break;
        case 0xD4: snprintf(op, 32, "CALL   NC,$%X%X", code[2], code[1]); op_bytes = 3; break;
        case 0xD5: snprintf(op, 32, "PUSH   DE"); break;
        case 0xD6: snprintf(op, 32, "SUB    A,$%X", code[1]); op_bytes = 2; break;
        case 0xD7: snprintf(op, 32, "RST    10"); break;
        case 0xD8: snprintf(op, 32, "RET    C"); break;
        case 0xD9: snprintf(op, 32, "RETI"); break;
        case 0xDA: snprintf(op, 32, "JP     C,$%X%X", code[2], code[1]); op_bytes = 3; break;
        case 0xDB: snprintf(op, 32, "XX"); break;
        case 0xDC: snprintf(op, 32, "CALL   C,$%X%X", code[2], code[1]); op_bytes = 3; break;
        case 0xDD: snprintf(op, 32, "XX"); break;
        case 0xDE: snprintf(op, 32, "SBC    A,$%X", code[1]); op_bytes = 2; break;
        case 0xDF: snprintf(op, 32, "RST    18"); break;

        case 0xE0: snprintf(op, 32, "LDH    ($%X),A", code[1]); op_bytes = 2; break;
        case 0xE1: snprintf(op, 32, "POP    HL"); break;
        case 0xE2: snprintf(op, 32, "LDH    (C),A"); break;
        case 0xE3: snprintf(op, 32, "XX"); break;
        case 0xE4: snprintf(op, 32, "XX"); break;
        case 0xE5: snprintf(op, 32, "PUSH   HL"); break;
        case 0xE6: snprintf(op, 32, "AND    $%X", code[1]); op_bytes = 2; break;
        case 0xE7: snprintf(op, 32, "RST    20"); break;
        case 0xE8: snprintf(op, 32, "ADD    SP,$%X", code[1]); op_bytes = 2; break;
        case 0xE9: snprintf(op, 32, "JP     (HL)"); break;
        case 0xEA: snprintf(op, 32, "LD     ($%X%X),A", code[2], code[1]); op_bytes = 3; break;
        case 0xEB: snprintf(op, 32, "XX"); break;
        case 0xEC: snprintf(op, 32, "XX"); break;
        case 0xED: snprintf(op, 32, "XX"); break;
        case 0xEE: snprintf(op, 32, "XOR    $%X", code[1]); op_bytes = 2; break;
        case 0xEF: snprintf(op, 32, "RST    28"); break;

        case 0xF0: snprintf(op, 32, "LDH    A,$%X", code[1]); op_bytes = 2; break;
        case 0xF1: snprintf(op, 32, "POP    AF"); break;
        case 0xF2: snprintf(op, 32, "XX"); break;
        case 0xF3: snprintf(op, 32, "DI"); break;
        case 0xF4: snprintf(op, 32, "XX"); break;
        case 0xF5: snprintf(op, 32, "PUSH   AF"); break;
        case 0xF6: snprintf(op, 32, "OR     $%X", code[1]); op_bytes = 2; break;
        case 0xF7: snprintf(op, 32, "RST    30"); break;
        case 0xF8: snprintf(op, 32, "LDHL   SP,$%X", code[1]); op_bytes = 2; break;
        case 0xF9: snprintf(op, 32, "LD     SP,HL"); break;
        case 0xFA: snprintf(op, 32, "LD     A,$%X%X", code[2], code[1]); op_bytes = 3; break;
        case 0xFB: snprintf(op, 32, "EI"); break;
        case 0xFC: snprintf(op, 32, "XX"); break;
        case 0xFD: snprintf(op, 32, "XX"); break;
        case 0xFE: snprintf(op, 32, "CP     $%X", code[1]); op_bytes = 2; break;
        case 0xFF: snprintf(op, 32, "RST    38"); break;
    }

    print_line(op, code, op_bytes, pc);
    return op_bytes;
}

static void extended_ops(uint8_t code, char *op)
{
    switch (code) {
        case 0x00: snprintf(op, 32, "RLC    B"); break;
        case 0x01: snprintf(op, 32, "RLC    C"); break;
        case 0x02: snprintf(op, 32, "RLC    D"); break;
        case 0x03: snprintf(op, 32, "RLC    E"); break;
        case 0x04: snprintf(op, 32, "RLC    H"); break;
        case 0x05: snprintf(op, 32, "RLC    L"); break;
        case 0x06: snprintf(op, 32, "RLC    (HL)"); break;
        case 0x07: snprintf(op, 32, "RLC    A"); break;
        case 0x08: snprintf(op, 32, "RRC    B"); break;
        case 0x09: snprintf(op, 32, "RRC    C"); break;
        case 0x0A: snprintf(op, 32, "RRC    D"); break;
        case 0x0B: snprintf(op, 32, "RRC    E"); break;
        case 0x0C: snprintf(op, 32, "RRC    H"); break;
        case 0x0D: snprintf(op, 32, "RRC    L"); break;
        case 0x0E: snprintf(op, 32, "RRC    (HL)"); break;
        case 0x0F: snprintf(op, 32, "RRC    A"); break;

        case 0x10: snprintf(op, 32, "RL     B"); break;
        case 0x11: snprintf(op, 32, "RL     C"); break;
        case 0x12: snprintf(op, 32, "RL     D"); break;
        case 0x13: snprintf(op, 32, "RL     E"); break;
        case 0x14: snprintf(op, 32, "RL     H"); break;
        case 0x15: snprintf(op, 32, "RL     L"); break;
        case 0x16: snprintf(op, 32, "RL     (HL)"); break;
        case 0x17: snprintf(op, 32, "RL     A"); break;
        case 0x18: snprintf(op, 32, "RR     B"); break;
        case 0x19: snprintf(op, 32, "RR     C"); break;
        case 0x1A: snprintf(op, 32, "RR     D"); break;
        case 0x1B: snprintf(op, 32, "RR     E"); break;
        case 0x1C: snprintf(op, 32, "RR     H"); break;
        case 0x1D: snprintf(op, 32, "RR     L"); break;
        case 0x1E: snprintf(op, 32, "RR     (HL)"); break;
        case 0x1F: snprintf(op, 32, "RR     A"); break;

        case 0x20: snprintf(op, 32, "SLA    B"); break;
        case 0x21: snprintf(op, 32, "SLA    C"); break;
        case 0x22: snprintf(op, 32, "SLA    D"); break;
        case 0x23: snprintf(op, 32, "SLA    E"); break;
        case 0x24: snprintf(op, 32, "SLA    H"); break;
        case 0x25: snprintf(op, 32, "SLA    L"); break;
        case 0x26: snprintf(op, 32, "SLA    (HL)"); break;
        case 0x27: snprintf(op, 32, "SLA    A"); break;
        case 0x28: snprintf(op, 32, "SRA    B"); break;
        case 0x29: snprintf(op, 32, "SRA    C"); break;
        case 0x2A: snprintf(op, 32, "SRA    D"); break;
        case 0x2B: snprintf(op, 32, "SRA    E"); break;
        case 0x2C: snprintf(op, 32, "SRA    H"); break;
        case 0x2D: snprintf(op, 32, "SRA    L"); break;
        case 0x2E: snprintf(op, 32, "SRA    (HL)"); break;
        case 0x2F: snprintf(op, 32, "SRA    A"); break;

        case 0x30: snprintf(op, 32, "SWAP   B"); break;
        case 0x31: snprintf(op, 32, "SWAP   C"); break;
        case 0x32: snprintf(op, 32, "SWAP   D"); break;
        case 0x33: snprintf(op, 32, "SWAP   E"); break;
        case 0x34: snprintf(op, 32, "SWAP   H"); break;
        case 0x35: snprintf(op, 32, "SWAP   L"); break;
        case 0x36: snprintf(op, 32, "SWAP   (HL)"); break;
        case 0x37: snprintf(op, 32, "SWAP   A"); break;
        case 0x38: snprintf(op, 32, "SRL    B"); break;
        case 0x39: snprintf(op, 32, "SRL    C"); break;
        case 0x3A: snprintf(op, 32, "SRL    D"); break;
        case 0x3B: snprintf(op, 32, "SRL    E"); break;
        case 0x3C: snprintf(op, 32, "SRL    H"); break;
        case 0x3D: snprintf(op, 32, "SRL    L"); break;
        case 0x3E: snprintf(op, 32, "SRL    (HL)"); break;
        case 0x3F: snprintf(op, 32, "SRL    A"); break;

        case 0x40: snprintf(op, 32, "BIT    0,B"); break;
        case 0x41: snprintf(op, 32, "BIT    0,C"); break;
        case 0x42: snprintf(op, 32, "BIT    0,D"); break;
        case 0x43: snprintf(op, 32, "BIT    0,E"); break;
        case 0x44: snprintf(op, 32, "BIT    0,H"); break;
        case 0x45: snprintf(op, 32, "BIT    0,L"); break;
        case 0x46: snprintf(op, 32, "BIT    0,(HL)"); break;
        case 0x47: snprintf(op, 32, "BIT    0,A"); break;
        case 0x48: snprintf(op, 32, "BIT    1,B"); break;
        case 0x49: snprintf(op, 32, "BIT    1,C"); break;
        case 0x4A: snprintf(op, 32, "BIT    1,D"); break;
        case 0x4B: snprintf(op, 32, "BIT    1,E"); break;
        case 0x4C: snprintf(op, 32, "BIT    1,H"); break;
        case 0x4D: snprintf(op, 32, "BIT    1,L"); break;
        case 0x4E: snprintf(op, 32, "BIT    1,(HL)"); break;
        case 0x4F: snprintf(op, 32, "BIT    1,A"); break;

        case 0x50: snprintf(op, 32, "BIT    2,B"); break;
        case 0x51: snprintf(op, 32, "BIT    2,C"); break;
        case 0x52: snprintf(op, 32, "BIT    2,D"); break;
        case 0x53: snprintf(op, 32, "BIT    2,E"); break;
        case 0x54: snprintf(op, 32, "BIT    2,H"); break;
        case 0x55: snprintf(op, 32, "BIT    2,L"); break;
        case 0x56: snprintf(op, 32, "BIT    2,(HL)"); break;
        case 0x57: snprintf(op, 32, "BIT    2,A"); break;
        case 0x58: snprintf(op, 32, "BIT    3,B"); break;
        case 0x59: snprintf(op, 32, "BIT    3,C"); break;
        case 0x5A: snprintf(op, 32, "BIT    3,D"); break;
        case 0x5B: snprintf(op, 32, "BIT    3,E"); break;
        case 0x5C: snprintf(op, 32, "BIT    3,H"); break;
        case 0x5D: snprintf(op, 32, "BIT    3,L"); break;
        case 0x5E: snprintf(op, 32, "BIT    3,(HL)"); break;
        case 0x5F: snprintf(op, 32, "BIT    3,A"); break;

        case 0x60: snprintf(op, 32, "BIT    4,B"); break;
        case 0x61: snprintf(op, 32, "BIT    4,C"); break;
        case 0x62: snprintf(op, 32, "BIT    4,D"); break;
        case 0x63: snprintf(op, 32, "BIT    4,E"); break;
        case 0x64: snprintf(op, 32, "BIT    4,H"); break;
        case 0x65: snprintf(op, 32, "BIT    4,L"); break;
        case 0x66: snprintf(op, 32, "BIT    4,(HL)"); break;
        case 0x67: snprintf(op, 32, "BIT    4,A"); break;
        case 0x68: snprintf(op, 32, "BIT    5,B"); break;
        case 0x69: snprintf(op, 32, "BIT    5,C"); break;
        case 0x6A: snprintf(op, 32, "BIT    5,D"); break;
        case 0x6B: snprintf(op, 32, "BIT    5,E"); break;
        case 0x6C: snprintf(op, 32, "BIT    5,H"); break;
        case 0x6D: snprintf(op, 32, "BIT    5,L"); break;
        case 0x6E: snprintf(op, 32, "BIT    5,(HL)"); break;
        case 0x6F: snprintf(op, 32, "BIT    5,A"); break;

        case 0x70: snprintf(op, 32, "BIT    6,B"); break;
        case 0x71: snprintf(op, 32, "BIT    6,C"); break;
        case 0x72: snprintf(op, 32, "BIT    6,D"); break;
        case 0x73: snprintf(op, 32, "BIT    6,E"); break;
        case 0x74: snprintf(op, 32, "BIT    6,H"); break;
        case 0x75: snprintf(op, 32, "BIT    6,L"); break;
        case 0x76: snprintf(op, 32, "BIT    6,(HL)"); break;
        case 0x77: snprintf(op, 32, "BIT    6,A"); break;
        case 0x78: snprintf(op, 32, "BIT    7,B"); break;
        case 0x79: snprintf(op, 32, "BIT    7,C"); break;
        case 0x7A: snprintf(op, 32, "BIT    7,D"); break;
        case 0x7B: snprintf(op, 32, "BIT    7,E"); break;
        case 0x7C: snprintf(op, 32, "BIT    7,H"); break;
        case 0x7D: snprintf(op, 32, "BIT    7,L"); break;
        case 0x7E: snprintf(op, 32, "BIT    7,(HL)"); break;
        case 0x7F: snprintf(op, 32, "BIT    7,A"); break;

        case 0x80: snprintf(op, 32, "RES    0,B"); break;
        case 0x81: snprintf(op, 32, "RES    0,C"); break;
        case 0x82: snprintf(op, 32, "RES    0,D"); break;
        case 0x83: snprintf(op, 32, "RES    0,E"); break;
        case 0x84: snprintf(op, 32, "RES    0,H"); break;
        case 0x85: snprintf(op, 32, "RES    0,L"); break;
        case 0x86: snprintf(op, 32, "RES    0,(HL)"); break;
        case 0x87: snprintf(op, 32, "RES    0,A"); break;
        case 0x88: snprintf(op, 32, "RES    1,B"); break;
        case 0x89: snprintf(op, 32, "RES    1,C"); break;
        case 0x8A: snprintf(op, 32, "RES    1,D"); break;
        case 0x8B: snprintf(op, 32, "RES    1,E"); break;
        case 0x8C: snprintf(op, 32, "RES    1,H"); break;
        case 0x8D: snprintf(op, 32, "RES    1,L"); break;
        case 0x8E: snprintf(op, 32, "RES    1,(HL)"); break;
        case 0x8F: snprintf(op, 32, "RES    1,A"); break;

        case 0x90: snprintf(op, 32, "RES    2,B"); break;
        case 0x91: snprintf(op, 32, "RES    2,C"); break;
        case 0x92: snprintf(op, 32, "RES    2,D"); break;
        case 0x93: snprintf(op, 32, "RES    2,E"); break;
        case 0x94: snprintf(op, 32, "RES    2,H"); break;
        case 0x95: snprintf(op, 32, "RES    2,L"); break;
        case 0x96: snprintf(op, 32, "RES    2,(HL)"); break;
        case 0x97: snprintf(op, 32, "RES    2,A"); break;
        case 0x98: snprintf(op, 32, "RES    3,B"); break;
        case 0x99: snprintf(op, 32, "RES    3,C"); break;
        case 0x9A: snprintf(op, 32, "RES    3,D"); break;
        case 0x9B: snprintf(op, 32, "RES    3,E"); break;
        case 0x9C: snprintf(op, 32, "RES    3,H"); break;
        case 0x9D: snprintf(op, 32, "RES    3,L"); break;
        case 0x9E: snprintf(op, 32, "RES    3,(HL)"); break;
        case 0x9F: snprintf(op, 32, "RES    3,A"); break;

        case 0xA0: snprintf(op, 32, "RES    4,B"); break;
        case 0xA1: snprintf(op, 32, "RES    4,C"); break;
        case 0xA2: snprintf(op, 32, "RES    4,D"); break;
        case 0xA3: snprintf(op, 32, "RES    4,E"); break;
        case 0xA4: snprintf(op, 32, "RES    4,H"); break;
        case 0xA5: snprintf(op, 32, "RES    4,L"); break;
        case 0xA6: snprintf(op, 32, "RES    4,(HL)"); break;
        case 0xA7: snprintf(op, 32, "RES    4,A"); break;
        case 0xA8: snprintf(op, 32, "RES    5,B"); break;
        case 0xA9: snprintf(op, 32, "RES    5,C"); break;
        case 0xAA: snprintf(op, 32, "RES    5,D"); break;
        case 0xAB: snprintf(op, 32, "RES    5,E"); break;
        case 0xAC: snprintf(op, 32, "RES    5,H"); break;
        case 0xAD: snprintf(op, 32, "RES    5,L"); break;
        case 0xAE: snprintf(op, 32, "RES    5,(HL)"); break;
        case 0xAF: snprintf(op, 32, "RES    5,A"); break;

        case 0xB0: snprintf(op, 32, "RES    6,B"); break;
        case 0xB1: snprintf(op, 32, "RES    6,C"); break;
        case 0xB2: snprintf(op, 32, "RES    6,D"); break;
        case 0xB3: snprintf(op, 32, "RES    6,E"); break;
        case 0xB4: snprintf(op, 32, "RES    6,H"); break;
        case 0xB5: snprintf(op, 32, "RES    6,L"); break;
        case 0xB6: snprintf(op, 32, "RES    6,(HL)"); break;
        case 0xB7: snprintf(op, 32, "RES    6,A"); break;
        case 0xB8: snprintf(op, 32, "RES    7,B"); break;
        case 0xB9: snprintf(op, 32, "RES    7,C"); break;
        case 0xBA: snprintf(op, 32, "RES    7,D"); break;
        case 0xBB: snprintf(op, 32, "RES    7,E"); break;
        case 0xBC: snprintf(op, 32, "RES    7,H"); break;
        case 0xBD: snprintf(op, 32, "RES    7,L"); break;
        case 0xBE: snprintf(op, 32, "RES    7,(HL)"); break;
        case 0xBF: snprintf(op, 32, "RES    7,A"); break;

        case 0xC0: snprintf(op, 32, "SET    0,B"); break;
        case 0xC1: snprintf(op, 32, "SET    0,C"); break;
        case 0xC2: snprintf(op, 32, "SET    0,D"); break;
        case 0xC3: snprintf(op, 32, "SET    0,E"); break;
        case 0xC4: snprintf(op, 32, "SET    0,H"); break;
        case 0xC5: snprintf(op, 32, "SET    0,L"); break;
        case 0xC6: snprintf(op, 32, "SET    0,(HL)"); break;
        case 0xC7: snprintf(op, 32, "SET    0,A"); break;
        case 0xC8: snprintf(op, 32, "SET    1,B"); break;
        case 0xC9: snprintf(op, 32, "SET    1,C"); break;
        case 0xCA: snprintf(op, 32, "SET    1,D"); break;
        case 0xCB: snprintf(op, 32, "SET    1,E"); break;
        case 0xCC: snprintf(op, 32, "SET    1,H"); break;
        case 0xCD: snprintf(op, 32, "SET    1,L"); break;
        case 0xCE: snprintf(op, 32, "SET    1,(HL)"); break;
        case 0xCF: snprintf(op, 32, "SET    1,A"); break;

        case 0xD0: snprintf(op, 32, "SET    2,B"); break;
        case 0xD1: snprintf(op, 32, "SET    2,C"); break;
        case 0xD2: snprintf(op, 32, "SET    2,D"); break;
        case 0xD3: snprintf(op, 32, "SET    2,E"); break;
        case 0xD4: snprintf(op, 32, "SET    2,H"); break;
        case 0xD5: snprintf(op, 32, "SET    2,L"); break;
        case 0xD6: snprintf(op, 32, "SET    2,(HL)"); break;
        case 0xD7: snprintf(op, 32, "SET    2,A"); break;
        case 0xD8: snprintf(op, 32, "SET    3,B"); break;
        case 0xD9: snprintf(op, 32, "SET    3,C"); break;
        case 0xDA: snprintf(op, 32, "SET    3,D"); break;
        case 0xDB: snprintf(op, 32, "SET    3,E"); break;
        case 0xDC: snprintf(op, 32, "SET    3,H"); break;
        case 0xDD: snprintf(op, 32, "SET    3,L"); break;
        case 0xDE: snprintf(op, 32, "SET    3,(HL)"); break;
        case 0xDF: snprintf(op, 32, "SET    3,A"); break;

        case 0xE0: snprintf(op, 32, "SET    4,B"); break;
        case 0xE1: snprintf(op, 32, "SET    4,C"); break;
        case 0xE2: snprintf(op, 32, "SET    4,D"); break;
        case 0xE3: snprintf(op, 32, "SET    4,E"); break;
        case 0xE4: snprintf(op, 32, "SET    4,H"); break;
        case 0xE5: snprintf(op, 32, "SET    4,L"); break;
        case 0xE6: snprintf(op, 32, "SET    4,(HL)"); break;
        case 0xE7: snprintf(op, 32, "SET    4,A"); break;
        case 0xE8: snprintf(op, 32, "SET    5,B"); break;
        case 0xE9: snprintf(op, 32, "SET    5,C"); break;
        case 0xEA: snprintf(op, 32, "SET    5,D"); break;
        case 0xEB: snprintf(op, 32, "SET    5,E"); break;
        case 0xEC: snprintf(op, 32, "SET    5,H"); break;
        case 0xED: snprintf(op, 32, "SET    5,L"); break;
        case 0xEE: snprintf(op, 32, "SET    5,(HL)"); break;
        case 0xEF: snprintf(op, 32, "SET    5,A"); break;

        case 0xF0: snprintf(op, 32, "SET    6,B"); break;
        case 0xF1: snprintf(op, 32, "SET    6,C"); break;
        case 0xF2: snprintf(op, 32, "SET    6,D"); break;
        case 0xF3: snprintf(op, 32, "SET    6,E"); break;
        case 0xF4: snprintf(op, 32, "SET    6,H"); break;
        case 0xF5: snprintf(op, 32, "SET    6,L"); break;
        case 0xF6: snprintf(op, 32, "SET    6,(HL)"); break;
        case 0xF7: snprintf(op, 32, "SET    6,A"); break;
        case 0xF8: snprintf(op, 32, "SET    7,B"); break;
        case 0xF9: snprintf(op, 32, "SET    7,C"); break;
        case 0xFA: snprintf(op, 32, "SET    7,D"); break;
        case 0xFB: snprintf(op, 32, "SET    7,E"); break;
        case 0xFC: snprintf(op, 32, "SET    7,H"); break;
        case 0xFD: snprintf(op, 32, "SET    7,L"); break;
        case 0xFE: snprintf(op, 32, "SET    7,(HL)"); break;
        case 0xFF: snprintf(op, 32, "SET    7,A"); break;
    }
}
