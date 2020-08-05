#include "stm32f10x.h"
#include "timer.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
__IO uint16_t CCR1_Val = 0xFFFF;
__IO uint16_t CCR2_Val = 27309;
__IO uint16_t CCR3_Val = 13654;
__IO uint16_t CCR4_Val = 6826;
uint16_t PrescalerValue = 0;

void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the TIM2 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure);
}

void init_timer2()
{
  /* PCLK1 = HCLK/4 */
  RCC_PCLK1Config(RCC_HCLK_Div16);
    /* TIM2 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  NVIC_Configuration();
  /* ---------------------------------------------------------------
    TIM2 Configuration: Output Compare Timing Mode:
    TIM2 counter clock at 6 MHz
    CC1 update rate = TIM2 counter clock / CCR1_Val = 146.48 Hz
    CC2 update rate = TIM2 counter clock / CCR2_Val = 219.7 Hz
    CC3 update rate = TIM2 counter clock / CCR3_Val = 439.4 Hz
    CC4 update rate = TIM2 counter clock / CCR4_Val = 878.9 Hz
  --------------------------------------------------------------- *
  /* Compute the prescaler value */
  PrescalerValue = (uint16_t) (SystemCoreClock / 500000) - 1;
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 65535;
  TIM_TimeBaseStructure.TIM_Prescaler = 0xFFFF;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  /* Prescaler configuration */
  TIM_PrescalerConfig(TIM2, PrescalerValue, TIM_PSCReloadMode_Immediate);

  /* Output Compare Timing Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  TIM_OC1Init(TIM2, &TIM_OCInitStructure);

  /* TIM IT enable */
  TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);

  /* TIM2 enable counter */
  TIM_Cmd(TIM2, ENABLE);
}

void disable_timer2()
{
  /* TIM2 enable counter */
  TIM_Cmd(TIM2, DISABLE);
}