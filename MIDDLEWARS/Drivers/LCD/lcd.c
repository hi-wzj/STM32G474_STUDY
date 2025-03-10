#include "../MIDDLEWARS/Drivers/LCD/lcd.h"
#include "../MIDDLEWARS/Drivers/LCD/font.h"
#include "../MIDDLEWARS/Drivers/SPI/spi.h"
#include "../SYSTEM/delay/delay.h"

/* LCD缓存大小设置，修改此值时请注意！！！！修改这两个值时可能会影响以下函数 lcd_clear/lcd_fill/lcd_draw_line */
#define LCD_TOTAL_BUF_SIZE      (LCD_WIDTH * LCD_HEIGHT * 2)
#define LCD_BUF_SIZE            11520

static uint8_t lcd_buf[LCD_BUF_SIZE];

uint16_t g_point_color = WHITE;
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

static void lcd_write_cmd(uint8_t cmd)
{
    LCD_WR(0);
    lcd_spi_send(&cmd, 1);
}

static void lcd_write_data(uint8_t data)
{
    LCD_WR(1);
    lcd_spi_send(&data, 1);
}

static void lcd_write_halfword(const uint16_t half_da)
{
    uint8_t data[2] = {0};
    
    data[0] = half_da >> 8;
    data[1] = half_da;
    
    LCD_WR(1);
    lcd_spi_send(data, 2);
}

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

/**
 * @brief       m^n函数
 * @param       m,n     输入参数
 * @retval      m^n次方
 */
static uint32_t lcd_pow(uint8_t m, uint8_t n)
{
    uint32_t result = 1;

    while(n--)result *= m;

    return result;
}

void lcd_display_on(void)
{
    LCD_PWR(1);
}

void lcd_display_off(void)
{
    LCD_PWR(0);
}

void lcd_draw_point(uint16_t x, uint16_t y, uint16_t color)
{
    lcd_address_set(x, y, x ,y);
    lcd_write_halfword(color);
}

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

void lcd_fill(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, uint16_t color)
{
    uint16_t i = 0;
    uint32_t size = 0, size_remain = 0;

    size = (x_end - x_start + 1) * (y_end - y_start + 1) * 2;

    if(size > LCD_BUF_SIZE)
    {
        size_remain = size - LCD_BUF_SIZE;
        size = LCD_BUF_SIZE;
    }

    lcd_address_set(x_start, y_start, x_end, y_end);

    while(1)
    {
        for(i = 0; i < size / 2; i++)
        {
            lcd_buf[2 * i] = color >> 8;
            lcd_buf[2 * i + 1] = color;
        }

        LCD_WR(1);
        lcd_spi_send(lcd_buf, size);

        if(size_remain == 0)
            break;

        if(size_remain > LCD_BUF_SIZE)
        {
            size_remain = size_remain - LCD_BUF_SIZE;
        }

        else
        {
            size = size_remain;
            size_remain = 0;
        }
    }
}

void lcd_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color)
{
    uint16_t t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, row, col;
    uint32_t i = 0;

    if (y1 == y2)
    {
        /*快速画水平线*/
        lcd_address_set(x1, y1, x2, y2);

        for (i = 0; i < x2 - x1; i++)
        {
            lcd_buf[2 * i] = g_point_color >> 8;
            lcd_buf[2 * i + 1] = g_point_color;
        }

        LCD_WR(1);
        lcd_spi_send(lcd_buf, (x2 - x1) * 2);
        return;
    }

    delta_x = x2 - x1;
    delta_y = y2 - y1;
    row = x1;
    col = y1;

    if (delta_x > 0)
    {
        incx = 1;
    }
    else if (delta_x == 0)
    {
        incx = 0;
    }
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }

    if (delta_y > 0)
    {
        incy = 1;
    }
    else if (delta_y == 0)
    {
        incy = 0;
    }
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }

    if (delta_x > delta_y)
    {
        distance = delta_x;
    }
    else
    {
        distance = delta_y;
    }

    for (t = 0; t <= distance + 1; t++)
    {
        lcd_draw_point(row, col,color);
        xerr += delta_x ;
        yerr += delta_y ;

        if (xerr > distance)
        {
            xerr -= distance;
            row += incx;
        }

        if (yerr > distance)
        {
            yerr -= distance;
            col += incy;
        }
    }
}

void lcd_draw_circle(uint16_t x0, uint16_t y0, uint8_t r,uint16_t color)
{
    int a, b;
    int di;
    a = 0;
    b = r;
    di = 3 - (r << 1);

    while (a <= b)
    {
        lcd_draw_point(x0 - b, y0 - a, color);
        lcd_draw_point(x0 + b, y0 - a, color);
        lcd_draw_point(x0 - a, y0 + b, color);
        lcd_draw_point(x0 - b, y0 - a, color);
        lcd_draw_point(x0 - a, y0 - b, color);
        lcd_draw_point(x0 + b, y0 + a, color);
        lcd_draw_point(x0 + a, y0 - b, color);
        lcd_draw_point(x0 + a, y0 + b, color);
        lcd_draw_point(x0 - b, y0 + a, color);
        a++;

        if (di < 0)
        {
            di += 4 * a + 6;
        }
        else
        {
            di += 10 + 4 * (a - b);
            b--;
        }

        lcd_draw_point(x0 + a, y0 + b, color);
    }
}

void lcd_draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color)
{
    lcd_draw_line(x1, y1, x2, y1,color);
    lcd_draw_line(x1, y1, x1, y2,color);
    lcd_draw_line(x1, y2, x2, y2,color);
    lcd_draw_line(x2, y1, x2, y2,color);
}

void lcd_show_num(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size, uint16_t color)
{
    uint8_t t, temp;
    uint8_t enshow = 0;

    for (t = 0; t < len; t++)   /* 按总显示位数循环 */
    {
        temp = (num / lcd_pow(10, len - t - 1)) % 10;   /* 获取对应位的数字 */

        if (enshow == 0 && t < (len - 1))   /* 没有使能显示,且还有位要显示 */
        {
            if (temp == 0)
            {
                lcd_show_char(x + (size / 2)*t, y, ' ', size, 0, color);/* 显示空格,占位 */
                continue;   /* 继续下个一位 */
            }
            else
            {
                enshow = 1; /* 使能显示 */
            }

        }

        lcd_show_char(x + (size / 2)*t, y, temp + '0', size, 0, color); /* 显示字符 */
    }
}

void lcd_show_xnum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size, uint8_t mode, uint16_t color)
{
    uint8_t t, temp;
    uint8_t enshow = 0;

    for (t = 0; t < len; t++)   /* 按总显示位数循环 */
    {
        temp = (num / lcd_pow(10, len - t - 1)) % 10;    /* 获取对应位的数字 */

        if (enshow == 0 && t < (len - 1))   /* 没有使能显示,且还有位要显示 */
        {
            if (temp == 0)
            {
                if (mode & 0X80)   /* 高位需要填充0 */
                {
                    lcd_show_char(x + (size / 2)*t, y, '0', size, mode & 0X01, color);  /* 用0占位 */
                }
                else
                {
                    lcd_show_char(x + (size / 2)*t, y, ' ', size, mode & 0X01, color);  /* 用空格占位 */
                }

                continue;
            }
            else
            {
                enshow = 1; /* 使能显示 */
            }

        }

        lcd_show_char(x + (size / 2)*t, y, temp + '0', size, mode & 0X01, color);
    }
}
