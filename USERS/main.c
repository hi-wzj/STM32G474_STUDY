#include "../SYSTEM/sys/sys.h"
#include "../SYSTEM/usart/usart.h"
#include "../SYSTEM/delay/delay.h"

#include "../MIDDLEWARS/Drivers/LED/led.h"

int main(void)
{
    uint8_t len;
    uint16_t times = 0;
    
	HAL_Init();
	sys_stm32_clock_init(85, 2, 2, 4, 8);
    delay_init(170);
    
    led_init();
    usart_init(115200);
    
    while(1)
    {
        if(g_usart_rx_sta & 0x8000)
        {
            len = g_usart_rx_sta & 0x3fff;
            printf("send msg:\n");
            
            HAL_UART_Transmit(&g_uart1_handle, (uint8_t *)g_usart_rx_buf, len, 1000);
            
            while(__HAL_UART_GET_FLAG(&g_uart1_handle, UART_FLAG_TC) != SET);
            
            printf("\n");
            g_usart_rx_sta = 0;
        }
        else
        {
            times++;
            if(times % 5000 == 0)
            {
                printf("wzj is god\n");
                printf("hello!!\n");
            }
            
            if(times % 200 == 0) printf("input data, enter over\n");
            if(times % 30 == 0) LED0_TOGGLE();
            delay_ms(10);
        }
            
    }
}
