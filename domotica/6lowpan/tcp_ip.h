#ifndef _6LPN_H_
#define _6LPN_H_

#include "contiki-net.h"
#include "net/ip/uip.h"
#include "device-registration.h"
#include "net/ip/uip-debug.h"
#include <stdlib.h>

#include "util.h"
#define PERIOD 1
#define SEND_INTERVAL	(PERIOD * CLOCK_SECOND)
#define SEND_TIME		(random_rand() % (SEND_INTERVAL))

#define UIP_IP_BUF ((struct uip_ip_hdr *)&uip_buf[UIP_LLH_LEN])
#define UDP_CLIENT_PORT	8765
#define UDP_SERVER_PORT	5678
#define UDP_EXAMPLE_ID 190
#define MAX_PAYLOAD_LEN	256

static int client_init = FALSE, devs_init = FALSE;

typedef struct{
	uip_ipaddr_t ipaddress;
	int socket;
	int isConnected;
	int listening;
}connection;

void send_rpc_packet(void *ptr);

int init_client();
void sics_lpn_init();
void tcpip_handler(void);
void initialize_client_on_server();
void send_to_server(char *string);

connection *get_connection(uip_ipaddr_t *);
int close_connection(connection *);
int delete_connection(connection *);
int amount_of_connections();
int is_connected(uip_ipaddr_t *);

uip_ipaddr_t *set_global_address(void);
void print_local_addresses(void);
void copy_ip6addr(uip_ipaddr_t *dst, uip_ipaddr_t *src);
void print_connection(connection *conn);
void sprintf_ipv6_addr(uip_ipaddr_t *, char []);
void string_to_ipv6_addr(uip_ipaddr_t *, char []);
int cmp_ip6addr(uip_ipaddr_t *, uip_ipaddr_t *);

#endif/*_6LPN_H_*/
