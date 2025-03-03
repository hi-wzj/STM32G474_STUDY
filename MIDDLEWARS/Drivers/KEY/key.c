#include "../MIDDLEWARS/Drivers/KEY/key.h"

void key_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    KEY0_CLK_ENABLE();
    KEY1_CLK_ENABLE();
    KEY2_CLK_ENABLE();
    
    GPIO_InitStruct.Pin = KEY0_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(KEY0_GPIO_PROT, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = KEY1_GPIO_PIN;
    HAL_GPIO_Init(KEY1_GPIO_PROT, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = KEY2_GPIO_PIN;
    HAL_GPIO_Init(KEY2_GPIO_PROT, &GPIO_InitStruct);
}

uint8_t key_scan(uint8_t mode)
{
    static uint8_t key_up = 1;
    uint8_t key_value = 0;
    
    if(mode) key_up = 1;    // 是否支持连按
    
    if(key_up && (KEY0 == 0 || KEY1 == 0 || KEY2 == 0)) // 按键松开标志为1，且有任意一个按键按下
    {
        delay_ms(10); // 去抖动
        key_up = 0;
        
        if(KEY0 == 0) key_value = KEY0_PRES;
        if(KEY1 == 0) key_value = KEY1_PRES;
        if(KEY2 == 0) key_value = KEY2_PRES;
    }
    else if(KEY0 == 1 && KEY1 == 1 && KEY2 == 1) // 没有任何按键按下，标记按键松开
    {
        key_up = 1;
    }
    
    return key_value;
}