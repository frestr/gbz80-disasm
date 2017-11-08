all: disasm

disasm: main.c disassembler.c opcodes.c
	gcc -Wall -Wextra -g -o disasm main.c disassembler.c opcodes.c

clean:
	rm disasm
