#define PRINT_DEBUG
#define _DHCP_DEBUG_

#define SOCK_TCPS                       0
#define SOCK_UDPS                       1
#define SOCK_DHCP	                7
#define BUFFER_SIZE	                2048

#define MQTT_SERVER_VERSION             4
#define IP_SERVER                       {95, 217, 164, 7}
#define MQTT_CHANNEL                    "/led_control"
#define USER_SERVER                     "user01"
#define PASS_SERVER                     "123456"
#define PORT_SERVER                     1883

void delay(uint32_t ui32_time);
void pubplish();