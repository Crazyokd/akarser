CORE_CFILES = aka-config.c aka-log.c aka-strings.c
CORE_OFILES = $(CORE_CFILES:.c=.o)

all: libaka-core.so libaka-core.a

libaka-core.so: $(CORE_CFILES)
	$(CC) -fPIC -shared $^ -o $@ $(CFLAGS)

libaka-core.a: $(CORE_OFILES)
	$(AR) rcs $@ $^

clean:
	$(RM) -f *.o *.so *.a
