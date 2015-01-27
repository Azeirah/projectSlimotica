/*
 * util.h
 *
 *  Created on: Dec 18, 2014
 *      Author: tackticaldude
 */

#define TRUE 	1
#define FALSE 	0

#ifndef SRC_UTIL_H_
#define SRC_UTIL_H_

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include "net/ip/uip.h"

char* custom_itoa(int, char[]);
int custom_atoi( char *);
int compare_string(char *, char *);
void custom_strcat(char *,  char *);
unsigned int string_to_hex(const char * s);

#endif /* SRC_UTIL_H_ */
