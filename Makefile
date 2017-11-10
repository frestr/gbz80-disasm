CC=gcc
CFLAGS=-Wall -Wextra -g -c

SRCS=$(wildcard *.c)
OBJS=$(notdir $(SRCS:.c=.o))

all: disasm

disasm: $(OBJS)
	$(CC) -o disasm $(OBJS)

lib: $(OBJS)
	ar r libdisasm.a $(filter-out main.o, $(OBJS))
	ranlib libdisasm.a

%.o: %.c
	$(CC) $(CFLAGS) $<

clean:
	$(RM) $(OBJS)
