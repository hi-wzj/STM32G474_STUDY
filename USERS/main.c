#include "../SYSTEM/sys/sys.h"
#include "../SYSTEM/usart/usart.h"
#include "../SYSTEM/delay/delay.h"

#include "../MIDDLEWARS/Drivers/LED/led.h"
#include "../MIDDLEWARS/Drivers/KEY/key.h"
#include "../MIDDLEWARS/Drivers/IWDG/iwdg.h"

int main(void)
{
   
	HAL_Init();
	sys_stm32_clock_init(85, 2, 2, 4, 8);
    delay_init(170);
    
    led_init();
    key_init();
    
    delay_ms(100);  // 延时 100ms,LED0 的变化"可见"
    iwdg_init(IWDG_PRESCALER_64, 500);
    LED0(0);
    while(1)
    {
        if(key_scan(1) == KEY0_PRES)
            iwdg_feed();
        delay_ms(10);    
    }
}
