/*
 *             ___      ___    ___             ___   
 *     o O O  / __|    | _ \  |_ _|           / __|  
 *    o       \__ \    |  _/   | |      _    | (__   
 *   TS__[O]  |___/   _|_|_   |___|   _(_)_   \___|  
 *  {======|_|"""""|_| """ |_|"""""|_|"""""|_|"""""| 
 * ./o--000'"`-0-0-'"`-0-0-'"`-0-0-'"`-0-0-'"`-0-0-' 
 *
 * Created: 15-12-2014 14:08:46
 * Author: Jasper van Meer
 * 
 * Module for communication via SPI
 * Data to transmit can be set by using setDatBuffer(DATA);
 * Data Recieved at a index of the buffer can be read using getDataBufferAtIndex(a_int); it returns -1 if number is out of bounds.
 */ 

#include "SPI.h"
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>

char sendMode = 0; // 0 = Receiving || 1 = Sending

//DataBuffer variables
char dataBuffer[BUFFER_SIZE];
char dataBufferCounter = 0;
char dataBufferLimit = 0;

/* @function : initSPI()
 *		To Initialize SPI Communication following the parameters.
 * @param - Refer to Explanation of SPCR in SPI.h
 * @return - Returns the contents of the SPCR register
 */	
char initSPI(void) {
	//enable SPI in power control register
	PRR0 = PRR0 & ~(1<PRSPI);
	//Set Interrupt for SPI STC(Serial Transfer Complete) if SPI is to be enabled
	//SPSR = (1 << SPIF); //SPI Status Register : See SPI.h for Explanation
	sei(); //Set Global Interrupt Enable : Or use SREG = (1<<7);
	
	//Set Direction registers for I/O pins
	DDRB &= ~(1<<SPI_MOSI_PIN); // input
	DDRB |= (1<<SPI_MISO_PIN); // output
	DDRB &= ~(1<<SPI_SS_PIN); // input
	DDRB &= ~(1<<SPI_SCK_PIN);// input
	
	//Set SPI Settings : See SPI.h for explanation
	char spiSettings = 0; //For the SPIsettings
	//spiSettings += (clockRate & 0x3);
	spiSettings = spiSettings & ~(1 << SPH);
	spiSettings = spiSettings & ~(1 << CPOL);
	spiSettings = spiSettings & ~(1 << MSTR);
	spiSettings = spiSettings & ~(1 << DORD);
	spiSettings = spiSettings | (1 << SPE);
	spiSettings = spiSettings | (1 << SPIE);
	//printf("Settings: %x \n", spiSettings);
	SPCR = spiSettings; //Set SPI settings
	return spiSettings;
}

/* @function : bufferRQReady()
 * 		If the databuffer has send or recieved all it's bytes the counter_t and
 *		limit are set to 0. This way with this function the user can check if the 
 *		dataBuffer is ready to be read.
*/
char bufferRQReady(void) {
	if (dataBufferCounter == 0 && dataBufferLimit == 0) return 1;
	else return 0;
}

/* @function : nextData()
 *		
*/
void nextData(void) {
	/*
	if (sendMode) {
		if (dataBufferCounter < dataBufferLimit) {
			SPDR = dataBuffer[dataBufferCounter];
			dataBufferCounter++;
		}
		else {
			dataBufferCounter = 0;
			dataBufferLimit = 0;
			sendMode = 0; //To recieve mode
		}
	}
	*/
	//if (!sendMode) {
		if (dataBufferCounter < BUFFER_SIZE) {
			dataBuffer[dataBufferCounter] = SPDR;
			//printf("Data received: %d", dataBuffer[dataBufferCounter]);
			dataBufferCounter++;
			if (dataBuffer[dataBufferCounter - 1] == '\0' || dataBufferCounter >= BUFFER_SIZE) {
				process_poll("print_info_spi_process");
				dataBufferCounter = 0;
				dataBufferLimit = 0;
				//sendMode = 1; //To send mode
			}
		}
	//}

}



/* @function : setDataBuffer()
 *		Set the databuffer
 * @param - A character array/string to fill the databuffer with
*/
void setDataBuffer(char data[]) {
	sendMode = 1;
	char datalen = strlen(data);
	if (datalen <= BUFFER_SIZE) {
		strcpy(dataBuffer, data);
		dataBufferLimit = datalen;
		dataBufferCounter = 0;
		nextData();
	}
}

/* @function : ISR()
 *		A Interrupt Service Routine; Interrupts when a successfull datatransfer occurred
 * @param - A "vector".
 *		SPI_STC_vect - Vector No.: 24 - Program Address: $0030
*/ 
ISR(SPI_STC_vect) {
	//printf("Data received: %x \n", SPDR);
	nextData();
}

/* @function : disableSPI()
 *		To Disable SPI Communication
 */
void disableSPI(void) {
	SPCR = 0;
}