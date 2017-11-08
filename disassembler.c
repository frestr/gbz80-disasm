#include <stdlib.h>
#include <stdio.h>
#include "disassembler.h"
#include "opcodes.h"

static void print_line(char *op, uint8_t *code, uint8_t op_bytes, uint32_t pc)
{
    printf("0x%06x:    ", pc);
    printf("%02x ", code[0]);
    op_bytes > 1 ? printf("%02x ", code[1]) : printf("   ");
    op_bytes > 2 ? printf("%02x ", code[2]) : printf("   ");
    printf("    %s\n", op);
}

uint8_t disassemble(uint8_t *buf, uint32_t pc, bool *ret_reached)
{
    uint8_t *code = &buf[pc];
    char op[32];

    const char *op_string = (code[0] == 0xCB) ? ext_ops[code[1]] : ops[code[0]];
    uint8_t op_len = op_lengths[code[0]];

    if (op_len < 3)
        snprintf(op, 32, op_string, code[1]);
    else
        snprintf(op, 32, op_string, code[2], code[1]);

    print_line(op, code, op_len, pc);
    *ret_reached = (code[0] == 0xC9 || code[0] == 0xD9);
    return op_len;
}
