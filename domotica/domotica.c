/*
 * My_Contiki.c
 *
 * Created: 27-11-2014 13:45:01
 *  Author: Jasper van Meer
 */ 

//#define F_CPU 1000000UL	//For delay Function
#include "contiki.h"	//Contiki
//#include "SPI.h"

#include <avr/io.h>	//For pin defines
#include <util/delay.h>	//For delay functions
#include <stdio.h>

extern char dataBuffer[BUFFER_SIZE];
extern char dataBufferCounter;
extern char dataBufferLimit;

//Making a Contiki Process
PROCESS(hello_world_process, "Hello world process");
PROCESS(print_info_spi_process, "Print SPI info prosess");
//Select a Process to start automatically after powering on
AUTOSTART_PROCESSES(&hello_world_process);



PROCESS_THREAD(hello_world_process, ev, data) {
	//char dataSPI;
	DDRE |= (1 << DDE6);
	PROCESS_BEGIN();
	clock_init();
	//printf("SPCR: %x \n", SPCR);
	//printf("SPSR: %x \n", SPSR);
	//disableSPI();
	while (1) {
		PROCESS_PAUSE();
	}
	PROCESS_END();
}
