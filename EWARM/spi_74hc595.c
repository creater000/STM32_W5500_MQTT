#include "stm32f10x.h"
#include "spi_74hc595.h"


SPI_InitTypeDef SPI_InitStructure;

void init_spi2()
{
  /* SPI_MASTER configuration ------------------------------------------------*/
  SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI_MASTER, &SPI_InitStructure);
  
  /* Enable SPI_MASTER TXE interrupt */
  SPI_I2S_ITConfig(SPI_MASTER, SPI_I2S_IT_TXE, ENABLE);
  
  /* Enable SPI_MASTER */
  SPI_Cmd(SPI_MASTER, ENABLE);
}

void init_gpio_spi()
{
  /* Enable GPIO clock for SPI_MASTER and SPI_SLAVE */
  RCC_APB2PeriphClockCmd(SPI_MASTER_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB1PeriphClockCmd(SPI_MASTER_CLK, ENABLE);
  
  GPIO_InitTypeDef GPIO_InitStructure;
  /* Configure SPI_MASTER pins: SCK and MOSI ---------------------------------*/
  /* Configure SCK and MOSI pins as Alternate Function Push Pull */
  GPIO_InitStructure.GPIO_Pin = SPI_MASTER_PIN_SCK | SPI_MASTER_PIN_MOSI;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(SPI_MASTER_GPIO, &GPIO_InitStructure);
  
  // pin cs
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = SPI_MASTER_PIN_CS;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(SPI_MASTER_GPIO,&GPIO_InitStructure);
}
