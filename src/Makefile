include src.mk

all: akarser

akarser: $(SRC_OFILES)
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)

clean:
	rm -f *.o mac/*.o akarser
