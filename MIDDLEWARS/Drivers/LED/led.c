#include "../MIDDLEWARS/Drivers/LED/led.h"

void led_init(void)
{
    /* 时钟使能 */
    LED0_GPIO_CLK_ENABLE();
    LED1_GPIO_CLK_ENABLE();

    /* 设置引脚 */
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = LED0_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(LED0_GPIO_PROT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LED1_GPIO_PIN;
    HAL_GPIO_Init(LED0_GPIO_PROT, &GPIO_InitStruct);

    LED0(1);
    LED1(1)
}
