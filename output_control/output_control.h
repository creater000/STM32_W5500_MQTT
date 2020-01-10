#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>




#define LED_GPIO_RCC			RCC_APB2Periph_GPIOB
#define LED_PORT		        GPIOB
#define LED_PIN			        GPIO_Pin_1


#define BUZZER_GPIO_RCC			RCC_APB2Periph_GPIOB
#define BUZZER_PORT		        GPIOB
#define BUZZER_PIN			GPIO_Pin_5
 

#define TURN_ON_LED                     0
#define TURN_OFF_LED                    1
#define TURN_ON_BUZZER                  2
#define TURN_OFF_BUZZER                 3


#define LEN_BUFFER_CONTROL             100
void init_LED_output();
void init_buzzer();
void turn_on_LED();
void turn_off_LED();
void turn_on_buzzer();
void turn_off_buzzer();
void process_message_control();
