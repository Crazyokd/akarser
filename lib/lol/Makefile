LDFLAGS=-L$(shell pwd) -Wl,-R$(shell pwd) -llol $(shell pkg-config --cflags yaml-0.1) $(shell pkg-config --libs yaml-0.1)

all: liblol.so liblol.a

liblol.so: core.c parser.c log.c lol-strings.c
	$(CC) -fPIC -shared $^ -o $@ $(CFLAGS) $(shell pkg-config --cflags yaml-0.1) $(shell pkg-config --libs yaml-0.1)

liblol.a: core.o parser.o log.o lol-strings.o
	$(AR) rcs $@ $^

clean:
	$(RM) -f *.o *.so *.a

# example: example.c
# 	$(CC) -DDEFAULT_CONFIG_FILENAME=\"lol.yaml\" $^ -o $@ $(CFLAGS) $(LDFLAGS)
