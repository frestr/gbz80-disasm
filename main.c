#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "disassembler.h"

int main(int argc, char** argv)
{
    if (argc < 2) {
        printf("Usage: %s <rom_file> [addr_start] [addr_end]\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "rb");
    if (f == NULL) {
        printf("ERROR: Unable to open file %s\n", argv[1]);
        return 1;
    }

    // Get file size
    fseek(f, 0, SEEK_END);
    int32_t fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    uint8_t *buf = malloc(fsize);

    // Load the file content into buf
    fread(buf, fsize, 1, f);
    fclose(f);

    // Read addr_start
    char *endptr;
    int32_t addr_start = 0;
    if (argc >= 3) {
        addr_start = strtol(argv[2], &endptr, 16);
        if (*endptr != '\0') {
            printf("ERROR: Unable to read addr_start\n");
            return 1;
        }
        if (addr_start < 0 || addr_start >= fsize) {
            printf("ERROR: addr_start is out of range\n");
            return 1;
        }
    }

    // Read addr_end
    int32_t addr_end = fsize - 1;
    if (argc >= 4) {
        addr_end = strtol(argv[3], &endptr, 16);
        if (*endptr != '\0') {
            printf("ERROR: Unable to read addr_end\n");
            return 1;
        }
        if (addr_end < 0 || addr_end >= fsize) {
            printf("ERROR: addr_start is out of range\n");
            return 1;
        }
        if (addr_end < addr_start) {
            printf("ERROR: addr_start > addr_end\n");
            return 1;
        }
    }

    // Disassemble in address interval
    uint32_t pc = addr_start;
    while (pc <= (uint32_t)addr_end) {
        pc += disassemble(buf, pc);
    }
}
