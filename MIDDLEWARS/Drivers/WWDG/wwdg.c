#include "../MIDDLEWARS/Drivers/WWDG/wwdg.h"
#include "../MIDDLEWARS/Drivers/LED/led.h"

static WWDG_HandleTypeDef WWDG_HandleStruct; 

void wwdg_init(uint32_t fprer, uint8_t wr, uint8_t tr)
{
    WWDG_HandleStruct.Instance = WWDG;
    WWDG_HandleStruct.Init.Prescaler = fprer;   // 设置分频系数
    WWDG_HandleStruct.Init.Window = wr;         // 设置窗口值
    WWDG_HandleStruct.Init.Counter = tr;        // 设置计数器值
    WWDG_HandleStruct.Init.EWIMode = WWDG_EWI_ENABLE;   // 使能窗口看门狗提前唤醒中断
    HAL_WWDG_Init(&WWDG_HandleStruct);
}

/* HAL弱函数 */
void HAL_WWDG_MspInit(WWDG_HandleTypeDef *Hwwdg)
{
    WWDG_CLK_ENABLE();
    HAL_NVIC_SetPriority(WWDG_IRQn, 2, 3);
    HAL_NVIC_EnableIRQ(WWDG_IRQn);
}

void WWDG_IRQHandler(void)
{
    HAL_WWDG_IRQHandler(&WWDG_HandleStruct);
}

void HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef *Hwwdg)
{
    HAL_WWDG_Refresh(&WWDG_HandleStruct);
    LED1_TOGGLE();
}