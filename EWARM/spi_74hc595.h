

#define SPI_MASTER                    SPI2
#define SPI_MASTER_CLK                RCC_APB1Periph_SPI2
#define SPI_MASTER_GPIO               GPIOB
#define SPI_MASTER_GPIO_CLK           RCC_APB2Periph_GPIOB 
#define SPI_MASTER_PIN_SCK            GPIO_Pin_13
#define SPI_MASTER_PIN_MISO           GPIO_Pin_14
#define SPI_MASTER_PIN_MOSI           GPIO_Pin_15 
#define SPI_MASTER_PIN_CS             GPIO_Pin_12 

#define SPI_MASTER_IRQn               SPI2_IRQn

void init_spi2();
void init_gpio_spi();
void pin_cs_high();
void pin_cs_low();