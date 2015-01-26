CONTIKI_PROJECT = domotica
PROJECTDIRS = source
PROJECTSOURCEFILES = SPI.c spi-udp-linker.c tcp_ip.c device-registration.c jsmn.c json-rpc.c server_calls.c util.c
all: $(CONTIKI_PROJECT)

# Use C99 instead of default C.
CFLAGS=-std=c99

LDFLAGS += -L/source
TARGET_LIBFILES += "source/SPI.c"

CONTIKI = contiki
include $(CONTIKI)/Makefile.include