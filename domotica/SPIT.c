/*
 * SPIT.c
 *
 * Created: 20-Jan-15 16:14:25
 *  Author: Dodedodo33
 */ 


#include "spi.h"
//#include "../event-post.h"


#define SPI_CLK_PORT_BASE        GPIO_PORT_TO_BASE(SPI_CLK_PORT)
#define SPI_CLK_PIN_MASK         GPIO_PIN_MASK(SPI_CLK_PIN)
#define SPI_MOSI_PORT_BASE       GPIO_PORT_TO_BASE(SPI_MOSI_PORT)
#define SPI_MOSI_PIN_MASK        GPIO_PIN_MASK(SPI_MOSI_PIN)
#define SPI_MISO_PORT_BASE       GPIO_PORT_TO_BASE(SPI_MISO_PORT)
#define SPI_MISO_PIN_MASK        GPIO_PIN_MASK(SPI_MISO_PIN)

int noMessages = 0;

char receivebuffer[200];
char stringbuffer[25][200];
int recievecount = 0;

char fillingbuffer= 0;

extern process_event_t spi_event;

/*
 * Function:  spi_available
 * --------------------
 * Check if a string is available in the spi buffer. If there is a string, then return the length.
 *
 * Arguments:
 * string: the string to send over spi (terminated with \n)
 *
 * Return: 0 if there is no message available
 * 		   the length of the first string if there is a message.
 */

int spi_available() {
	if (noMessages != 0) {
		return strlen(stringbuffer[0]) + 1;
	}
	return 0;
}

/*
 * Function:  spi_write
 * --------------------
 * Write a string to the spi.
 *
 * Arguments:
 * string: the string to send over spi (terminated with \n)
 *
 * Return: void
 */
void spi_write(char * string) {
	printf("write over spi: \n %s", string);
	int i = 0;
	fillingbuffer = 1;
	while (string[i] != '\n') {
		if ((REG(SSI0_BASE + SSI_SR) & SSI_SR_TNF) == SSI_SR_TNF) { //If the transmit buffer is not full
			REG(SSI0_BASE + SSI_DR) = string[i];	//write char to spi buffer
			i++;
		}
		watchdog_periodic();
	}
	while((REG(SSI0_BASE + SSI_SR) & SSI_SR_TNF) != SSI_SR_TNF);//while transmit buffer is full
	REG(SSI0_BASE + SSI_DR) = '\n';
	REG(SSI0_BASE + SSI_DR) = 0;
	fillingbuffer = 0;
}

/*
 * Function:  spi_read
 * --------------------
 * Returns the first received spi string from the buffer.
 *
 * Arguments:
 * returnString: A pointer value where the string should be placed.
 * The pointer value should be as long as the value returned by spi_Available().
 *
 * Return: void
 */

void spi_read(char * returnString) {
	int i, j;

	if (noMessages == 0) {
		printf("Error spi_read is called before receiving a spi string");
		return;
	}
	// Disable SSI0 Interrupts
	nvic_interrupt_disable(NVIC_INT_SSI0);
	strcpy(returnString, stringbuffer[0]);
	for (i = 0; i < 24; i++) {
		for (j = 0; j < 200; j++) {
			stringbuffer[i][j] = stringbuffer[i + 1][j];
		}
	}
	// Enable SSI0 Interrupts
	noMessages--;
	nvic_interrupt_enable(NVIC_INT_SSI0);

	return;
}

/*
 * Function:  ssi0_isr
 * --------------------
 * Interrupt service routine. This saves incoming characters to the fifo buffer.
 *
 * Return: void
 */
void ssi0_isr() {
	char receivedValue;
	nvic_interrupt_disable(NVIC_INT_SSI0);
	REG(SSI0_BASE + SSI_ICR) |= 2;
	while ((REG(SSI0_BASE + SSI_SR) & SSI_SR_RNE) == SSI_SR_RNE) { // as long as the recieve buffer is not empty
		receivedValue = REG(SSI0_BASE + SSI_DR);
		if (receivedValue != 0) {
			//MMEM_PTR(&SpiBuffer) = recievedValue;
			receivebuffer[recievecount] = receivedValue;
			//memcpy(MMEM_PTR(&SpiBuffer)+recievecount, &recievedValue,1);
			recievecount++;

			if (receivedValue == '\n') {
				//StringArray[noMessages] = malloc((strlen(receivebuffer) + 1) * sizeof(char));
				receivebuffer[recievecount] = 0;
				strcpy(stringbuffer[noMessages], receivebuffer);
				recievecount = 0;
				noMessages++;
				//process_poll(&tcp_thread);
			}
		}
	}
	REG(SSI0_BASE + SSI_ICR) |= 3;
	if(fillingbuffer == 0){
		while((REG(SSI0_BASE + SSI_SR) & SSI_SR_TNF) == SSI_SR_TNF){
			REG(SSI0_BASE + SSI_DR) = 0;
		}
	}
	nvic_interrupt_enable(NVIC_INT_SSI0);
}

/*
 * Function:  spi_setup
 * --------------------
 * Initializes the spi interface in slave mode.
 *
 * Return: void
 */
void spi_setup(void) {
	spi_enable();

	/* Start by disabling the peripheral before configuring it */
	REG(SSI0_BASE + SSI_CR1) = 0;

	/* Set the IO clock as the SSI clock */
	REG(SSI0_BASE + SSI_CC) = 1;

	// INPUT: Mux port SPI_CLK_PORT / pin SPI_CLK_PIN to SSI0 CLK
	REG (IOC_CLK_SSIIN_SSI0) = (SPI_CLK_PORT * 8) + SPI_CLK_PIN;

	// INPUT: Mux port CC2538_SPI_SEL_PORT_NUM / pin CC2538_SPI_SEL_PIN_NUM to SSI0 CS
	REG (IOC_SSIFSSIN_SSI0) = (SPI_SS_PORT * 8) + SPI_SS_PIN;

	// OUTPUT: Mux MISO output to the port SPI_MISO_PORT / pin SPI_MISO_PIN
	ioc_set_sel(SPI_MISO_PORT, SPI_MISO_PIN, IOC_PXX_SEL_SSI0_TXD);

	// INPUT: Mux port SPI_MOSI_PORT / pin SPI_MOSI_PIN to SSI0 MOSI
	REG (IOC_SSIRXD_SSI0) = (SPI_MOSI_PORT * 8) + SPI_MOSI_PIN;

	// Put all the SSI gpios into peripheral mode
	GPIO_PERIPHERAL_CONTROL(GPIO_PORT_TO_BASE(SPI_CLK_PORT),
			GPIO_PIN_MASK(SPI_CLK_PIN));
	GPIO_PERIPHERAL_CONTROL(GPIO_PORT_TO_BASE(SPI_MOSI_PORT),
			GPIO_PIN_MASK(SPI_MOSI_PIN));
	GPIO_PERIPHERAL_CONTROL(GPIO_PORT_TO_BASE(SPI_MISO_PORT),
			GPIO_PIN_MASK(SPI_MISO_PIN));
	GPIO_PERIPHERAL_CONTROL(GPIO_PORT_TO_BASE(SPI_SS_PORT),
			GPIO_PIN_MASK(SPI_SS_PIN));

	// Disable any control
	ioc_set_over(SPI_CLK_PORT, SPI_CLK_PIN, IOC_OVERRIDE_DIS);
	ioc_set_over(SPI_MOSI_PORT, SPI_MOSI_PIN, IOC_OVERRIDE_DIS);
	ioc_set_over(SPI_MISO_PORT, SPI_MISO_PIN, IOC_OVERRIDE_DIS);
	ioc_set_over(SPI_SS_PORT, SPI_SS_PIN, IOC_OVERRIDE_DIS);

	REG(SSI0_BASE + SSI_CPSR) = 16; //Spi speed to 1Mhz

	REG(SSI0_BASE + SSI_CR0) = 0x07; //8 bit data size

	REG(SSI0_BASE + SSI_CR1) |= SSI_CR1_MS; // Device is "SLAVE"

	// Enable Receive Time-Out interrupt
	REG(SSI0_BASE + SSI_IM) = SSI_IM_RTIM | SSI_IM_RXIM;

	// Clear SSI0 interrupt flags
	REG(SSI0_BASE + SSI_ICR) = SSI_ICR_RORIC | SSI_ICR_RTIC;

	// Enable SSI0 Interrupts
	nvic_interrupt_enable(NVIC_INT_SSI0);

	// Enable the SSI0
	REG(SSI0_BASE + SSI_CR1) |= SSI_CR1_SSE;

	while((REG(SSI0_BASE + SSI_SR) & SSI_SR_TNF) == SSI_SR_TNF){
		REG(SSI0_BASE + SSI_DR) = 0;
	}
	printf("SPI initialization finishes\n");
}