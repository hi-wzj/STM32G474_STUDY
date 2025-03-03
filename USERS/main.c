#include "../SYSTEM/sys/sys.h"
#include "../SYSTEM/usart/usart.h"
#include "../SYSTEM/delay/delay.h"

#include "../MIDDLEWARS/Drivers/LED/led.h"

int main(void)
{
	HAL_Init();
	sys_stm32_clock_init(85, 2, 2, 4, 8);
    delay_init(170);
    led_init();
    
    while(1)
    {
        LED0(0);
        LED1(1);
        delay_ms(500);
        
        LED0(1);
        LED1(0);
        delay_ms(500);
    }
}
