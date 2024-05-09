.PHONY: clean
CFLAGS=-g -Wall -Werror -Wextra -Wshadow -Wunused-function -std=gnu99
export CFLAGS
LDFLAGS=
ROOT_DIR=$(shell pwd)

.DEFAULT_GOAL := default # Set default target

include lib/core/core.mk

default:
	bear -- $(MAKE) -C lib/core
	bear --append -- make akarser
	doxygen Doxyfile

akarser: main.o akarser.o dloader.o
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)

run: akarser
	./akarser
debug: akarser
	gdb akarser

clean:
	$(MAKE) -C lib/core clean
	rm -f *.o akarser compile_commands.json
	rm -rf html latex
