CONTIKI_PROJECT = domotica
PROJECTDIRS = source
PROJECT_SOURCEFILES = I2C.c
all: $(CONTIKI_PROJECT)

TARGET=avr-atmega128rfa1

# Use C99 instead of default C.
CFLAGS=-std=c99

LDFLAGS += -L/source
#TARGET_LIBFILES += "I2C.c"

CONTIKI = contiki
include $(CONTIKI)/Makefile.include
