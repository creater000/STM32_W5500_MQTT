
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stm32f10x.h>
#include <stdbool.h>
#include "output_control.h"
#include "process_string.h"

// json control:   {"CMD":x}  x is number control

uint32_t ui32_value_control = 0;
extern uint32_t ui32_counter_timer2;
uint32_t ui32_len_buffer_control = 0;
char buffer_control[LEN_BUFFER_CONTROL]={0};
uint32_t is_process_data = false;

void init_LED_output()
{
  /*Initialize Structure*/
  GPIO_InitTypeDef	GPIO_InitStructure;
  /*Enable clock related Periphs*/
  RCC_APB2PeriphClockCmd(LED_GPIO_RCC,ENABLE);
  /*Initialize LED Pin*/
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = LED_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(LED_PORT,&GPIO_InitStructure);
}

void init_buzzer()
{
  /*Initialize Structure*/
  GPIO_InitTypeDef	GPIO_InitStructure;
  /*Enable clock related Periphs*/
  RCC_APB2PeriphClockCmd(BUZZER_GPIO_RCC,ENABLE);
  /*Initialize BUZZER Pin*/
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = BUZZER_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(BUZZER_PORT,&GPIO_InitStructure);
}

void turn_on_LED()
{
  GPIO_SetBits(LED_PORT, LED_PIN);
}
void turn_off_LED()
{
  GPIO_ResetBits(LED_PORT, LED_PIN);
}
void turn_on_buzzer()
{
  GPIO_SetBits(BUZZER_PORT, BUZZER_PIN);
}
void turn_off_buzzer()
{
  GPIO_ResetBits(BUZZER_PORT, BUZZER_PIN);
}



void process_message_control()
{
  if(is_process_data == true)
  {
    is_process_data = false;
    if(getValueInt(buffer_control,0,ui32_len_buffer_control,FIELD_CONTROL_OUPUT,(uint32_t *)&ui32_value_control)==EXIST_FIELD)
    {      
      switch(ui32_value_control)
      {
        case TURN_ON_LED:
          init_timer2();
          break;
        case TURN_OFF_LED:
          disable_timer2();
          display_number(0);
          break;               
        default:
          ui32_counter_timer2 = ui32_value_control;
          break;
      }
      
    }
    
  }
  
  
}
