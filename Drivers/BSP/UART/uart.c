#include "./BSP/UART/uart.h"
#include "./BSP/PULSE/pulse.h"
#include "./BSP/TMC2209/tmc2209.h"


static uint8_t usb_uart_rx_buffer[BUFFER_SIZE_FOR_USART1] = {0};
static uint8_t uart4_rx_buffer[BUFFER_SIZE_FOR_UART4] = {0};
static UART_HandleTypeDef handle_for_half_duplex_uart = {0};
static UART_HandleTypeDef handle_for_uart = {0};
static UART_HandleTypeDef handle_for_full_duplex_uart = {0};

// 下面的块注释也很常见，以后写函数就这么写吧。下面的例子是详细说明方法
/**
 * @brief 半双工初始化外设
 * @param 波特率
 * @retval 没有返回，会填充指针指向的地址，之后用指针获得句柄就行。
 * @attention 使用pa2口
 */

void half_duplex_uart_init(uint32_t baudrate)
{

    handle_for_half_duplex_uart.Instance = HALF_DUPLEX_USART_UX;      /* USART_UX */
    handle_for_half_duplex_uart.Init.BaudRate = baudrate;             /* 波特率 */
    handle_for_half_duplex_uart.Init.WordLength = UART_WORDLENGTH_8B; /* 字长为8位数据格式 */
    handle_for_half_duplex_uart.Init.StopBits = UART_STOPBITS_1;      /* 一个停止位 */
    handle_for_half_duplex_uart.Init.Parity = UART_PARITY_NONE;       /* 无奇偶校验位 */
    handle_for_half_duplex_uart.Init.HwFlowCtl = UART_HWCONTROL_NONE; /* 无硬件流控 */
    handle_for_half_duplex_uart.Init.Mode = UART_MODE_TX_RX;          /* 收发模式 */
    HAL_HalfDuplex_Init(&handle_for_half_duplex_uart);                // 这个函数很好玩，我还从没见过这么玩的
    // 这个函数中有个必须要用到的函数官方没动，就是下面的HAL_UART_MspInit，没有默认模式，直接空的。
    // 这导致我要重定义这个必须的函数，如果定义了，就会和正点原子的代码冲突，不能引入其uart的头文件了

    /* 该函数会开启接收中断，每次接收一个数据，触发中断，cpu把数据从寄存器搬到缓冲区，这个缓冲区在hal里必须*/
    // 注意，这个中断不会回调，只有在缓冲区满了才会回调。

    HAL_UART_Receive_IT(&handle_for_half_duplex_uart, usb_uart_rx_buffer, BUFFER_SIZE_FOR_USART1);
}

// 下面的块注释也很常见，以后写函数就这么写吧。下面的例子是详细说明方法
/**
 * @brief 初始化外设
 * @param 波特率
 * @retval 没有返回，会填充指针指向的地址，之后用指针获得句柄就行。
 * @note 工作原理是：在stm32中有个寄存器来存储uart数据，但是只能存一个字节，所以每次存到一个字节，就会触发中断，
 * 然后cpu自动搬运数据到缓存区，等到一定条件，如缓冲区满了才执行回调函数，这里没有用dma，cpu自己搬运，
 * @todo 但是什么时候触发回调呢？
 * @attention 此函数会占用pa9,10
 */

void full_duplex_uart_init(uint32_t baudrate)
{

    handle_for_full_duplex_uart.Instance = FULL_DUPLEX_USART_UX;      /* USART_UX */
    handle_for_full_duplex_uart.Init.BaudRate = baudrate;             /* 波特率 */
    handle_for_full_duplex_uart.Init.WordLength = UART_WORDLENGTH_8B; /* 字长为8位数据格式 */
    handle_for_full_duplex_uart.Init.StopBits = UART_STOPBITS_1;      /* 一个停止位 */
    handle_for_full_duplex_uart.Init.Parity = UART_PARITY_NONE;       /* 无奇偶校验位 */
    handle_for_full_duplex_uart.Init.HwFlowCtl = UART_HWCONTROL_NONE; /* 无硬件流控 */
    handle_for_full_duplex_uart.Init.Mode = UART_MODE_TX_RX;          /* 收发模式 */
    HAL_UART_Init(&handle_for_full_duplex_uart);                      // 这个函数很好玩，我还从没见过这么玩的
    // 这个函数中有个必须要用到的函数官方没动，就是下面的HAL_UART_MspInit，甚至没有默认模式，直接空的，离谱，要我们自己写，而且，还不在注释里标出来
    // 这导致我要重定义这个必须的函数，如果定义了，就会和正点原子的代码冲突，不能引入其uart的头文件了

    /* 该函数会开启接收中断，每次接收一个数据，触发中断，cpu把数据从寄存器搬到缓冲区，这个缓冲区在hal里必须*/
    // 注意，这个中断不会回调，只有在缓冲区满了才会回调。

    HAL_UART_Receive_IT(&handle_for_full_duplex_uart, uart4_rx_buffer, BUFFER_SIZE_FOR_UART4);
}

void uart_init(uint32_t baudrate)
{

    handle_for_uart.Instance = USART_UX;                  /* USART_UX */
    handle_for_uart.Init.BaudRate = baudrate;             /* 波特率 */
    handle_for_uart.Init.WordLength = UART_WORDLENGTH_8B; /* 字长为8位数据格式 */
    handle_for_uart.Init.StopBits = UART_STOPBITS_1;      /* 一个停止位 */
    handle_for_uart.Init.Parity = UART_PARITY_NONE;       /* 无奇偶校验位 */
    handle_for_uart.Init.HwFlowCtl = UART_HWCONTROL_NONE; /* 无硬件流控 */
    handle_for_uart.Init.Mode = UART_MODE_TX_RX;          /* 收发模式 */
    HAL_UART_Init(&handle_for_uart);                      // 这个函数很好玩，我还从没见过这么玩的
    // 这个函数中有个必须要用到的函数官方没动，就是下面的HAL_UART_MspInit，甚至没有默认模式，直接空的，离谱，要我们自己写，而且，还不在注释里标出来
    // 这导致我要重定义这个必须的函数，如果定义了，就会和正点原子的代码冲突，不能引入其uart的头文件了

    /* 该函数会开启接收中断，每次接收一个数据，触发中断，cpu把数据从寄存器搬到缓冲区，这个缓冲区在hal里必须*/
    // 注意，这个中断不会回调，只有在缓冲区满了才会回调。

    HAL_UART_Receive_IT(&handle_for_uart, usb_uart_rx_buffer, BUFFER_SIZE_FOR_USART1);
}

// 共用函数区域begin///////////////////////////////////////////////////////////////////////////////

/**
 * @brief       就是这个函数，要自己重写。
 * @param       huart: UART句柄类型指针
 * @note        此函数会被HAL_UART_Init()调用
 *              完成时钟使能，引脚配置，中断配置
 * @retval      无
 */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef gpio_init_struct;

    if (huart->Instance == HALF_DUPLEX_USART_UX) /* 如果是串口1，进行串口1 MSP初始化 */
    {
        HALF_DUPLEX_USART_TX_GPIO_CLK_ENABLE(); /* 使能串口TX脚时钟 */
        HALF_DUPLEX_USART_UX_CLK_ENABLE();      /* 使能串口时钟 */

        gpio_init_struct.Pin = HALF_DUPLEX_USART_TX_GPIO_PIN; /* 串口发送引脚号 */
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;              /* 复用推挽输出 */
        gpio_init_struct.Pull = GPIO_PULLUP;                  /* 上拉 */
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;        /* IO速度设置为高速 */
        HAL_GPIO_Init(HALF_DUPLEX_USART_TX_GPIO_PORT, &gpio_init_struct);
    }

    if (huart->Instance == USART_UX)
    {
        USART_TX_GPIO_CLK_ENABLE(); /* 使能串口TX脚时钟 */
        USART_RX_GPIO_CLK_ENABLE(); /* 使能串口RX脚时钟 */
        USART_UX_CLK_ENABLE();      /* 使能串口时钟 */

        gpio_init_struct.Pin = USART_TX_GPIO_PIN;      /* 串口发送引脚号 */
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;       /* 复用推挽输出 */
        gpio_init_struct.Pull = GPIO_PULLUP;           /* 上拉 */
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH; /* IO速度设置为高速 */
        HAL_GPIO_Init(USART_TX_GPIO_PORT, &gpio_init_struct);

        gpio_init_struct.Pin = USART_RX_GPIO_PIN; /* 串口RX脚 模式设置 */
        gpio_init_struct.Mode = GPIO_MODE_AF_INPUT;
        HAL_GPIO_Init(USART_RX_GPIO_PORT, &gpio_init_struct); /* 串口RX脚 必须设置成输入模式 */

        HAL_NVIC_EnableIRQ(USART_UX_IRQn);         /* 使能USART1中断通道 */
        HAL_NVIC_SetPriority(USART_UX_IRQn, 3, 3); // 组2，最低优先级:抢占优先级3，子优先级3 *
    }

    if (huart->Instance == FULL_DUPLEX_USART_UX)
    {
        FULL_DUPLEX_USART_TX_GPIO_CLK_ENABLE(); /* 使能串口TX脚时钟 */
        FULL_DUPLEX_USART_RX_GPIO_CLK_ENABLE(); /* 使能串口RX脚时钟 */
        FULL_DUPLEX_USART_UX_CLK_ENABLE();      /* 使能串口时钟 */

        gpio_init_struct.Pin = FULL_DUPLEX_USART_TX_GPIO_PIN; /* 串口发送引脚号 */
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;              /* 复用推挽输出 */
        gpio_init_struct.Pull = GPIO_PULLUP;                  /* 上拉 */
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;        /* IO速度设置为高速 */
        HAL_GPIO_Init(FULL_DUPLEX_USART_TX_GPIO_PORT, &gpio_init_struct);

        gpio_init_struct.Pin = FULL_DUPLEX_USART_RX_GPIO_PIN; /* 串口RX脚 模式设置 */
        gpio_init_struct.Mode = GPIO_MODE_AF_INPUT;
        HAL_GPIO_Init(FULL_DUPLEX_USART_RX_GPIO_PORT, &gpio_init_struct); /* 串口RX脚 必须设置成输入模式 */

        HAL_NVIC_EnableIRQ(FULL_DUPLEX_USART_UX_IRQn);         /* 使能USART1中断通道 */
        HAL_NVIC_SetPriority(FULL_DUPLEX_USART_UX_IRQn, 3, 3); // 组2，最低优先级:抢占优先级3，子优先级3 *
    }
}
/**
 * @brief 处理来自电脑的通信函数
 * 
 */
void uart_reaction(void)
{
    switch (usb_uart_rx_buffer[0])
    {
    case 0x10://左转1cm，慢转
        tmc2209_full_write(0,0,dir_left_data); 
        pulse_period_set_us(10000);
        pulse_times_set(PULSE_TIMES_ONE_MM*10);
        break;
    case 0x01://右转1cm，慢转
        tmc2209_full_write(0,0,dir_right_data);
        pulse_period_set_us(10000);
        pulse_times_set(PULSE_TIMES_ONE_MM*10);    
        break;
    case 0x00://停止
        pulse_stop();
        break;
    case 0x02://高速左转动
        tmc2209_full_write(0,0,dir_left_data); 
        pulse_period_set_us(10000);
        pulse_times_set(100);
        pulse_period_set_us(5000);
        pulse_times_set(100);
        pulse_period_set_us(2500);
        pulse_times_set(PULSE_TIMES_ONE_MM*50);
        break;
    case 0x03://高速右转动
        tmc2209_full_write(0,0,dir_right_data); 
        pulse_period_set_us(10000);
        pulse_times_set(100);
        pulse_period_set_us(5000);
        pulse_times_set(100);
        pulse_period_set_us(2500);
        pulse_times_set(PULSE_TIMES_ONE_MM*50);  
        break;
    default:
        break;
    }
    
}


/**
 * @brief       中断回调函数
 * @param       handle:串口句柄
 * @retval      无
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *handle)
{
    if (handle->Instance == USART_UX)
    {   
        uart_reaction();
        HAL_UART_Transmit(handle, usb_uart_rx_buffer, BUFFER_SIZE_FOR_USART1, 1000);
        HAL_UART_Receive_IT(handle, usb_uart_rx_buffer, BUFFER_SIZE_FOR_USART1); // this function is necessary when processing data accomplished.
    }
    if (handle->Instance == FULL_DUPLEX_USART_UX)
    {
        // TODO 串口中断接收处理函数
        HAL_UART_Receive_IT(handle, usb_uart_rx_buffer, BUFFER_SIZE_FOR_UART4); // this function is necessary when processing data accomplished.
    }
}

// 共用函数区域end///////////////////////////////////////////////////////////////////////////////

// usb串口通信函数

void USART_UX_IRQHandler(void)
{

    HAL_UART_IRQHandler(&handle_for_uart); /* 调用HAL库中断处理函数 */
}

HAL_StatusTypeDef uart_transmit(uint8_t *pData, uint16_t Size, uint32_t Timeout)
{   

    return HAL_UART_Transmit(&handle_for_uart, pData, Size, Timeout);

}



// 全双工

HAL_StatusTypeDef full_duplex_uart_transmit(uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    return HAL_UART_Transmit(&handle_for_full_duplex_uart, pData, Size, Timeout);
}
HAL_StatusTypeDef full_duplex_uart_receive(uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    return HAL_UART_Receive(&handle_for_full_duplex_uart, pData, Size, Timeout);
}

/**
 * @brief   处理中断函数
 */
void FULL_DUPLEX_USART_UX_IRQHandler(void)
{

    HAL_UART_IRQHandler(&handle_for_full_duplex_uart); /* 调用HAL库中断处理公用函数 */
}

// 半双工通信函数
HAL_StatusTypeDef half_duplex_uart_transmit(uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    return HAL_UART_Transmit(&handle_for_half_duplex_uart, pData, Size, Timeout);
}

HAL_StatusTypeDef half_duplex_uart_receive(uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    return HAL_UART_Receive(&handle_for_half_duplex_uart, pData, Size, Timeout);
}
