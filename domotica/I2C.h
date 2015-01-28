/*
 * I2C.h
 *
 * Created: 27-Jan-15 10:34:49
 *  Author: Dodedodo33
 */ 


#ifndef I2C_H_
#define I2C_H_


void initTWI(void);
void printData(void);
char checkTransferComplete(void);
char checkMode(void);
void fillBuffer(void);


#endif /* I2C_H_ */