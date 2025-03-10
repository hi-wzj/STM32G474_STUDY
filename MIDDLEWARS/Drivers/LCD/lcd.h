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

extern uint16_t g_point_color;  // 默认画笔颜色
extern uint16_t g_black_color;  // 默认背景颜色

/**
 * @brief       LCD初始化
 * @remark      无
 * @retval      无
 */
void lcd_init(void);

/**
 * @brief       以一种颜色清空LCD屏
 * @param       color 清屏颜色
 * @retval      无
 */
void lcd_clear(uint16_t color);

/**
 * @brief       画点函数
 * @param       x,y 画点坐标
 * @retval      无
 */
void lcd_draw_point(uint16_t x, uint16_t y,uint16_t color);

/**
 * @brief       显示字符串
 * @param       x,y         : 起始坐标
 * @param       width,height: 区域大小
 * @param       size        : 选择字体 12/16/24/32
 * @param       p           : 字符串首地址
 * @retval      无
 */
void lcd_show_string(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t size, char *p, uint16_t color);

/**
 * @brief       以一种颜色清空LCD屏
 * @param       x_start,y_start 起点坐标
 * @param       x_end,y_end 终点坐标
 * @param       color 填充颜色
 * @retval      无
 */
void lcd_fill(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, uint16_t color);

/**
 * @brief       画线函数(直线、斜线)
 * @param       x1,y1   起点坐标
 * @param       x2,y2   终点坐标
 * @retval      无
 */
void lcd_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color);

/**
 * @brief       画一个圆
 * @param       x0,y0   圆心坐标
 * @param       r   圆半径
 * @retval      无
 */
void lcd_draw_circle(uint16_t x0, uint16_t y0, uint8_t r,uint16_t color);

/**
 * @brief       画一个矩形
 * @param       x1,y1   起点坐标
 * @param       x2,y2   终点坐标
 * @retval      无
 */
void lcd_draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color);

/**
 * @brief       显示len个数字
 * @param       x,y : 起始坐标
 * @param       num : 数值(0 ~ 2^32)
 * @param       len : 显示数字的位数
 * @param       size: 选择字体 12/16/24/32
 * @retval      无
 */
void lcd_show_num(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size, uint16_t color);

/**
 * @brief       扩展显示len个数字(高位是0也显示)
 * @param       x,y : 起始坐标
 * @param       num : 数值(0 ~ 2^32)
 * @param       len : 显示数字的位数
 * @param       size: 选择字体 12/16/24/32
 * @param       mode: 显示模式
 *              [7]:0,不填充;1,填充0.
 *              [6:1]:保留
 *              [0]:0,非叠加显示;1,叠加显示.
 *
 * @retval      无
 */
void lcd_show_xnum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size, uint8_t mode, uint16_t color);
#endif /* __LCD_H__ */
