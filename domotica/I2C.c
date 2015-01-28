/*
 * I2C.c
 *
 * Created: 26-Jan-15 13:29:37
 *  Author: Dodedodo33
 */ 

#include <avr/interrupt.h>
#include <util/twi.h>
#include <dev/watchdog.h>
#include <stdio.h>

char dataBufferIn[200];
char dataBufferOut[200];
char bufferOverflow;
char transferComplete;
char receive;
int bufferCounter;
int dataSize;
int dataLength;

void initTWI(void){
	sei();
	PRR0 &= ~(1 << PRTWI);
	TWCR |= (1<<TWIE)|(1<<TWEN)|(1<<TWEA); //Enable TWI and TWI interrupt and last but not least the slave address recognition
	TWAR = 0xAA; //Set slave address;
	TWAMR = 0; //Disable slave address masking
	dataBufferOut[0] = -1;
	receive = 1;
	bufferOverflow = 0;
	transferComplete = 0;
	bufferCounter = 0;
	dataSize = -1;
	dataLength = 0;
}

char checkTransferComplete(void) {
	if (transferComplete) {
		transferComplete = 0;
		return 1;
	} else {
		return 0;
	}
}

char checkMode(void) {
	return receive;
}

void printData(void) {
	int i;
	for (i = 0; i < dataLength; i++) {
		printf("Data received: %x \n", dataBufferIn[i]);
		watchdog_periodic();
	}
}

void fillBuffer(void) {
	int i = 0;
	dataSize = 5;
	for (i = 0 ; i <30 ; i++) {
		dataBufferOut[i] = 0x80 - i;
	}
}

ISR(TWI_vect) {
	switch(TW_STATUS) {
		case TW_ST_SLA_ACK:
			//transferComplete = 0;
			receive = 0;
			bufferCounter = 0;
			TWDR = dataBufferOut[bufferCounter];
			bufferCounter++;
			TWCR |= (1<<TWINT)|(1<<TWEA);
			break;
		case TW_ST_ARB_LOST_SLA_ACK:
			receive = 0;
			//transferComplete = 0;
			bufferCounter = 0;
			TWDR = dataBufferOut[bufferCounter];
			bufferCounter++;
			TWCR |= (1<<TWINT)|(1<<TWEA);
			break;
		case TW_ST_DATA_ACK:
			if (dataSize < 0) {
				TWDR = -1;
				TWCR |= (1<<TWINT)|(1<<TWEA);
			} else if (dataSize > 0) {
				TWDR = dataBufferOut[bufferCounter];
				dataSize--;
				TWCR |= (1<<TWINT)|(1<<TWEA);
			} else {
				TWDR = dataBufferOut[bufferCounter];
				TWCR |= (1<<TWINT);
				TWCR &= ~(1<<TWEA);
			}
			break;
		case TW_ST_DATA_NACK:
			TWCR |= (1<<TWINT)|(1<<TWEA);
			//transferComplete = 1;
			dataSize = 5;
			bufferCounter = 0;
			break;
		case TW_ST_LAST_DATA:
			TWCR |= (1<<TWINT)|(1<<TWEA);
			//transferComplete = 1;
			bufferCounter = 0;
			break;
		case TW_SR_SLA_ACK:
			receive = 1;
			bufferCounter = 0;
			transferComplete = 0;
			TWCR |= (1<<TWINT)|(1<<TWEA);
			break;
		case TW_SR_ARB_LOST_SLA_ACK:
			bufferCounter = 0;
			receive = 1;
			transferComplete = 0;
			TWCR |= (1<<TWINT)|(1<<TWEA);
			break;
		case TW_SR_DATA_ACK:
			if (bufferCounter < 198) {
				dataBufferIn[bufferCounter] = TWDR;
				bufferCounter++;
				TWCR |= (1<<TWINT)|(1<<TWEA);
			} else {
				dataBufferIn[bufferCounter] = TWDR;
				bufferCounter++;
				TWCR |= (1<<TWINT);
				TWCR &= ~(0<<TWEA);
			}
			break;
		case TW_SR_DATA_NACK:
			dataBufferIn[bufferCounter] = TWDR;
			transferComplete = 1;
			dataLength = bufferCounter;
			bufferCounter = 0;
			TWCR |= (1<<TWINT)|(1<<TWEA);
			break;
		case TW_SR_STOP:
			transferComplete = 1;
			dataLength = bufferCounter;
			bufferCounter = 0;
			TWCR |= (1<<TWINT)|(1<<TWEA);
			break;
		case TW_NO_INFO:
			break;
		case TW_BUS_ERROR:
			TWCR |= (1<<TWINT)|(1<<TWSTO)|(1<<TWEA);
			printf("BUSERROR");
			break;
	}
}