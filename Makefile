.PHONY: clean

CFLAGS=-g -Wall -Werror -Wextra -Wshadow -Wunused-function -std=gnu99
export CFLAGS
LDFLAGS=
export LDFLAGS

ROOT_DIR=$(shell pwd)

.DEFAULT_GOAL := default # Set default target

include lib/core/core.mk

all: default doxygen

default:
	bear -- $(MAKE) -C lib/core
	bear --append -- $(MAKE) -C src

doxygen:
	@doxygen Doxyfile

run: default
	./src/akarser
debug: default
	gdb ./src/akarser

clean:
	$(MAKE) -C lib/core clean
	$(MAKE) -C src clean
	$(RM) -f compile_commands.json
