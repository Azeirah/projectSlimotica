/*
 * I2C.c
 *
 * Created: 26-Jan-15 13:29:37
 *  Author: Dodedodo33
 */ 

#include <avr/interrupt.h>
#include <util/twi.h>

char dataBufferIn[200];
char dataBufferOut[200];
char bufferOverflow;
char transferComplete;
int bufferCounter;
int dataSize;

void initTWI(){
	sei();
	PRR0 &= ~(1 << PRTWI);
	TWAR = 0xAA; //Set slave address;
	TWAMR = 0; //Disable slave address masking
	TWCR |= (1<<TWIE)|(1<<TWEN)|(1<<TWEA); //Enable TWI and TWI interrupt and last but not least the slave address recognition
	dataBufferOut[0] = -1;
	bufferOverflow = 0;
	transferComplete = 0;
	bufferCounter = 0;
	dataSize = -1;
}

ISR(TWI_vect) {
	switch(TW_STATUS) {
		case TW_ST_SLA_ACK:
			transferComplete = 0;
			TWDR = dataBufferOut[bufferCounter];
			TWCR = (1<<TWINT)|(1<<TWEA);
			break;
		case TW_ST_ARB_LOST_SLA_ACK:
			transferComplete = 0;
			TWDR = dataBufferOut[bufferCounter];
			TWCR = (1<<TWINT)|(1<<TWEA);
			break;
		case TW_ST_DATA_ACK:
			if (dataSize < 0) {
				TWDR = -1;
				TWCR = (1<<TWINT)|(1<<TWEA);
			} else if (dataSize > 0) {
				TWDR = dataBufferOut[bufferCounter];
				dataSize--;
				TWCR = (1<<TWINT)|(1<<TWEA);
			} else {
				TWDR = dataBufferOut[bufferCounter];
				TWCR = (1<<TWINT)&~(0<<TWEA);
			}
			break;
		case TW_ST_DATA_NACK:
			TWCR = (1<<TWINT)|(1<<TWEA);
			transferComplete = 1;
			break;
		case TW_ST_LAST_DATA:
			TWCR = (1<<TWINT)|(1<<TWEA);
			transferComplete = 1;
			break;
		case TW_SR_SLA_ACK:
			transferComplete = 0;
			TWCR = (1<<TWINT)|(1<<TWEA);
			break;
		case TW_SR_ARB_LOST_SLA_ACK:
			transferComplete = 0;
			TWCR = (1<<TWINT)|(1<<TWEA);
			break;
		case TW_SR_DATA_ACK:
			if (bufferCounter < 199) {
				dataBufferIn[bufferCounter] = TWDR;
				bufferCounter++;
				TWCR = (1<<TWINT)|(1<<TWEA);
			} else {
				TWCR = (1<<TWINT)&~(0<<TWEA);
			}
			break;
		case TW_SR_DATA_NACK:
			dataBufferIn[bufferCounter] = TWDR;
			transferComplete = 1;
			TWCR = (1<<TWINT)|(1<<TWEA);
			break;
		case TW_SR_STOP:
			transferComplete = 1;
			TWCR = (1<<TWINT)|(1<<TWEA);
			break;
		case TW_NO_INFO:
			break;
		case TW_BUS_ERROR:
			TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEA);
			break;
	}
}