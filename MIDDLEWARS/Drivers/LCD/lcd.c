#include "../MIDDLEWARS/Drivers/LCD/lcd.h"
#include "../MIDDLEWARS/Drivers/LCD/font.h"
#include "../MIDDLEWARS/Drivers/SPI/spi.h"
#include "../SYSTEM/delay/delay.h"

/* LCD缓存大小设置，修改此值时请注意！！！！修改这两个值时可能会影响以下函数 lcd_clear/lcd_fill/lcd_draw_line */
#define LCD_TOTAL_BUF_SIZE      (LCD_WIDTH * LCD_HEIGHT * 2)
#define LCD_BUF_SIZE            11520
static uint8_t lcd_buf[LCD_BUF_SIZE];

uint16_t g_piont_color = WHITE;
uint16_t g_black_color = WHITE;

static void lcd_gpio_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    LCD_PWR_GPIO_CLK_ENABLE();
    LCD_RST_GPIO_CLK_ENABLE();
    LCD_WR_GPIO_CLK_ENABLE();
    LCD_CS_GPIO_CLK_ENABLE();

    GPIO_InitStruct.Pin = LCD_PWR_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(LCD_PWR_GPIO_PORT, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = LCD_RST_GPIO_PIN;
    HAL_GPIO_Init(LCD_RST_GPIO_PORT, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = LCD_WR_GPIO_PIN;
    HAL_GPIO_Init(LCD_WR_GPIO_PORT, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = LCD_CS_GPIO_PIN;
    HAL_GPIO_Init(LCD_CS_GPIO_PORT, &GPIO_InitStruct);
    
    LCD_CS(0);
    LCD_PWR(0);
    
    LCD_RST(0);
    delay_ms(120);
    LCD_RST(1);
    
    spi1_init();
}

static void lcd_spi_send(uint8_t *data, uint32_t size)
{
    uint32_t i;
    uint32_t delta;
    
    LCD_CS(0);
    delta = size / 0xFFFF;
    for(i = 0; i <= delta; i++)
    {
        if(i == delta) // 最后一帧数据
            spi1_write_data(&data[i * 0xFFFF], size % 0xFFFF);
        else // 超长数据一次发送0xFFFF字节数据
            spi1_write_data(&data[i * 0xFFFF], 0xFFFF);
    }
    LCD_CS(1);
}

/* 写命令到LCD */
static void lcd_write_cmd(uint8_t cmd)
{
    LCD_WR(0);
    lcd_spi_send(&cmd, 1);
}

/* 写数据到LCD */
static void lcd_write_data(uint8_t data)
{
    LCD_WR(1);
    lcd_spi_send(&data, 1);
}

/* 写半个字的数据到LCD */
static void lcd_write_halfword(const uint16_t half_da)
{
    uint8_t data[2] = {0};
    
    data[0] = half_da >> 8;
    data[1] = half_da;
    
    LCD_WR(1);
    lcd_spi_send(data, 2);
}

/* 设置数据写入LCD缓存区域 */
static void lcd_address_set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    lcd_write_cmd(0x2a);
    lcd_write_data(x1 >> 8);
    lcd_write_data(x1);
    lcd_write_data(x2 >> 8);
    lcd_write_data(x2);
    
    lcd_write_cmd(0x2b);
    lcd_write_data(y1 >> 8);
    lcd_write_data(y1);
    lcd_write_data(y2 >> 8);
    lcd_write_data(y2);
    
    lcd_write_cmd(0x2c);
}

/* 打开LCD显示 */
void lcd_display_on(void)
{
    LCD_PWR(1);
}

/* 关闭LCD显示 */
void lcd_display_off(void)
{
    LCD_PWR(0);
}

/* 画点函数 */
void lcd_draw_point(uint16_t x, uint16_t y, uint16_t color)
{
    lcd_address_set(x, y, x ,y);
    lcd_write_halfword(color);
}

/* 显示一个字符 */
void lcd_show_char(uint16_t x, uint16_t y, char chr,
                    uint8_t size, uint8_t mode, uint8_t color)
{
    uint8_t temp = 0;
    uint8_t t = 0, t1 = 0;
    uint8_t *pfont = NULL;
    uint8_t c_size = 0;     // 得到字体一个字符对应的点阵集所占的字节数
    uint8_t color_temp = 0;
    uint8_t sta = 0;
    
    /* 得到偏移后的值
    （ASCII字库是从空格开始取模，所以减去‘ ’就是对应字符的字库）*/
    chr = chr - ' ';
    
    if((x > (LCD_WIDTH - size/2) )|| (y > (LCD_HEIGHT - size)))
        return;
    
    /* 表达式 (x,y,x+8-1,y+16-1) */
    lcd_address_set(x, y, x+size/2-1, y+size-1);
    switch(size)
    {
        case 12:
            pfont = (uint8_t *)asc2_1206[chr];
            break;
        case 16:
            pfont = (uint8_t *)asc2_1206[chr];
            break;
        case 24:
            pfont = (uint8_t *)asc2_1206[chr];
            break;
        case 32:
            pfont = (uint8_t *)asc2_1206[chr];
            break;
        default : return;
    }
    
    if(size != 24)
    {
        c_size = (size/8 + ((size%8) ? 1 : 0)) * (size/2);
        for(t = 0; t < c_size; t++)
        {
            temp = pfont[t];
            if(size == 12)
            {
                for(t1 = 0; t1 < 6; t1++)
                {
                    if(temp & 0x80)
                        color_temp = color;
                    else if(mode == 0)
                        color_temp = g_black_color;
                    
                    lcd_write_halfword(color_temp);
                    temp <<= 1;
                }
            }
            else if(size == 16 || size == 32)
            {
                for(t1 = 0; t1 < 8; t1++)
                {
                    if(temp & 0x80)
                        color_temp = color;
                    else if(mode == 0)  // // 无效点，不显示
                        color_temp = g_black_color;
                    
                    lcd_write_halfword(color_temp);
                    temp <<= 1;
                }
            }
        }
    }
    else
    {
        c_size = (size * 16) / 8;
        for(t = 0; t < c_size; t++)
        {
            temp = asc2_2412[chr][t];
            if((t % 2) == 0)
                sta = 8;
            else 
                sta = 4;
            
            for(t1 = 0; t1 < sta; t1++)
            {
                if(temp & 0x80)
                    color_temp = color;
                else if(mode == 0)      // 无效点，不显示
                    color_temp = g_black_color;
                
                lcd_write_halfword(color_temp);
                temp <<= 1;
            }
        }
    }
}

/* 以一种颜色清空LCD屏 */
void lcd_clear(uint16_t color)
{
    uint16_t i, j;
    uint8_t data[2] = {0};

    data[0] = color >> 8;
    data[1] = color;

    lcd_address_set(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);

    for(j = 0; j < LCD_BUF_SIZE / 2; j++)
    {
        lcd_buf[j * 2] =  data[0];
        lcd_buf[j * 2 + 1] =  data[1];
    }

    LCD_WR(1);

    for(i = 0; i < (LCD_TOTAL_BUF_SIZE / LCD_BUF_SIZE); i++)
    {
        lcd_spi_send(lcd_buf, LCD_BUF_SIZE);
    }
}

/* LCD初始化 */
void lcd_init(void)
{
    lcd_gpio_init();            /* 硬件接口初始化 */
	
    delay_ms(120);
    /* Sleep Out */
    lcd_write_cmd(0x11);
    
    /* wait for power stability */
    delay_ms(120);

    lcd_write_cmd(0x36); //MX, MY, RGB mode 
    if(USE_HORIZONTAL==0)lcd_write_data(0x00);
    else if(USE_HORIZONTAL==1)lcd_write_data(0xC0);
    else if(USE_HORIZONTAL==2)lcd_write_data(0x70);
    else lcd_write_data(0xA0);
    
    lcd_write_cmd(0x3A);     
    lcd_write_data(0x55);   

    lcd_write_cmd(0xB2);     
    lcd_write_data(0x1F);   
    lcd_write_data(0x1F);   
    lcd_write_data(0x00);   
    lcd_write_data(0x33);   
    lcd_write_data(0x33); 

    lcd_write_cmd(0xB7);    
    lcd_write_data(0x12);  //VGH=12.54V,VGL=-8.23V 

    lcd_write_cmd(0xBB);     
    lcd_write_data(0x35);  

    lcd_write_cmd(0xC0);     
    lcd_write_data(0x2C); 

    lcd_write_cmd(0xC2);     
    lcd_write_data(0x01);   

    lcd_write_cmd(0xC3);     
    lcd_write_data(0x15); //4.6V 

    lcd_write_cmd(0xC4);     
    lcd_write_data(0x20); //VDV, 0x20:0v

    lcd_write_cmd(0xC6);     
    lcd_write_data(0x13);     

    lcd_write_cmd(0xD0);     
    lcd_write_data(0xA4);   
    lcd_write_data(0xA1);   

    lcd_write_cmd(0xD6);     
    lcd_write_data(0xA1);   //sleep in后，gate输出为GND

    lcd_write_cmd(0xE0);
    lcd_write_data(0xF0);
    lcd_write_data(0x06);
    lcd_write_data(0x0D);
    lcd_write_data(0x0B);
    lcd_write_data(0x0A);
    lcd_write_data(0x07);
    lcd_write_data(0x2E);
    lcd_write_data(0x43);
    lcd_write_data(0x45);
    lcd_write_data(0x38);
    lcd_write_data(0x14);
    lcd_write_data(0x13);
    lcd_write_data(0x25);
    lcd_write_data(0x29);

    lcd_write_cmd(0xE1);
    lcd_write_data(0xF0);
    lcd_write_data(0x07);
    lcd_write_data(0x0A);
    lcd_write_data(0x08);
    lcd_write_data(0x07);
    lcd_write_data(0x23);
    lcd_write_data(0x2E);
    lcd_write_data(0x33);
    lcd_write_data(0x44);
    lcd_write_data(0x3A);
    lcd_write_data(0x16);
    lcd_write_data(0x17);
    lcd_write_data(0x26);
    lcd_write_data(0x2C);
        
    lcd_write_cmd(0xE4);    
    lcd_write_data(0x1D); //使用240根gate  (N+1)*8
    lcd_write_data(0x00); //设定gate起点位置
    lcd_write_data(0x00); //当gate没有用完时，bit4(TMG)设为0

    lcd_write_cmd(0x21);        

    lcd_write_cmd(0x2A);    //Column Address Set
    lcd_write_data(0x00);   
    lcd_write_data(0x00);   //0
    lcd_write_data(0x00);   
    lcd_write_data(0xEF);   //239

    lcd_write_cmd(0x2B);    //Row Address Set
    lcd_write_data(0x00);   
    lcd_write_data(0x00);   //0
    lcd_write_data(0x00);   
    lcd_write_data(0xEF);   //239

    lcd_write_cmd(0x2C);     

    lcd_write_cmd(0x29);    //Display on	
	
    lcd_clear(WHITE);

    LCD_PWR(1); // 打开显示
}

/* 显示字符串 */
void lcd_show_string(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t size, char *p, uint16_t color)
{
    uint8_t x0 = x;
    width += x;
    height += y;

    while ((*p <= '~') && (*p >= ' '))   /* 判断是不是非法字符! */
    {
        if (x >= width)
        {
            x = x0;
            y += size;
        }

        if (y >= height) break;  /* 退出 */

        lcd_show_char(x, y, *p, size, 0, color);
        x += size / 2;
        p++;
    }
}
