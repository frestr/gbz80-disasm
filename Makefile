all: disasm

disasm: main.c disassembler.c
	gcc -Wall -Wextra -g -o disasm main.c disassembler.c

clean:
	rm disasm
