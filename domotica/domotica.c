/*
 * My_Contiki.c
 *
 * Created: 27-11-2014 13:45:01
 *  Author: Jasper van Meer
 */ 

//#define F_CPU 1000000UL	//For delay Function
#include "contiki.h"	//Contiki
#include <avr/io.h>	//For pin defines
#include <util/delay.h>	//For delay functions
#include <stdio.h>

//Making a Contiki Process
PROCESS(hello_world_process, "Hello world process");
//Select a Process to start automatically after powering on
AUTOSTART_PROCESSES(&hello_world_process);


PROCESS_THREAD(hello_world_process, ev, data) {
	DDRE |= (1 << DDE6);
	PROCESS_BEGIN();
	clock_init();
	while (1) {
		PROCESS_PAUSE();
	}
	PROCESS_END();
}
