# gbz80-disasm
Note: This disassembler is as simple as it gets. It pays no attention to things like cartridge headers, memory banks, data locations etc.

Should only be used if you want to see disassembled code from a location with a known good offset.

## Usage

```
$ ./disasm 
Usage: ./disasm [option(s)] <rom_file>

Options (all values are given in hexadecimal):
  -s  <start_address>   (default = 0)
  -e  <end_address>     (default = end of file)
  -r                    stop on first RET or RETI reached
  -l  <load_addr>       FOR .GBS-FILES. subtract 0x70 (header size)
                        and offset addresses with load_addr
$ ./disasm test.rom
0x000000:    00           NOP
0x000001:    00           NOP
0x000002:    00           NOP
0x000003:    01 ab 12     LD     BC,$12AB
0x000006:    02           LD     (BC),A
0x000007:    c9           RET
0x000008:    00           NOP
```
