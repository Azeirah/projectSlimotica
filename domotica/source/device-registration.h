/*
 * dev-registration.h
 *
 *  Created on: Jan 2, 2015
 *      Author: tackticaldude
 */

#ifndef SRC_DEVICE_REGISTRATION_H_
#define SRC_DEVICE_REGISTRATION_H_

#include "contiki-net.h"
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "tcp_ip.h"
#include "json-rpc.h"

static int call_id;

typedef enum{
	DEGREES_CELSIUS = 1,
	LIGHT_VALUE 	= 2,
	/** pls add more **/
}unit_t;

typedef enum{
	SWITCH = '1',
	BUTTON = '2',
	SLIDER = '3',
	VALUE  = '4',
	EVENT  = '5',
	COLOR  = '6',
	TIME   = '7'
}interface_t;

typedef enum{
	BOOL 	= 1,
	INT  	= 2,
	FLOAT 	= 3,
	STRING 	= 4
}type_t;

typedef enum{
	SENSOR 		= '1',
	ACTUATOR 	= '2'
}sensor_t;

typedef struct{
	uint8_t *next;
	uint8_t deviceId;
	char deviceName[10];
	char icon[10];
	type_t type;
	unit_t unit;
	char min[10];
	char max[10];
	interface_t interfaceDisplay;
	float logTime;
	sensor_t sensor;
	uint8_t initialized;
}device;

void device_mem_init();
device *create_device(uint8_t,char *,char *,type_t,unit_t,char *,char *,interface_t,float,sensor_t);
void create_rpc_call(char *,char *, int, char *,char *,int,char *,char *,char *,int,int,int);
void register_device_on_server(device *);
void register_all_devs_on_server();
int are_devs_registered();
void add_device(device *dev);
void init_device_complete(device *dev);
void delete_device(device *dev);
void delete_device_by_name(char *name);
void delete_device_by_id(uint8_t id);
device *get_device_by_name(char *name);
device *get_device_by_id(uint8_t id);
char *get_unit_type(device *dev);
char *get_interface_type(device *dev);
int get_interface_type_int(device *dev);
char *get_sensor_type(device *dev);
int get_sensor_type_int (device *dev);
char *get_type_in_string(device *dev);
int get_type_in_int(device *dev);
void print_device_list();
#endif /* SRC_DEVICE_REGISTRATION_H_ */
