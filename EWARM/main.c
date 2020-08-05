/*Include: Board configuration*/
#include "dhcp.h"
/*Include: MCU peripheral Library*/
#include "stm32f10x_rcc.h"
#include "stm32f10x.h"
#include "main.h"
/*Include: W5500 iolibrary*/
#include "w5500.h"
#include "wizchip_conf.h"
#include <stdbool.h>
#include "mqtt_interface.h"
#include "MQTTClient.h"
#include "uart_debug.h"
/*Include: MCU Specific W5500 driver*/
#include "W5500HardwareDriver.h"

/*Include: Standard IO Library*/
#include <stdio.h>
#include "net_config.h"
#include "output_control.h"
#include "spi_74hc595.h"
#include "timer.h"


extern uint32_t ui32_counter_timer2;

extern uint32_t ui32_len_buffer_control;
extern char buffer_control[LEN_BUFFER_CONTROL];
extern uint32_t is_process_data;

uint8_t targetIP[4] = IP_SERVER;
int32_t rc = 0;
uint32_t dhcp_ret = DHCP_STOPPED;
uint8_t buf[100]; 
Network n;
Client c;
char SubString[] = "/led_control";
uint32_t ui32_len_message = 0;
char message[16] = {0};
MQTTMessage pubMessage;


uint8_t tempBuffer[BUFFER_SIZE];
// Global variables
uint32_t dhcp_counter;
uint8_t mqtt_push_counter;
uint8_t mqtt_flag;
uint16_t mes_id;

wiz_NetInfo gWIZNETINFO = 
{
  .mac = {ETHADDR0, ETHADDR1, ETHADDR2, ETHADDR3, ETHADDR4, ETHADDR5},
  .ip = {IPADDR0, IPADDR1, IPADDR2, IPADDR3},
  .sn = {NETMASK0, NETMASK1, NETMASK2, NETMASK3},
  .gw = {DRIPADDR0, DRIPADDR1, DRIPADDR2, DRIPADDR3},
  .dns = {DRIPADDR0, DRIPADDR1, DRIPADDR2, DRIPADDR3},
  .dhcp = NETINFO_DHCP  // NETINFO_STATIC
};


void msTick_Handler(void) 
{
  MilliTimer_Handler();
  
  if (++dhcp_counter >= 1000)  
  {
    dhcp_counter = 0;
    DHCP_time_handler(); 
    
    if (++mqtt_push_counter >= 10)  
    {
      mqtt_push_counter = 0;
      mqtt_flag = 1;
    }
  }
}

void print_network_information(void)
{
  wizchip_getnetinfo(&gWIZNETINFO);
  printf("Mac address: %02x:%02x:%02x:%02x:%02x:%02x\n\r", gWIZNETINFO.mac[0], gWIZNETINFO.mac[1], gWIZNETINFO.mac[2], gWIZNETINFO.mac[3], gWIZNETINFO.mac[4], gWIZNETINFO.mac[5]);
  if (gWIZNETINFO.dhcp == NETINFO_DHCP)
    printf("DHCP\n\r");
  else
    printf("Static IP\n\r");
  printf("IP address : %d.%d.%d.%d\n\r", gWIZNETINFO.ip[0], gWIZNETINFO.ip[1], gWIZNETINFO.ip[2], gWIZNETINFO.ip[3]);
  printf("SM Mask    : %d.%d.%d.%d\n\r", gWIZNETINFO.sn[0], gWIZNETINFO.sn[1], gWIZNETINFO.sn[2], gWIZNETINFO.sn[3]);
  printf("Gate way   : %d.%d.%d.%d\n\r", gWIZNETINFO.gw[0], gWIZNETINFO.gw[1], gWIZNETINFO.gw[2], gWIZNETINFO.gw[3]);
  printf("DNS Server : %d.%d.%d.%d\n\r", gWIZNETINFO.dns[0], gWIZNETINFO.dns[1], gWIZNETINFO.dns[2], gWIZNETINFO.dns[3]);
}

void messageArrived(MessageData* md)
{
  uint32_t i=0;
  MQTTMessage* message = md->message;  
  char* data;
  for (i = 0; i < md->topicName->lenstring.len; i++)
  {
    putchar(*(md->topicName->lenstring.data + i));
  } 
  if(ui32_len_buffer_control>LEN_BUFFER_CONTROL)
  {
    ui32_len_buffer_control = LEN_BUFFER_CONTROL;
  }
  else
  {
    ui32_len_buffer_control = (int32_t)message->payloadlen;
  } 
  data = (char*)message->payload;
  free_buffer(buffer_control,LEN_BUFFER_CONTROL);  
  memcpy(buffer_control,data,ui32_len_buffer_control);
  printf(" Message: %s\r\n", buffer_control);
  is_process_data = true;
}

uint8_t DHCP_proc(void)
{
  uint8_t dhcp_res = DHCP_run();
  switch (dhcp_res)
  {
  case DHCP_IP_ASSIGN:
  case DHCP_IP_CHANGED:
  case DHCP_IP_LEASED:
    getIPfromDHCP(gWIZNETINFO.ip);
    getGWfromDHCP(gWIZNETINFO.gw);
    getSNfromDHCP(gWIZNETINFO.sn);
    getDNSfromDHCP(gWIZNETINFO.dns);
    gWIZNETINFO.dhcp = NETINFO_DHCP;
    ctlnetwork(CN_SET_NETINFO, (void*)&gWIZNETINFO);
    // For debug
    // printf("\r\n>> DHCP IP Leased Time : %ld Sec\r\n", getDHCPLeasetime());
    break;
  case DHCP_FAILED:
    printf(">> DHCP Failed\r\n");
    gWIZNETINFO.dhcp = NETINFO_STATIC;
    break;
  }
  return dhcp_res;
}


int8_t str_printf(char *StrBuff, uint8_t BuffLen, const char *args, ...)
{
  va_list ap;
  va_start(ap, args);
  int8_t len = vsnprintf(StrBuff, BuffLen, args, ap);
  va_end(ap);
  return len;
}

int main(void)
{
  uint8_t status = 0; 
  //int i;
  int rc = 0;
  unsigned char buf[100];
  /* PCLK2 = HCLK/2 */
  RCC_PCLK2Config(RCC_HCLK_Div2);
  /*Usart initialization for Debug.*/
  USART1Initialze();
  init_timer2();
  init_gpio_spi();
  init_spi2();  
  printf("USART initialized.\n\r");
  init_LED_output();
  init_buzzer();    
  /*W5500 initialization.*/
  W5500HardwareInitilize();
  printf("W5500 hardware interface initialized.\n\r");
  
  W5500Initialze();
  printf("W5500 IC initialized.\n\r");
  
  /*Set network informations*/
  wizchip_setnetinfo(&gWIZNETINFO);
  
  setSHAR(gWIZNETINFO.mac);
  if (gWIZNETINFO.dhcp == NETINFO_DHCP)
  {
    DHCP_init(SOCK_DHCP, tempBuffer); 
    while( !((dhcp_ret == DHCP_IP_ASSIGN) || (dhcp_ret == DHCP_IP_CHANGED) || (dhcp_ret == DHCP_FAILED) || (dhcp_ret == DHCP_IP_LEASED)))
    {
      dhcp_ret = DHCP_proc();      
    } 
  }
  print_network_information();  
  n.my_socket = 0; 
  n.my_socket = 0;  
  NewNetwork(&n);
  ConnectNetwork(&n, targetIP, PORT_SERVER);
  MQTTClient(&c, &n, 1000, buf, 100, tempBuffer, BUFFER_SIZE);  
  MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
  data.willFlag = 0;
  data.MQTTVersion = MQTT_SERVER_VERSION;
  data.clientID.cstring = USER_SERVER;
  data.username.cstring = USER_SERVER;
  data.password.cstring = PASS_SERVER;
  data.keepAliveInterval = 60;
  data.cleansession = 1;
  rc = MQTTConnect(&c, &data);
  
  printf("Connected %d\r\n", rc);
  rc = MQTTSubscribe(&c, SubString, QOS0, messageArrived);
  printf("Subscribed (%s) %d\r\n", SubString, rc);
  
  while(1)
  {
    if (gWIZNETINFO.dhcp == NETINFO_DHCP)
    {
      dhcp_ret = DHCP_proc();
    }         
    if (mqtt_flag)
    {
      mqtt_flag = 0;     
      ui32_len_message = str_printf(message, sizeof(message), "%d.%d.%d.%d", gWIZNETINFO.ip[0], gWIZNETINFO.ip[1], gWIZNETINFO.ip[2], gWIZNETINFO.ip[3]);
      if (ui32_len_message > 0)
      {        
        pubMessage.qos = QOS0;
        pubMessage.id = mes_id++;
        pubMessage.payloadlen = ui32_len_message;
        pubMessage.payload = message;
        MQTTPublish(&c,MQTT_CHANNEL, &pubMessage);
      }
    }
    MQTTYield(&c, 1000);
    process_message_control();   
  }
}

void pubplish()
{
  pubMessage.qos = QOS0;
  pubMessage.id = mes_id++;
  pubMessage.payloadlen = 4;
  pubMessage.payload = "test";
  MQTTPublish(&c,MQTT_CHANNEL, &pubMessage);
}

void delay(uint32_t ui32_time)
{
  uint32_t i = 0;
  for(i=0;i<ui32_time;i++)
  {
      
  }
}