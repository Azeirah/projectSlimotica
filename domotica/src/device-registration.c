/*
 * dev_registration.c
 *
 *  Created on: Jan 2, 2015
 *      Author: tackticaldude
 */

#include "device-registration.h"

LIST(device_table);
MEMB(device_mem, device, 32);
static int device_table_init = FALSE;

void device_mem_init(){
	memb_init(&device_mem);
	list_init(device_table);
	device_table_init= TRUE;
}

device *
create_device(uint8_t devId,
		char *name,
		char *icon,
		type_t type,
		unit_t unit,
		char *min,
		char *max,
		interface_t interf,
		float logTime,
		sensor_t sensor){
	device *newDev;
	newDev = memb_alloc(&device_mem);

	if(newDev!=NULL){
		newDev->deviceId = devId;
		strncpy(newDev->deviceName, name, strlen(name));
		strncpy(newDev->icon, icon, strlen(icon));
		newDev->type = type;
		newDev->unit = unit;
		strncpy(newDev->min, min, strlen(min));
		strncpy(newDev->max, max, strlen(max));
		newDev->interfaceDisplay = interf;
		newDev->logTime = logTime;
		newDev->sensor = sensor;
		newDev->initialized = FALSE;
		printf("create_device(): created new dev\n");
	}else{
		printf("create_device(): failed to allocate memory\n");
		return NULL;
	}

	return newDev;
}


void create_device_register_call(char *json_string, char *method, int id, char *devName, char *icon, int type, char *unit, char *min, char *max, int inter, int log, int sensor){
	struct tuple json_call; //= malloc(sizeof(struct tuple));
	char methodName[20];
	char parameters[200];
	char param_format[64] = "%d,%s,%s,%d,%s,%s,%s,%d,%d,%d";

	json_call.id = call_id;
	call_id++;
	strcpy(methodName, method);
	sprintf(parameters, "%d,%s,%s,%d,%s,%s,%s,%d,%d,%d",id,devName,icon,type,unit,min,max,inter,log,sensor);

	json_call.a = JSON_RPC_CALL;
	json_call.call.method = methodName;
	json_call.call.params = parameters;
	json_call.call.params_format = param_format;

	encode_json_rpc(&json_call, json_string);
}

void register_device_on_server(device *newDevice){
	char jsonString[256];
	create_device_register_call(jsonString,"addDevice",
			newDevice->deviceId,
			newDevice->deviceName,
			newDevice->icon,
			get_type_in_int(newDevice),
			get_unit_type(newDevice),
			newDevice->min,
			newDevice->max,
			get_interface_type_int(newDevice),
			newDevice->logTime,
			get_sensor_type_int(newDevice));
	send_to_server(jsonString);
	printf("Complete json String: %s\n", jsonString);
}

void register_all_devs_on_server(){
	device *newDevice;
	if(list_length(device_table)!=0){
		for(newDevice=list_head(device_table);
			newDevice!=NULL;
			newDevice=list_item_next(newDevice)){
			if(!newDevice->initialized){
				register_device_on_server(newDevice);
			}
		}
	}else{
		printf("register_all_devs(): list is empty");
	}
}

int are_devs_registered(){
	device *newDevice;
	for(newDevice=list_head(device_table);
		newDevice!=NULL;
		newDevice=list_item_next(newDevice)){
		if(newDevice->initialized==FALSE){
			return 0;
		}
	}
	return 1;
}

void add_device(device *dev){
	if(dev!=NULL){
		if(device_table_init){
			list_add(device_table, dev);
			printf("add_device(): add dev to list\n");
		}
		else 	printf("add_device(): list not initialized\n");
	}else printf("add_device(): pointer to device is NULL\n");
}

void delete_device(device *dev){
	list_remove(device_table, dev);
	memb_free(&device_mem, dev);
}

void init_device_complete(device *dev){
	dev->initialized = TRUE;
}

void delete_device_by_name(char *name){
	device *d;
	for(d=list_head(device_table);
		d!=NULL;
		d=list_item_next(d)){
		if(compare_string(d->deviceName, name)){
			list_remove(device_table, d);
			memb_free(&device_mem, d);
			break;
		}
	}
}

void delete_device_by_id(uint8_t id){
	device *d;
	for(d=list_head(device_table);
		d!=NULL;
		d=list_item_next(d)){
		if(d->deviceId==id){
			list_remove(device_table, d);
			memb_free(&device_mem, d);
			break;
		}
	}
}

device *get_device_by_name(char *name){
	device *d;
	for(d=list_head(device_table);
		d!=NULL;
		d=list_item_next(d)){
		if(compare_string(d->deviceName, name)){
			return d;
		}
	}
	return NULL;
}

device *get_device_by_id(uint8_t id){
	device *d;
	for(d=list_head(device_table);
		d!=NULL;
		d=list_item_next(d)){
		if(d->deviceId==id){
			return d;
		}
	}
	return NULL;
}

char *get_unit_type(device *dev){
	switch(dev->unit){
	case DEGREES_CELSIUS : return "Degrees_Celsius";
	case LIGHT_VALUE : return "Light Value";
	default: return "Unknown Type";
	}
}

char *get_interface_type(device *dev){
	switch(dev->interfaceDisplay){
	case SWITCH: return "Switch";
	case BUTTON: return "Button";
	case SLIDER: return "Slider";
	case VALUE:  return "Value";
	case EVENT:  return "Event";
	case COLOR:	 return "Color";
	case TIME:	 return "Time";
	default: return "Unknown Type";
	}
}

int get_interface_type_int(device *dev){
	switch(dev->interfaceDisplay){
	case SWITCH: return 1;
	case BUTTON: return 2;
	case SLIDER: return 3;
	case VALUE:  return 4;
	case EVENT:  return 5;
	case COLOR:	 return 6;
	case TIME:	 return 7;
	default: 	 return 0;
	}
}


char *get_sensor_type(device *dev){
	switch(dev->sensor){
	case SENSOR: 	return "Sensor";
	case ACTUATOR: 	return "Actuator";
	default: 		return "Unknown Type";
	}
}

int get_sensor_type_int(device *dev){
	switch(dev->sensor){
	case SENSOR: 	return 1;
	case ACTUATOR: 	return 2;
	default: 		return 0;
	}
}

char *get_type_in_string(device *dev){
	switch(dev->type){
	case BOOL:		return "Bool";
	case INT:		return "Int";
	case FLOAT:		return "Float";
	case STRING:	return "String";
	default: 		return "Unknown Type";
	}
}

int get_type_in_int(device *dev){
	switch(dev->type){
	case BOOL:		return 1;
	case INT:		return 2;
	case FLOAT:		return 3;
	case STRING:	return 4;
	default: 		return 0;
	}
}

void print_device_list(){
	device *dev;
	for(dev = list_head(device_table);
		dev!= NULL;
		dev = list_item_next(dev)){
		printf("Device %d: %s\n", dev->deviceId, dev->deviceName);
		printf("Icon: %s, Type: %d\n", dev->icon, dev->type);
		printf("Unit: %s, Max: %s, Min: %s\n", get_unit_type(dev), dev->min, dev->max);
		printf("Display: %s, Sensor: %s\n", get_interface_type(dev), get_sensor_type(dev));
		printf("--------\t--------\t--------\n");
	}
	printf("End print \t ------------------------------\n");
}
