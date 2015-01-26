CONTIKI_PROJECT = domotica
PROJECTDIRS = source
PROJECTSOURCEFILES = SPI.c spi-udp-linker.c tcp_ip.c device-registration.c jsmn.c json-rpc.c server_calls.c util.c
all: $(CONTIKI_PROJECT)

# Use C99 instead of default C.
CFLAGS=-std=c99
CFLAGS+= -DUIP_CONF_IPV6_RPL

UIP_CONF_IPV6 = 1
UIP_CONF_RPL = 1
CONTIKI_WITH_IPV6 = 1

LDFLAGS += -L/source
TARGET_LIBFILES += "source/SPI.c"

CONTIKI = contiki
include $(CONTIKI)/Makefile.include