CFLAGS=-g -Wall -Werror -Wextra -Wshadow -Wpedantic -std=c99

.DEFAULT_GOAL := default # Set default target

default:
	bear -- make akarser
	doxygen Doxyfile

akarser: main.o akarser.o config.o
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)

run: akarser
	./akarser
debug: akarser
	gdb akarser

.PHONY: clean

clean:
	rm -f *.o akarser compile_commands.json
	rm -rf html latex