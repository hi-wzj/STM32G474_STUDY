#include "../SYSTEM/sys/sys.h"
#include "../SYSTEM/usart/usart.h"
#include "../SYSTEM/delay/delay.h"

#include "../MIDDLEWARS/Drivers/LED/led.h"
#include "../MIDDLEWARS/Drivers/KEY/key.h"
#include "../MIDDLEWARS/Drivers/WWDG/wwdg.h"

int main(void)
{
   
	HAL_Init();
	sys_stm32_clock_init(85, 2, 2, 4, 8);
    delay_init(170);
    
    led_init();
    key_init();
    
    LED0(0);
    delay_ms(300);  // 延时 300ms,LED0 的变化"可见"
    wwdg_init(WWDG_PRESCALER_8, 0X5F, 0X7F); // 计数器值为 7f,窗口寄存器为 5f,8 分频
    
    while(1)
    {
        LED0(1);  
    }
}
