#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stm32f10x.h>
#include "uart_debug.h"



void USART1Initialze(void)
{
  /* GPIO for USART clock enable */
  
  RCC_APB2PeriphClockCmd(USART1_GPIO_RCC,ENABLE);
  
  /* USART1 clock enable */
  RCC_APB2PeriphClockCmd(USART1_RCC,ENABLE);
  
  /*GPIO initialize configuration structure*/
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /*GPIO for USART RX configuration */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin = USART1_RX_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(USART1_RX_PORT,&GPIO_InitStructure);
  
  /*GPIO for USART TX configuration */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin = USART1_TX_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(USART1_TX_PORT,&GPIO_InitStructure);
  
  /*USART initialize configuration structure*/
  USART_InitTypeDef USART_InitStructure;
  
  /* USART1 configuration */
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  
  /* USART1 intialize */
  USART_Init(USART1, &USART_InitStructure);
  
  /* USART1 Enable */
  USART_Cmd(USART1, ENABLE);
}
void send_buffer_uart(char* buffer,uint16_t len)
{
  uint16_t i=0;
  for(i=0;i<len;i++)
  {
    USART_SendData(USART1,buffer[i]);
    while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
  } 
  USART_SendData(USART1,'\r');
    while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
    USART_SendData(USART1,'\n');
    while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
}