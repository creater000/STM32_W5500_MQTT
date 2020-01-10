#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stm32f10x.h>

#define USART1_GPIO_RCC RCC_APB2Periph_GPIOA
#define USART1_RCC		RCC_APB2Periph_USART1
#define USART1_RX_PORT	GPIOA
#define USART1_RX_PIN	GPIO_Pin_10
#define USART1_TX_PORT	GPIOA
#define USART1_TX_PIN	GPIO_Pin_9

void USART1Initialze(void);
void send_buffer_uart(char* buffer,uint16_t len);