aka_core_dir = $(ROOT_DIR)/lib/core
CFLAGS += -I$(aka_core_dir)
LDFLAGS += -L$(aka_core_dir) -Wl,-R$(aka_core_dir) -laka-core
