/*
 * SPI.h
 *
 * Created: 15-12-2014 14:09:01
 *  Author: Jasper van Meer
 */ 

/* Explanation:
 * SPCR = SPI Control Register
 * 8765 4321 bit
 * 7654 3210 bit nr.
 * 0000 00XX = SPR  : SPI Clock Rate Select.	2 bits, 4 possibilities
 * 0000 0x00 = CPHA : SPI Clock Phase.			1 = React on rising edge if CPOL = 1	||	0 = React on falling edge if CPOL = 1
 * 0000 x000 = CPOL : SPI Clock Polarity.		1 = SPI Clock high is idle				||	0 = SPI Clock low is idle
 * 000x 0000 = MSTR : SPI Master/Slave Mode.	1 = SPI Master Mode						||	0 = SPI Slave Mode
 * 00x0 0000 = DORD : SPI Data Order.			1 = LSB of data word Transmitted first	||	0 = MSB of data word transmitted first
 * 0x00 0000 = SPE  : SPI Enable.				1 = SPI Enabled							||	0 = SPI Disabled
 * x000 0000 = SPIE : SPI interrupt Enable		Refer to Datasheet : 
 */

/* Explanation:
 * SPSR = SPI Status register
 * 8765 4321 bit
 * 7654 3210 bit nr.
 * 0000 000x = SPI2X: SPI double speed bit		1 = SPI Clock now runs on double speed	||	0 = SPI Clock runs on "normal" speed
 * 00xx xxx0 = RES  : Reserved
 * 0x00 0000 = WCOL : Write Colision Flag		1 = If SPDR is written while transfer	||	0 = If SPDR is not written while transfer
 * x000 0000 = SPIF : SPI Interrupt Flag		Refer to Datasheet
 */

/* Explanation:
 * SPDR = SPI Data register
 */

#define BUFFER_SIZE 200 //DataBuffer of the SPI bridge is limited to 200 bytes

//SPI Pin configurations: Conform deRFmega128 Pin Configuration
#define SPI_MOSI_PIN	PORTB2
#define SPI_MISO_PIN	PORTB3
#define SPI_SS_PIN		PORTD0
#define SPI_SCK_PIN		PORTB1

//Defines for ease of use parameters of Function: initSPI();
#define MASTER_MODE		1
#define SLAVE_MODE		0
#define ENABLE			1
#define DISABLE			0
#define RISING_EDGE		1
#define FALLING_EDGE	0
#define LSB_FIRST		1
#define MSB_FIRST		0

//Functions
char initSPI(void);
//char receiveSPI(void);

void disableSPI(void); //Disable SPI

void nextData(void); //Set next data databuffer > spiDATA || databuffer < spiDATA

char bufferRQReady(void);

void setDataBuffer(char data[]); //Set the databuffer to send via SPI

