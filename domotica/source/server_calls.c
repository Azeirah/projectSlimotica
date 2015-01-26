/*
 * server_calls.c
 *
 *  Created on: Jan 22, 2015
 *      Author: tackticaldude
 */

#include "server_calls.h"

void getSensorValue(int devId){

}

void setSensorValue(int moduleId, int devId, int value){
	struct tuple json_call; //= malloc(sizeof(struct tuple));
	char methodName[20] = "setSensorValue";
	char parameters[200];
	char json_string[256];
}

void setActuatorValue(int devId, int value){

}

void getActuatorValue(int moduleId, int devId, int value){
	struct tuple json_call; //= malloc(sizeof(struct tuple));
	char methodName[20] = "getActuatorValue";
	char parameters[200];
	char json_string[256];
	char tempString[12];
	char param_format[20]= "%d%d%d";

	custom_itoa(moduleId, tempString);
	strcat(parameters, tempString);
	strcat(parameters, ",");

	custom_itoa(devId, tempString);
	strcat(parameters, tempString);
	strcat(parameters, ",");

	custom_itoa(value, tempString);
	strcat(parameters, tempString);

	json_call.call.method = methodName;
	json_call.call.params = parameters;
	json_call.call.params_format = param_format;
	json_call.a = JSON_RPC_CALL;

	encode_json_rpc(&json_call, json_string);
}

void enableDeviceOnServer(){
	struct tuple json_call; //= malloc(sizeof(struct tuple));
	char methodName[20] = "enableDevice";
	char parameters[200];
	char json_string[256];
	char tempString[12];
	char param_format[20]= "%d%d%d";
}

void disableDeviceOnServer(){

}

void enableModeleOnServer(){

}

void ping(){

}

void pong(){

}
