# gbz80-disasm
Note: This disassembler is as simple as it gets. It pays no attention to things like cartridge headers, memory banks, data locations etc.

Should only be used if you want to see disassembled code from a location with a known good offset.

The `-r` flag makes the disassembler stop on the first RET or RETI found.

## Usage

```
$ ./disasm 
Usage: ./disasm <rom_file> [-r] [addr_start [[addr_end]]

$ ./disasm test.rom 
0x000000:    00           NOP
0x000001:    00           NOP
0x000002:    00           NOP
0x000003:    01 ab 12     LD     BC,$12AB
0x000006:    02           LD     (BC),A
0x000007:    c9           RET
0x000008:    00           NOP

$ ./disasm test.rom 2 3
0x000002:    00           NOP
0x000003:    01 ab 12     LD     BC,$12AB

$ ./disasm test.rom -r 3 
0x000003:    01 ab 12     LD     BC,$12AB          
0x000006:    02           LD     (BC),A            
0x000007:    c9           RET                      
```
