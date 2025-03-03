#include "../SYSTEM/sys/sys.h"
#include "../SYSTEM/usart/usart.h"
#include "../SYSTEM/delay/delay.h"

void led_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    __HAL_RCC_GPIOE_CLK_ENABLE();
    
    gpio_init_struct.Pin    = GPIO_PIN_0;
    gpio_init_struct.Mode   = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull   = GPIO_PULLUP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOE, &gpio_init_struct);

    gpio_init_struct.Pin = GPIO_PIN_1;
    HAL_GPIO_Init(GPIOE, &gpio_init_struct);

    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_SET);
}

int main(void)
{
	HAL_Init();
	sys_stm32_clock_init(85, 2, 2, 4, 8);
    delay_init(170);
    led_init();
    
    while(1)
    {
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_SET);
        delay_ms(1000);
        
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET);
        delay_ms(1000);
    }
}
