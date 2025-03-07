/* ../MIDDLEWARS/Drivers/SPI/spi.h */
#ifndef __SPI_H__
#define __SPI_H__

#include "../SYSTEM/sys/sys.h"

/* SPI1 相关定义 */
#define SPI1_SCK_GPIO_PORT  GPIOB
#define SPI1_SCK_GPIO_PIN   GPIO_PIN_3
#define SPI1_SCK_GPIO_AF         GPIO_AF5_SPI1
#define SPI1_SCK_GPIO_CLK_ENABLE()  do { __HAL_RCC_GPIOB_CLK_ENABLE(); } while(0)

#define SPI1_MOSI_GPIO_PORT  GPIOB
#define SPI1_MOSI_GPIO_PIN   GPIO_PIN_5
#define SPI1_MOSI_GPIO_AF         GPIO_AF5_SPI1
#define SPI1_MOSI_GPIO_CLK_ENABLE()  do { __HAL_RCC_GPIOB_CLK_ENABLE(); } while(0)

#define SPI1_SPI                SPI1
#define SPI1_SPI_CLK_ENABLE()   do { __HAL_RCC_SPI1_CLK_ENABLE(); } while(0)

void spi1_init(void);
void spi1_write_data(uint8_t *tx_data,uint16_t size);

#endif /* __SPI_H__ */
