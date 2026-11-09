CC ?= cc
CFLAGS ?= -std=c11 -O2 -Wall -Wextra -Wpedantic

.PHONY: all clean test
all: usagic

usagic: src/main.c src/transpiler.c include/usagic.h
	$(CC) $(CFLAGS) -Iinclude src/main.c src/transpiler.c -o usagic

test: usagic
	sh tests/run.sh

clean:
	rm -f usagic examples/*.usagic.c examples/hello examples/variables examples/loops examples/structs
