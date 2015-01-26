#ifndef SRC_JSON_RPC_H_
#define SRC_JSON_RPC_H_

#include "jsmn.h"
#include "util.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lib/list.h"
#define USING_CONTIKI
#ifdef USING_CONTIKI

#include "lib/memb.h"
#define USING_PRINTF
#endif // USING_CONTIKI

#define ERROR_METHOD

#define JSON_RPC_NOT_ASSIGNED 	0
#define JSON_RPC_CALL 			1
#define JSON_RPC_RESPONSE 		2

typedef struct
{
	char function_name[32];
	void (*function);
}function_t;

typedef struct
{
    char *method;
    char *params;
    char *params_format;
} call_t;

typedef struct
{
    char *result;
} response_t;

struct tuple
{
    uint8_t a;
    uint16_t id;
    response_t response;
    call_t call;
};

void function_list_init();
int add_function(char function_name[], void (*fp));
int delete_function(void (*fp));
int method_caller(char method_name[], char params[]);
uint8_t jsoneq(const char *, jsmntok_t *, const char *);
void json_rpc_handler(call_t *);
void encode_json_rpc(struct tuple *, char *);
void response_to_string(response_t *, char *);
void call_to_string(call_t *, char *);
void decode_json_rpc(char *, struct tuple *);
void get_array_from_tuple(struct tuple *, char [][50],uint8_t);
char *strtok_two(char *s, const char *);
void get_tag_value_from_jason(char *, char *, char *);


#endif /* SRC_JSON_RPC_H_ */
