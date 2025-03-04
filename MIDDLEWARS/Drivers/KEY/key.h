/* ../MIDDLEWARS/Drivers/KEY/key.h */
#ifndef __KEY_H__
#define __KEY_H__

#include "../SYSTEM/sys/sys.h"
#include "../SYSTEM/delay/delay.h"

/* 引脚定义 */
#define KEY0_GPIO_PROT  GPIOE
#define KEY0_GPIO_PIN   GPIO_PIN_12
#define KEY0_CLK_ENABLE()   do { __HAL_RCC_GPIOE_CLK_ENABLE(); } while(0)
#define KEY1_GPIO_PROT  GPIOE
#define KEY1_GPIO_PIN   GPIO_PIN_13
#define KEY1_CLK_ENABLE()   do { __HAL_RCC_GPIOE_CLK_ENABLE(); } while(0)
#define KEY2_GPIO_PROT  GPIOE
#define KEY2_GPIO_PIN   GPIO_PIN_14
#define KEY2_CLK_ENABLE()   do { __HAL_RCC_GPIOE_CLK_ENABLE(); } while(0)

/* 端口定义 */
#define KEY0 HAL_GPIO_ReadPin(KEY0_GPIO_PROT, KEY0_GPIO_PIN)
#define KEY1 HAL_GPIO_ReadPin(KEY1_GPIO_PROT, KEY1_GPIO_PIN)
#define KEY2 HAL_GPIO_ReadPin(KEY2_GPIO_PROT, KEY2_GPIO_PIN)

/* 按键动作定义 */
typedef enum {
    KEY0_PRES = 1,
    KEY1_PRES,
    KEY2_PRES
}KEY_ACTION;

void key_init(void);
uint8_t key_scan(uint8_t mode);

#endif /* __KEY_H__ */

