CFLAGS=-g -Wall -Werror -Wextra -Wshadow -std=gnu99
export CFLAGS

.DEFAULT_GOAL := default # Set default target

default:
	bear -- make akarser
	doxygen Doxyfile

lol:
	$(MAKE) -C lib/lol

akarser: main.o akarser.o config.o d-loader.o
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)

run: akarser
	./akarser
debug: akarser
	gdb akarser

.PHONY: clean

clean:
	$(MAKE) -C lib/lol clean
	rm -f *.o akarser compile_commands.json
	rm -rf html latex
