#ifndef __LED_H__
#define __LED_H__

#include "../SYSTEM/sys/sys.h"

/* LED引脚定义 */
#define LED0_GPIO_PROT  GPIOE
#define LED0_GPIO_PIN   GPIO_PIN_0
#define LED0_GPIO_CLK_ENABLE() do { __HAL_RCC_GPIOE_CLK_ENABLE(); } while (0);
#define LED1_GPIO_PROT  GPIOE
#define LED1_GPIO_PIN   GPIO_PIN_1
#define LED1_GPIO_CLK_ENABLE() do { __HAL_RCC_GPIOE_CLK_ENABLE(); } while (0);

/* LED端口定义 */
#define LED0(x) do { x ? \
                  HAL_GPIO_WritePin(LED0_GPIO_PROT, LED0_GPIO_PIN, GPIO_PIN_SET) : \
                  HAL_GPIO_WritePin(LED0_GPIO_PROT, LED0_GPIO_PIN, GPIO_PIN_RESET); } while (0);
#define LED1(x) do { x ? \
                HAL_GPIO_WritePin(LED1_GPIO_PROT, LED1_GPIO_PIN, GPIO_PIN_SET) : \
                HAL_GPIO_WritePin(LED1_GPIO_PROT, LED1_GPIO_PIN, GPIO_PIN_RESET); } while (0);

/* LED取反定义 */
#define LED0_TOGGLE() do { HAL_GPIO_TogglePin(LED0_GPIO_PROT, LED0_GPIO_PIN); } while (0);
#define LED1_TOGGLE() do { HAL_GPIO_TogglePin(LED1_GPIO_PROT, LED1_GPIO_PIN); } while (0);

void led_init(void);

#endif /* __LED_H__ */
