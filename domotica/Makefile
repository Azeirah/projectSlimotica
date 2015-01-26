CONTIKI_PROJECT = domotica
all: $(CONTIKI_PROJECT)

# Use C99 instead of default C.
CFLAGS=-std=c99
CFLAGS += -ggdb3 -DDEBUG -DWITH_UIP=1
CFLAGS+= -DUIP_CONF_IPV6_RPL

LDFLAGS += -L/source
TARGET_LIBFILES += "source/SPI.c"

UIP_CONF_IPV6 = 1
UIP_CONF_RPL = 1

CONTIKI_PROJECT = contiki-iot
PROJECTDIRS = src 6lowpan
PROJECT_SOURCEFILES = device-registration.c util.c jsmn.c json-rpc.c tcp_ip.c

DEBUG = 1

$TARGET = Atmega128-RFA1
CONTIKI_WITH_IPV6 = 1
APPS=servreg-hack

CONTIKI = contiki
include $(CONTIKI)/Makefile.include