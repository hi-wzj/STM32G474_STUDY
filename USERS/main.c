#include "../SYSTEM/sys/sys.h"
#include "../SYSTEM/usart/usart.h"
#include "../SYSTEM/delay/delay.h"

#include "../MIDDLEWARS/Drivers/LED/led.h"
#include "../MIDDLEWARS/Drivers/LCD/lcd.h"

int main(void)
{
    uint8_t x = 0;
    char show_buf[24];
    
	HAL_Init();
	sys_stm32_clock_init(85, 2, 2, 4, 8);
    delay_init(170);
    
    led_init();
    lcd_init();
    g_piont_color = RED;
    
    while(1)
    {
        
        sprintf(show_buf, "STM32G47%d_LCD", x);
        x++;
        if (x == 9)
            x = 0;
        lcd_show_string(40, 0, 240, 24, 24, show_buf, RED);
        LED0_TOGGLE(); /* LED0 闪烁 */
        delay_ms(1000);
    }
}
