#include "./BSP/UART/half_duplex_uart.h"


static uint8_t rx_buffer[BUFFER_SIZE_FOR_USART1] = {0};
static UART_HandleTypeDef handle_for_usart1 = {0};


//下面的块注释也很常见，以后写函数就这么写吧。下面的例子是详细说明方法
/**
 * @brief 初始化外设
 * @param 波特率
 * @retval 没有返回，会填充指针指向的地址，之后用指针获得句柄就行。
 * @note 工作原理是：在stm32中有个寄存器来存储uart数据，但是只能存一个字节，所以每次存到一个字节，就会触发中断，
 * 然后cpu自动搬运数据到缓存区，等到一定条件，如缓冲区满了才执行回调函数，这里没有用dma，cpu自己搬运，
 * @todo 但是什么时候触发回调呢？
 * @attention 此函数会占用pa9,10
 */

void half_duplex_uart_init(uint32_t baudrate)
{
    
    handle_for_usart1.Instance = HALF_DUPLEX_USART_UX;                                       /* USART_UX */
    handle_for_usart1.Init.BaudRate = baudrate;                                  /* 波特率 */
    handle_for_usart1.Init.WordLength = UART_WORDLENGTH_8B;                      /* 字长为8位数据格式 */
    handle_for_usart1.Init.StopBits = UART_STOPBITS_1;                           /* 一个停止位 */
    handle_for_usart1.Init.Parity = UART_PARITY_NONE;                            /* 无奇偶校验位 */
    handle_for_usart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;                      /* 无硬件流控 */
    handle_for_usart1.Init.Mode = UART_MODE_TX_RX;                               /* 收发模式 */
    HAL_HalfDuplex_Init(&handle_for_usart1);                                           //这个函数很好玩，我还从没见过这么玩的
    //这个函数中有个必须要用到的函数官方没动，就是下面的HAL_UART_MspInit，没有默认模式，直接空的。
    //这导致我要重定义这个必须的函数，如果定义了，就会和正点原子的代码冲突，不能引入其uart的头文件了

    /* 该函数会开启接收中断，每次接收一个数据，触发中断，cpu把数据从寄存器搬到缓冲区，这个缓冲区在hal里必须*/
    //注意，这个中断不会回调，只有在缓冲区满了才会回调。

    HAL_UART_Receive_IT(&handle_for_usart1,rx_buffer, BUFFER_SIZE_FOR_USART1); 
    
}


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

    if (huart->Instance == HALF_DUPLEX_USART_UX)                            /* 如果是串口1，进行串口1 MSP初始化 */
    {
        USART_TX_GPIO_CLK_ENABLE();                             /* 使能串口TX脚时钟 */
        USART_RX_GPIO_CLK_ENABLE();                             /* 使能串口RX脚时钟 */
        USART_UX_CLK_ENABLE();                                  /* 使能串口时钟 */

        gpio_init_struct.Pin = USART_TX_GPIO_PIN;               /* 串口发送引脚号 */
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;                /* 复用推挽输出 */
        gpio_init_struct.Pull = GPIO_PULLUP;                    /* 上拉 */
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* IO速度设置为高速 */
        HAL_GPIO_Init(USART_TX_GPIO_PORT, &gpio_init_struct);
        
        HAL_NVIC_EnableIRQ(HALF_DUPLEX_USART_UX_IRQn);                      /* 使能USART1中断通道 */
        HAL_NVIC_SetPriority(HALF_DUPLEX_USART_UX_IRQn, 3, 3);              // 组2，最低优先级:抢占优先级3，子优先级3 *
    }
}



/**
 * @brief 把接收到的信号发送回去
 * 
 */
void send_back(UART_HandleTypeDef *handle, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    /**
  * @brief  Sends an amount of data in blocking mode.来自官方库的注释
  * @note   When UART parity is not enabled (PCE = 0), and Word Length is configured to 9 bits (M1-M0 = 01),
  *         the sent data is handled as a set of u16. In this case, Size must indicate the number
  *         of u16 provided through pData.//这个是说，如果每个数据位有9个有效bit
  * //这得了解一些uart的工作方式，转到stm32学习日记
  * //总之，我们不用管
  * @param  huart Pointer to a UART_HandleTypeDef structure that contains
  *               the configuration information for the specified UART module.
  * @param  pData Pointer to data buffer (u8 or u16 data elements).
  * @param  Size  Amount of data elements (u8 or u16) to be sent
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
   HAL_StatusTypeDef status =  HAL_UART_Transmit(handle, pData, Size,Timeout);
}


/**
 * @brief       
 * @param       handle:串口句柄
 * @retval      无
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *handle)
{

    send_back(handle,rx_buffer,BUFFER_SIZE_FOR_USART1,1000);
    
    HAL_UART_Receive_IT(handle,rx_buffer, BUFFER_SIZE_FOR_USART1);// this function is necessary when processing data accomplished.
}


void USART_UX_IRQHandler(void)
{

    HAL_UART_IRQHandler(&handle_for_usart1);   /* 调用HAL库中断处理公用函数 */
    
}
