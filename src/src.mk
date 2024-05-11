aka_src_dir = $(shell pwd)
CFLAGS += -I$(aka_src_dir)

SRC_CFILES = akarser.c dloader.c main.c
SRC_CFILES += $(aka_src_dir)/mac/aka-mac.c # add mac/*.c
SRC_CFILES += $(aka_src_dir)/ip/aka-ip.c # add ip/*.c
SRC_OFILES = $(SRC_CFILES:.c=.o)
