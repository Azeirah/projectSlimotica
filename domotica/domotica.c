/*
 * My_Contiki.c
 *
 * Created: 27-11-2014 13:45:01
 *  Author: Jasper van Meer
 */ 

#define F_CPU 1000000UL	//For delay Function
#include "contiki.h"	//Contiki
//#include "I2C.h"
#include "source/tcp_ip.h"

#include <avr/io.h>	//For pin defines
#include <util/delay.h>	//For delay functions
#include <stdio.h>
//void blinkingLeds(void);

//Making a Contiki Process
PROCESS(hello_world_process, "Hello world process");
//Select a Process to start automatically after powering on
AUTOSTART_PROCESSES(&hello_world_process);

PROCESS_THREAD(hello_world_process, ev, data) {
	DDRE |= (1 << DDE6);
	PROCESS_BEGIN();
	// int x;
	while (1) {
		PORTE |= (1 << DDE6);
		clock_delay_usec(500000);
		PORTE &= ~(1 << DDE6);
		clock_delay_usec(500000);
		printf("Got event number %d\n", ev);
		PROCESS_PAUSE();
	}
	PROCESS_END();
}

/*
void blinkingLeds(void) {
	
	int x;
	
	printf("Hello contiki world");
	for(x = 0; x < 10000 ; x++) {
		PROCESS_PAUSE();
		PORTE |= (1 << DDE6);
		clock_delay_usec(500000);
		PORTE &= ~(1 << DDE6);
		clock_delay_usec(500000);
	}
}
*/