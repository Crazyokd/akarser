all: akarser

akarser: main.o akarser.o dloader.o
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)

clean:
	rm -f *.o akarser
