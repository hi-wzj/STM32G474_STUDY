#include "../SYSTEM/sys/sys.h"
#include "../SYSTEM/usart/usart.h"
#include "../SYSTEM/delay/delay.h"
#include "../MIDDLEWARS/USMART/usmart.h"

#include "../MIDDLEWARS/Drivers/LED/led.h"
#include "../MIDDLEWARS/Drivers/LCD/lcd.h"

void led_set(uint8_t sta)
{
    LED1(sta);
}

void test_fun(void(*ledset)(uint8_t), uint8_t sta)
{
    ledset(sta);
}

int main(void)
{
	HAL_Init();
	sys_stm32_clock_init(85, 2, 2, 4, 8);
    delay_init(170);
    usart_init(115200);
    usmart_dev.init(170);
    
    led_init();
    lcd_init();
    
    lcd_show_string(40, 40, 240, 24, 24, "wzj", RED);
    while(1)
    {
        LED0_TOGGLE(); /* LED0 闪烁 */
        delay_ms(1000);
    }
}
