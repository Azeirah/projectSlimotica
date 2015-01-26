

/** Client RPL_TCP **/
#include "contiki.h"
#include "contiki-net.h"
#include "lib/random.h"
#include "sys/ctimer.h"
#include "net/ip/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ip/uip-udp-packet.h"
#include "sys/ctimer.h"

#include <stdio.h>
#include <string.h>

#include "device-registration.h"
#include "6lowpan/tcp_ip.h"
#include "spi/spi.h"
#include "spi/spi.c"

device *one, *two, *three;
process_event_t spi_event;

PROCESS(udp_client_process, "UDP client process");
AUTOSTART_PROCESSES(&udp_client_process);

PROCESS_THREAD(udp_client_process, ev, data)
{
  static struct etimer periodic;
  static struct ctimer backoff_timer;

  PROCESS_BEGIN();
  PROCESS_PAUSE();

  if(!init_client()) PROCESS_EXIT();

  one = create_device(1, "DevOne", "IconOne", INT, DEGREES_CELSIUS, "0", "100", SLIDER, 1, SENSOR);
  two = create_device(2, "DevTwo", "IconTwo", INT, LIGHT_VALUE, "0", "25", SLIDER, 1, SENSOR);
  three = create_device(3, "DevThree", "IconThree", INT, DEGREES_CELSIUS, "25", "100", SLIDER, 1, ACTUATOR);

  add_device(one);
  add_device(two);
  add_device(three);

  print_device_list();

  etimer_set(&periodic, SEND_TIME);
  while(1) {
    PROCESS_YIELD();
    if(ev == tcpip_event) {
      tcpip_handler();
    }
    devs_init = are_devs_registered();
    if(etimer_expired(&periodic)) {
    	etimer_reset(&periodic);
    	if(client_init==FALSE){
    		ctimer_set(&backoff_timer, SEND_TIME, initialize_client_on_server, NULL);
    	}else if(devs_init==FALSE){
    		ctimer_set(&backoff_timer, SEND_TIME, register_all_devs_on_server, NULL);
    	}else{
    		//custom function
    	}
	}
  }
  PROCESS_END();
}
