#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <getopt.h>
#include "disassembler.h"

void print_help(char *cmd_name)
{
    printf("Usage: %s [option(s)] <rom_file>\n", cmd_name);
    printf("\nOptions (all values are given in hexadecimal):\n");
    printf("  -s  <start_address>   (default = 0)\n");
    printf("  -e  <end_address>     (default = end of file)\n");
    printf("  -r                    stop on first RET or RETI reached\n");
    printf("  -l  <load_addr>       FOR .GBS-FILES. subtract 0x70 (header size)\n");
    printf("                        and offset addresses with load_addr\n");
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        print_help(argv[0]);
        return 1;
    }

    int32_t addr_start = 0;
    int32_t addr_end = -1;
    bool stop_on_ret = false;
    int32_t addr_offset = 0;

    // Read options
    char *endptr;
    int c;
    while ((c = getopt(argc, argv, "rs:e:l:")) != -1) {
        switch (c) {
            case 'r':
                stop_on_ret = true;
                break;
            case 's':
                addr_start = strtol(optarg, &endptr, 16);
                if (*endptr != '\0') {
                    printf("ERROR: Unable to read addr_start\n");
                    return 1;
                }
                break;
            case 'e':
                addr_end = strtol(optarg, &endptr, 16);
                if (*endptr != '\0') {
                    printf("ERROR: Unable to read addr_end\n");
                    return 1;
                }
                break;
            case 'l':
                addr_offset = strtol(optarg, &endptr, 16);
                if (*endptr != '\0') {
                    printf("ERROR: Unable to read load_addr\n");
                    return 1;
                }
                addr_offset -= 0x70;
                break;
            default:
                print_help(argv[0]);
                return 1;
        }
    }

    if (optind == argc) {
        printf("ERROR: No ROM file specified\n");
        return 1;
    }

    FILE *f;
    for (int index = optind; index < argc; ++index) {
        // Read ROM file
        f = fopen(argv[index], "rb");
        if (f == NULL) {
            printf("ERROR: Unable to open file %s\n", argv[index]);
            return 1;
        }
    }

    // Get file size
    fseek(f, 0, SEEK_END);
    int32_t fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    uint8_t *buf = malloc(fsize);

    // Load the file content into buf
    fread(buf, fsize, 1, f);
    fclose(f);

    if (addr_offset != 0 && fsize < 0x70) {
        printf("ERROR: File is smaller than .gbs-header...\n");
    }
    if (addr_end == -1) {
        addr_end = addr_offset + fsize - 1;
    }

    addr_start -= addr_offset;
    addr_end -= addr_offset;

    if (addr_start < 0 || addr_start >= fsize) {
        printf("ERROR: addr_start is out of range\n");
        return 1;
    }
    if (addr_end < 0 || addr_end >= fsize) {
        printf("ERROR: addr_end is out of range\n");
        return 1;
    }
    if (addr_start > addr_end) {
        printf("ERROR: addr_start > addr_end\n");
        return 1;
    }

    // Disassemble in address interval
    uint32_t pc = addr_start;
    bool ret_reached;
    while (pc <= (uint32_t)addr_end) {
        pc += disassemble(buf, pc, &ret_reached, addr_offset);
        if (ret_reached && stop_on_ret)
            break;
    }
    free(buf);
}
