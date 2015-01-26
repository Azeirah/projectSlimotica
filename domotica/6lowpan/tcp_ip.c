#include "tcp_ip.h"

LIST(connection_table);
MEMB(connection_mem, connection, 64);

static struct uip_udp_conn *client_conn;
static uip_ipaddr_t *server_ipaddr;
uip_ipaddr_t server_addr;
static char module_ipaddress[82], module_name[50];
static int module_ipaddress_is_set;
static void (*dev_init_compleet)(int, int);

void tcpip_handler(void) {
	char *str;
	if (uip_newdata()) {
		str = uip_appdata;
		str[uip_datalen()] = '\0';
		printf("DATA recv '%s'\n", str);

		struct tuple json_res;
		char response_value[64];
		char call_method[32];
		char call_params[128];
		json_res.response.result = response_value;
		json_res.call.method = call_method;
		json_res.call.params = call_params;
		decode_json_rpc(str, &json_res);
		char *tok = strtok(json_res.response.result, ", ");

		if (!client_init) {
			if (compare_string("true", json_res.response.result)) { //input string
				client_init = TRUE;
			}
		} else if (!devs_init) {
			if (compare_string("true", tok)) {
				tok = strtok(NULL, ", ");
				dev_init_compleet(custom_atoi(tok), 1);
			}
		} else {
			method_caller(json_res.call.method, json_res.call.params);
		}
	}
}

int init_client() {
	server_ipaddr = set_global_address();
	call_id = FALSE;
	module_ipaddress_is_set = FALSE;
	PRINTF("UDP client process started\n");

	print_local_addresses();

	client_conn = udp_new(NULL, UIP_HTONS(UDP_SERVER_PORT), NULL);
	if (client_conn == NULL) {
		PRINTF("No UDP connection available, exiting the process!\n");
		return 0;
	}
	udp_bind(client_conn, UIP_HTONS(UDP_CLIENT_PORT));

	PRINTF("Created a connection with the server "); PRINT6ADDR(&client_conn->ripaddr); PRINTF(" local/remote port %u/%u\n",
			UIP_HTONS(client_conn->lport), UIP_HTONS(client_conn->rport));

	device_mem_init();
	function_list_init();

	strcpy(module_name, "JustAnotherModule");

	return 1;
}

void initialize_client_on_server() {
	struct tuple json_call;
	char methodName[20] = "addModuleWithIP";
	char parameters[200];
	char param_format[6] = "%s%s";
	char json_string[256];

	if (module_ipaddress_is_set == FALSE) {
		printf("setting mod ipaddress ... : ");
		PRINT6ADDR(&uip_ds6_if.addr_list[1].ipaddr);
		printf("\n");
		sprintf_ipv6_addr(&uip_ds6_if.addr_list[1].ipaddr, module_ipaddress);
		module_ipaddress_is_set = TRUE;
	}

	json_call.id = call_id;
	call_id++;
	sprintf(parameters, "%s,%s", module_name, module_ipaddress);

	json_call.a = JSON_RPC_CALL;
	json_call.call.method = methodName;
	json_call.call.params = parameters;
	json_call.call.params_format = param_format;

	encode_json_rpc(&json_call, json_string);
	printf("id: %d\n", call_id);

	send_to_server(json_string);
}

void send_to_server(char *string) {
	char buf[MAX_PAYLOAD_LEN];
	uip_udp_packet_sendto(client_conn, string, strlen(string), server_ipaddr,UIP_HTONS(UDP_SERVER_PORT));
}

void sics_lpn_init() {
	memb_init(&connection_mem);
	list_init(connection_table);
}

int delete_connection(connection *conn) {
	if (close_connection(conn)) {
		list_remove(connection_table, conn);
		memb_free(&connection_mem, conn);
		return TRUE;
	} else {
#ifdef DEBUG_PRINT
		printf("delete_connection() failed to close connection\n\r");
#endif
		return FALSE;
	}
}

int close_connection(connection *conn) {
	if (uip_connected()) {
		uip_close();
		if (uip_closed()) {
			return TRUE;
			conn->isConnected = FALSE;
		} else {
			return FALSE;
		}
	} else {
		return TRUE;
		conn->isConnected = FALSE;
	}
}

int is_connected(uip_ipaddr_t *ipaddr) {
	return get_connection(ipaddr)->isConnected;
}

void print_local_addresses(void) {
	static int i;
	static uint8_t state;
	printf("IPv6 addresses:\n");
	for (i = 0; i < UIP_DS6_ADDR_NB; i++) {
		state = uip_ds6_if.addr_list[i].state;
		if (uip_ds6_if.addr_list[i].isused
				&& (state == ADDR_TENTATIVE || state == ADDR_PREFERRED)) {
			PRINT6ADDR(&uip_ds6_if.addr_list[i].ipaddr); PRINTF("\n");
		}
	}
}

int amount_of_connections() {
	return list_length(connection_table);
}

void print_connection(connection *conn) {
	PRINT6ADDR(&conn->ipaddress);
	printf("\n\rSocket : %d\n\r", conn->socket);
	printf("Listening : %d\n\r", conn->listening);
	printf("Connected : %d\n\r", conn->isConnected);
}

void copy_ip6addr(uip_ipaddr_t *dst, uip_ipaddr_t *src) {
	int i;
	for (i = 0; i < 16; i++) {
		dst->u16[i] = src->u16[i];
	}
	//PRINT6ADDR(src);
	//PRINT6ADDR(dst);
}

int cmp_ip6addr(uip_ipaddr_t *one, uip_ipaddr_t *two) {
	int i = 0;
	for (i = 0; i < 16; i++) {
		printf("%d\t%d\n\r", one->u16[i], two->u16[i]);
	}
	return TRUE;
}

uip_ipaddr_t * set_global_address(void) {
	uip_ipaddr_t ipaddr;
	uip_ip6addr(&ipaddr, 0xaaaa, 0, 0, 0, 0, 0, 0, 0);
	uip_ds6_set_addr_iid(&ipaddr, &uip_lladdr);
	uip_ds6_addr_add(&ipaddr, 0, ADDR_AUTOCONF);
	//uip_ip6addr(&server_ipaddr, 0xaaaa, 0, 0, 0, 0, 0x00ff, 0xfe00, 1);
	uip_ip6addr(&server_addr, 0xaaaa, 0, 0, 0, 0x0212, 0x4b00, 0x03a6, 0x4cd9); PRINT6ADDR(&server_addr);
	printf("\n");
	return &server_addr;
}

void sprintf_ipv6_addr(uip_ipaddr_t *ipaddr, char buffer[]) {
	sprintf(buffer,
			"%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x",
			ipaddr->u8[0], ipaddr->u8[1], ipaddr->u8[2], ipaddr->u8[3],
			ipaddr->u8[4], ipaddr->u8[5], ipaddr->u8[6], ipaddr->u8[7],
			ipaddr->u8[8], ipaddr->u8[9], ipaddr->u8[10], ipaddr->u8[11],
			ipaddr->u8[12], ipaddr->u8[13], ipaddr->u8[14], ipaddr->u8[15]);
	printf("sprintf(): %s\n", buffer);
}

void string_to_ipv6_addr(uip_ipaddr_t *ipaddr, char buffer[]) {
	char *p;
	int i = 0;
	p = strtok(buffer, ":");

	for (i = 0; i < 16 || p != NULL; i++, p = strtok(NULL, ":")) {
		char temp[7] = "0x";
		strcat(temp, p);

		ipaddr->u8[i] = string_to_hex(temp) >> 8;
		ipaddr->u8[i + 1] = string_to_hex(temp) & 0xFF;
		i++;
	} PRINT6ADDR(ipaddr);
}
