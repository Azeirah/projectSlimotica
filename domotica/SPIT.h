/*
 * SPIT.h
 *
 * Created: 20-Jan-15 16:14:51
 *  Author: Dodedodo33
 */ 


#ifndef SPIT_H_
#define SPIT_H_

int spi_available();
void spi_write(char string[]);
void spi_read(char * returnString);
void spi_setup(void);

#include "contiki.h"
#include "../cpu/cc2538/spi-arch.h"
//#include "dev/spi.h"
//#include "reg.h"
//#include "spi-arch.h"
#include "dev/ioc.h"
//#include "dev/sys-ctrl.h"
//#include "dev/ssi.h"
//#include "dev/gpio.h"
//#include "lib/mmem.h"
//#include "dev/watchdog.h"
//#include <stdio.h>

#endif /* SPIT_H_ */