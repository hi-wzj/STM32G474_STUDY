#include "../MIDDLEWARS/Drivers/SPI/spi.h"

static SPI_HandleTypeDef SPI_HandleStruct;

uint8_t spi1_read_write_byte(uint8_t txdata)
{
    uint8_t rxdata;
    HAL_SPI_TransmitReceive(&SPI_HandleStruct, &txdata, &rxdata, 1, 1000);
    return rxdata;
}

void spi1_init(void)
{
    SPI1_SPI_CLK_ENABLE();
    
    SPI_HandleStruct.Instance               = SPI1_SPI;
    SPI_HandleStruct.Init.Mode              = SPI_MODE_MASTER;              // 设置位主模式
    SPI_HandleStruct.Init.Direction         = SPI_DIRECTION_2LINES;         // SPI设置位双线模式
    SPI_HandleStruct.Init.DataSize          = SPI_DATASIZE_8BIT;            // SPI发送接收8位帧结构
    SPI_HandleStruct.Init.CLKPolarity       = SPI_POLARITY_HIGH;            // 串行同步时钟的空闲状态为高电平
    SPI_HandleStruct.Init.CLKPhase          = SPI_PHASE_2EDGE;              // 串行同步时钟由第二个跳变沿数据被采样
    SPI_HandleStruct.Init.NSS               = SPI_NSS_SOFT;                 // 内部NSS信号由软件（SSI位）控制
    SPI_HandleStruct.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;      // 波特率预分频值为4
    SPI_HandleStruct.Init.FirstBit          = SPI_FIRSTBIT_MSB;             // 数据传输从MSB开始
    SPI_HandleStruct.Init.TIMode            = SPI_TIMODE_DISABLE;           // 关闭TI模式
    SPI_HandleStruct.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;   // 关闭硬件CRC校验
    SPI_HandleStruct.Init.CRCPolynomial     = 10;                           // CRC值计算的多项式
    HAL_SPI_Init(&SPI_HandleStruct);
    
    __HAL_SPI_ENABLE(&SPI_HandleStruct);
    
    /* 启动传输, 实际上就是产生 8 个时钟脉冲, 达到清空 DR 的作用, 非必需 */
    spi1_read_write_byte(0xFF);
}

/* 弱函数
    SPI底层驱动，时钟使能，引脚配置 */
void HAL_SPI_MspInit(SPI_HandleTypeDef *Hspi)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    if(Hspi->Instance == SPI1_SPI)
    {
        SPI1_SCK_GPIO_CLK_ENABLE();
        SPI1_MOSI_GPIO_CLK_ENABLE();
        
        GPIO_InitStruct.Pin         = SPI1_SCK_GPIO_PIN;
        GPIO_InitStruct.Mode        = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull        = GPIO_PULLUP;
        GPIO_InitStruct.Speed       = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate   = SPI1_SCK_GPIO_AF;
        HAL_GPIO_Init(SPI1_SCK_GPIO_PORT, &GPIO_InitStruct);
        
        GPIO_InitStruct.Pin         = SPI1_MOSI_GPIO_PIN;
        GPIO_InitStruct.Alternate   = SPI1_MOSI_GPIO_AF;
        HAL_GPIO_Init(SPI1_MOSI_GPIO_PORT, &GPIO_InitStruct);
    }
}

/* SPI速度设置函数 */
void spi1_set_speed(uint8_t speed)
{
    assert_param(IS_SPI_BAUDRATE(speed));           // 判断有效性
    __HAL_SPI_DISABLE(&SPI_HandleStruct);           // 关闭SPI
    SPI_HandleStruct.Instance->CR1 &= 0xFFC7;       // 位3-5清零，用来设置波特率
    SPI_HandleStruct.Instance->CR1 |= speed << 3;   // 设置速度
    __HAL_SPI_ENABLE(&SPI_HandleStruct);            // 使能SPI
}

void spi1_write_data(uint8_t *tx_data,uint16_t size)
{
    HAL_SPI_Transmit(&SPI_HandleStruct, tx_data, size, HAL_MAX_DELAY);
}