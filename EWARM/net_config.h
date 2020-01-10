#ifndef __NET_CONFIG_H__
#define __NET_CONFIG_H__


// MAC-адрес
#define ETHADDR0        0x00 // The first octet of the Ethernet address 
#define ETHADDR1        0x08 // The second octet of the Ethernet address
#define ETHADDR2        0xDC // The third octet of the Ethernet address 
#define ETHADDR3        0x47 // The fourth octet of the Ethernet address
#define ETHADDR4        0x47 // The fifth octet of the Ethernet address 
#define ETHADDR5        0x54 // The sixth octet of the Ethernet address 
// Параметры IP (если DHCP отключен)
#define IPADDR0         192  // The first octet of the IP address of this uIP node
#define IPADDR1         168  // The second octet of the IP address of this uIP node
#define IPADDR2         1    // The third octet of the IP address of this uIP node
#define IPADDR3         137  // The fourth octet of the IP address of this uIP node
#define NETMASK0        255  // The first octet of the netmask of this uIP node
#define NETMASK1        255  // The second octet of the netmask of this uIP node
#define NETMASK2        255  // The third octet of the netmask of this uIP node
#define NETMASK3        0    // The fourth octet of the netmask of this uIP node
#define DRIPADDR0       192  // The first octet of the IP address of the default router
#define DRIPADDR1       168  // The second octet of the IP address of the default router
#define DRIPADDR2       1    // The third octet of the IP address of the default router
#define DRIPADDR3       1    // The fourth octet of the IP address of the default router


#endif //__NET_CONFIG_H__
