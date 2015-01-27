/*
 * Server_calls.h
 *
 *  Created on: Jan 22, 2015
 *      Author: tackticaldude
 */

#ifndef SRC_SERVER_CALLS_H_
#define SRC_SERVER_CALLS_H_

#include "json-rpc.h"

void getSensorValue(int);
void setSensorValue(int, int, int);
void setActuatorValue(int, int);
void getActuatorValue(int, int, int);
void ping();
void pong();

#endif /* SRC_SERVER_CALLS_H_ */
