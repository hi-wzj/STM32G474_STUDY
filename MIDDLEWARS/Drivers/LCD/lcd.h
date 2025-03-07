/* ../MIDDLEWARS/Drivers/LCD/lcd.h */
#ifndef __LCD_H__
#define __LCD_H__

#include "../SYSTEM/sys/sys.h"

#define USE_HORIZONTAL 0

/* LCD引脚定义 */
#define LCD_PWR_GPIO_PORT   GPIOD
#define LCD_PWR_GPIO_PIN    GPIO_PIN_8
#define LCD_PWR_GPIO_CLK_ENABLE()   do { __HAL_RCC_GPIOD_CLK_ENABLE(); } while(0)

#define LCD_RST_GPIO_PORT   GPIOD
#define LCD_RST_GPIO_PIN    GPIO_PIN_9
#define LCD_RST_GPIO_CLK_ENABLE()   do { __HAL_RCC_GPIOD_CLK_ENABLE(); } while(0)

#define LCD_WR_GPIO_PORT   GPIOB
#define LCD_WR_GPIO_PIN    GPIO_PIN_4
#define LCD_WR_GPIO_CLK_ENABLE()   do { __HAL_RCC_GPIOB_CLK_ENABLE(); } while(0)

#define LCD_CS_GPIO_PORT   GPIOD
#define LCD_CS_GPIO_PIN    GPIO_PIN_10
#define LCD_CS_GPIO_CLK_ENABLE()   do { __HAL_RCC_GPIOD_CLK_ENABLE(); } while(0)

#define LCD_PWR(x)  do { x ? \
                        (HAL_GPIO_WritePin(LCD_PWR_GPIO_PORT, \
                                            LCD_PWR_GPIO_PIN, \
                                            GPIO_PIN_SET)) :  \
                        (HAL_GPIO_WritePin(LCD_PWR_GPIO_PORT, \
                                            LCD_PWR_GPIO_PIN, \
                                            GPIO_PIN_RESET)); \
                          } while(0)

#define LCD_RST(x)  do { x ? \
                        (HAL_GPIO_WritePin(LCD_RST_GPIO_PORT, \
                                            LCD_RST_GPIO_PIN, \
                                            GPIO_PIN_SET)) :  \
                        (HAL_GPIO_WritePin(LCD_RST_GPIO_PORT, \
                                            LCD_RST_GPIO_PIN, \
                                            GPIO_PIN_RESET)); \
                          } while(0)

#define LCD_WR(x)  do { x ? \
                        (HAL_GPIO_WritePin(LCD_WR_GPIO_PORT, \
                                            LCD_WR_GPIO_PIN, \
                                            GPIO_PIN_SET)) : \
                        (HAL_GPIO_WritePin(LCD_WR_GPIO_PORT, \
                                            LCD_WR_GPIO_PIN, \
                                            GPIO_PIN_RESET));\
                          } while(0)

#define LCD_CS(x)  do { x ? \
                        (HAL_GPIO_WritePin(LCD_CS_GPIO_PORT, \
                                            LCD_CS_GPIO_PIN, \
                                            GPIO_PIN_SET)) : \
                        (HAL_GPIO_WritePin(LCD_CS_GPIO_PORT, \
                                            LCD_CS_GPIO_PIN, \
                                            GPIO_PIN_RESET));\
                          } while(0)                         

/* LCD 的宽和高定义 */
#define LCD_WIDTH   240
#define LCD_HEIGHT  240
                          
/* 常用画笔颜色 */
#define WHITE   0XFFFF  // 白色
#define BLACK   0X0000  // 黑色
#define RED     0XF800  // 红色
#define BLUE    0X001F  // 蓝色
#define YELLOW  0XFFE0  // 黄色 = GREEN + RED
#define GBLUE   0X07FF  // 青色 = GREEN + BLUE
#define MAGENTA 0XF81F  // 紫红色 = BLUE + RED
#define GREEN   0X07E0  // 绿色
#define CYAN    0X7FFF  // 青蓝色
                          
/* 不常用画笔颜色 */                          
#define BROWN   0XBC40  // 棕色
#define BRRED   0XFC07  // 棕红色
#define GRAY    0X8430  // 灰色
#define DARKBLUE   0X01CF  // 深蓝色
#define LIGHTBLUE  0X7D7C  // 浅蓝色
#define GRAYBLUE   0X5458  // 灰蓝色                           

#define LIGHTGREEN  0X841F  // 浅绿色
#define LGRAY       0XC618  // 浅灰色-窗体背景色
#define LGRAYBLUE   0XA651  // 浅灰蓝色-中间层颜色
#define LBBLUE      0X2B12  // 浅棕蓝色 -选择条目的反色

extern uint16_t g_piont_color;  // 默认画笔颜色
extern uint16_t g_black_color;  // 默认背景颜色

void lcd_init(void);
void lcd_clear(uint16_t color);
void lcd_show_string(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t size, char *p, uint16_t color);
#endif /* __LCD_H__ */