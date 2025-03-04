#include "../SYSTEM/sys/sys.h"
#include "../SYSTEM/usart/usart.h"
#include "../SYSTEM/delay/delay.h"

#include "../MIDDLEWARS/Drivers/LED/led.h"
#include "../MIDDLEWARS/Drivers/KEY/key.h"
#include "../MIDDLEWARS/Drivers/EXTI/exti.h"

/* 通过外部中断，就不需要扫描按键GPIO的输入，由中断事件来触发
    这样实时性又高，代码量又少 */
int main(void)
{
    uint8_t key;
    
	HAL_Init();
	sys_stm32_clock_init(85, 2, 2, 4, 8);
    delay_init(170);
    
    led_init();
    key_init(); // 要比extix_init先初始化
    extix_init();
    
    while(1)
    {
        delay_ms(1000);
    }
}
