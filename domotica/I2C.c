/*
 * I2C.c
 * About:
 * 
 * Created: 3-12-2014 14:30:15
 *  Author: Jasper van Meer
 */ 

#include <avr/interrupt.h> //For all interrupt vectors

/* I2C_Slave_Init is the function to
 * initialize I2C / TWI in Slave mode.
*/
void I2C_Slave_Init(void) {
	
}


/* ISR = Interrupt Service Routine;  
 * @param; Vectorname / Hashmap Key / 1 Dimensional Associative Array
 * TWI_vect = Two Wire Interface Vector; Vect no = 39; Programm Address = $004E
 * Bron: Atmel-8266-MCU-_Wireless-ATmega128RFA1_Datasheet
 */
ISR(TWI_vect) {
	
}