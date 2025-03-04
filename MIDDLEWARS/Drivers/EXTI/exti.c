#include "../MIDDLEWARS/Drivers/EXTI/exti.h"
#include "../MIDDLEWARS/Drivers/KEY/key.h"
#include "../MIDDLEWARS/Drivers/LED/led.h"
#include "../SYSTEM/delay/delay.h"

void extix_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    GPIO_InitStruct.Pin = KEY0_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(KEY0_GPIO_PROT, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = KEY1_GPIO_PIN;
    HAL_GPIO_Init(KEY1_GPIO_PROT, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = KEY2_GPIO_PIN;
    HAL_GPIO_Init(KEY2_GPIO_PROT, &GPIO_InitStruct);
    
    /* 设置中断优先级，抢占0，响应2*/
    HAL_NVIC_SetPriority(KEY_INT_IRQn, 0, 2);
    /* 使能中断线15_10 */
    HAL_NVIC_EnableIRQ(KEY_INT_IRQn);
}

void KEY_INT_IRQHandler(void)
{
    /* 调用中断处理公用函数，清除KEY0~2所在中断线的中断标志位 */
    HAL_GPIO_EXTI_IRQHandler(KEY0_GPIO_PIN);
    HAL_GPIO_EXTI_IRQHandler(KEY1_GPIO_PIN);
    HAL_GPIO_EXTI_IRQHandler(KEY2_GPIO_PIN);
    
    /* HAL库默认先清中断在处理回调函数，退出时再清一次中断，避免按键抖动误触发 */
    __HAL_GPIO_EXTI_CLEAR_IT(KEY0_GPIO_PIN);
    __HAL_GPIO_EXTI_CLEAR_IT(KEY1_GPIO_PIN);
    __HAL_GPIO_EXTI_CLEAR_IT(KEY2_GPIO_PIN);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    delay_ms(10);   // 消抖
    switch(GPIO_Pin)
    {
        case KEY0_GPIO_PIN:
            if(KEY0 == 0)
            {
                LED0_TOGGLE();
            }
            break;
        case KEY1_GPIO_PIN:
            if(KEY1 == 0)
            {
                LED1_TOGGLE();
            }
            break;
            
        case KEY2_GPIO_PIN:
            if(KEY2 == 0)
            {
                LED0_TOGGLE();
                LED1_TOGGLE();
            }
            break;
        default : break;
    }
}