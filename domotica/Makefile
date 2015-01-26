CONTIKI_PROJECT = domotica
all: $(CONTIKI_PROJECT)

# Use C99 instead of default C.
CFLAGS=-std=c99

LDFLAGS += -L/source
TARGET_LIBFILES += "I2C.c"

CONTIKI = contiki
include $(CONTIKI)/Makefile.include
