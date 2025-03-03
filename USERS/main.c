#include "../SYSTEM/sys/sys.h"
#include "../SYSTEM/usart/usart.h"
#include "../SYSTEM/delay/delay.h"

#include "../MIDDLEWARS/Drivers/LED/led.h"
#include "../MIDDLEWARS/Drivers/KEY/key.h"

int main(void)
{
    uint8_t key;
    
	HAL_Init();
	sys_stm32_clock_init(85, 2, 2, 4, 8);
    delay_init(170);
    
    led_init();
    key_init();
    
    while(1)
    {
        key = key_scan(0);  //获取键值
        
        if(key)
        {
            switch(key)
            {
                case KEY0_PRES:     // 控制LED翻转
                    LED0_TOGGLE();  // LED取反
                    break;
                case KEY1_PRES:     // 控制LED翻转
                    LED1_TOGGLE();  // LED取反
                    break;
                case KEY2_PRES:     // 控制LED翻转
                    LED0_TOGGLE();  // LED取反
                    LED1_TOGGLE();  // LED取反
                    break;
                default: break;
            }
        }
        else
        {
            delay_ms(10);
        }
    }
}
