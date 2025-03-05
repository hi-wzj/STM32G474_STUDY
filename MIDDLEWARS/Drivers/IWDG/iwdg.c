#include "../MIDDLEWARS/Drivers/IWDG/iwdg.h"

static IWDG_HandleTypeDef IWDG_HandleStruct; 

void iwdg_init(uint8_t fprer, uint16_t rlr)
{
    IWDG_HandleStruct.Instance = IWDG;
    IWDG_HandleStruct.Init.Prescaler = fprer;    // 设置分频系数
    IWDG_HandleStruct.Init.Reload = rlr;        // 重装载值
    IWDG_HandleStruct.Init.Window = IWDG_WINDOW_DISABLE;    // 禁用窗口看门狗
    HAL_IWDG_Init(&IWDG_HandleStruct);
}

void iwdg_feed(void)
{
    HAL_IWDG_Refresh(&IWDG_HandleStruct);
}
