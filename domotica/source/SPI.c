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
#include <string.h>

char sendMode = 0; // 0 = Receiving || 1 = Sending
uint8_t spiDATA = 0; //Global variable for the data recieved or to transmit

//DataBuffer variables
char dataBuffer[BUFFER_SIZE];
char dataBufferCounter = 0;
char dataBufferLimit = 0;

/* @function : initSPI()
 *		To Initialize SPI Communication following the parameters.
 * @param - Refer to Explanation of SPCR in SPI.h
 * @return - Returns the contents of the SPCR register
 */	
char initSPI(char clockRate, EDGE_REACT, SPI_MODE, SIGNIFICANT_BIT_FIRST, SPI_ENABLE, SPI_INTERRUPT_ENABLE) {
	//Set Interrupt for SPI STC(Serial Transfer Complete) if SPI is to be enabled
	if ((spiEnable & 0x1) == 1) {
		SPSR = (1 << SPIF); //SPI Status Register : See SPI.h for Explanation
		sei(); //Set Global Interrupt Enable : Or use SREG = (1<<7); 
	}
	//Set Direction registers for I/O pins
	if ((spiMasterEnable & 0x1) == 0) {			//SPI is Slave
		DDRB &= ~(1<<SPI_MOSI_PIN); // input
		DDRB |= (1<<SPI_MISO_PIN); // output
		DDRB &= ~(1<<SPI_SS_PIN); // input
		DDRB &= ~(1<<SPI_SCK_PIN);// input
	}
	else if ((spiMasterEnable & 0x1) == 1) {	//SPI is Master
		DDRB |= (1<<SPI_MOSI_PIN); // output
		DDRB &= ~(1<<SPI_MISO_PIN); // input
		DDRB |= (1<<SPI_SCK_PIN);// output
		DDRB |= (1<<SPI_SS_PIN);// output
	}
	//Set SPI Settings : See SPI.h for explanation
	char spiSettings = 0; //For the SPIsettings
	spiSettings += (clockRate & 0x3);
	spiSettings += ((edgeReact & 0x1) << SPH);
	spiSettings += (1 << CPOL);
	spiSettings += ((spiMasterEnable & 0x1) << MSTR);
	spiSettings += ((dataOrder & 0x1) << DORD);
	spiSettings += ((spiEnable & 0x1) << SPE);
	spiSettings += ((spiInterruptEnable & 0x1) << SPIE);
	SPCR = spiSettings; //Set SPI settings
	return spiSettings;
}

/* @function : disableSPI()
 *		To Disable SPI Communication
 */
void disableSPI(void) {
	SPCR = 0;
}

/* @function : recievedDataSPI()
 * @return - Returns Register SPDR's contents
*/ 
uint8_t getRecievedDataSPI(void) {
	return SPDR;
}

/* @function : setSendDataSPI()
 * @param - Data to transmit in next session
*/ 
void setDataToSendSPI(uint8_t dataOut) {
	if (dataOut == '\0') { //May or may not need to be changed
		sendMode = 0;
		strcpy(dataBuffer, "");
		dataBufferCounter = 0;
		dataBufferLimit = BUFFER_SIZE;
	}
	SPDR = dataOut;
}

/* @function : nextData()
 *		Set spiDATA with the next item from the buffer
*/
void nextData(void) {
	if (sendMode) {
		if (dataBufferCounter < dataBufferLimit) dataBufferCounter++;
		spiDATA = (uint8_t) dataBuffer[dataBufferCounter];
	}
	if (!sendMode) {
		if (dataBufferCounter < dataBufferLimit) dataBufferCounter++;
		dataBuffer[dataBufferCounter] = (char) spiDATA;
	}
}

/* @function : setDataBuffer()
 *		Set the databuffer
 * @param - A character array/string to fill the databuffer with
*/
void setDataBuffer(char data[]) {
	sendMode = 1;
	strcpy(dataBuffer, data);
	dataBufferLimit = strlen(data);
	dataBufferCounter = 0;
}

char getDataBufferAtIndex(char a) {
	if(a <= dataBufferLimit) return dataBuffer[a];
	else return -1;
}

/* @function : ISR()
 *		A Interrupt Service Routine; Interrupts when a successfull datatransfer occurred
 * @param - A "vector".
 *		SPI_STC_vect - Vector No.: 24 - Program Address: $0030
*/ 
ISR(SPI_STC_vect) {
	if (sendMode) { //Set the data Register and do not read
		setDataToSendSPI(spiDATA); 
		nextData(); //Next 8 bits to send set in spiDATA
	}
	if (!sendMode) { //Read the data register and do not set
		spiDATA = getRecievedDataSPI();
		nextData(); //Contents of SPDR set in spiDATA
	}
}