#pragma once//防止重复include一个文件导致问题，每个h文件都要有，有些编译器不支持，换就行了

#include "stdio.h"
#include "./SYSTEM/sys/sys.h"



//全双工
#define USART_TX_GPIO_PORT                  GPIOA
#define USART_TX_GPIO_PIN                   GPIO_PIN_9
#define USART_TX_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PA口时钟使能 */

#define USART_RX_GPIO_PORT                  GPIOA
#define USART_RX_GPIO_PIN                   GPIO_PIN_10
#define USART_RX_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PA口时钟使能 */

#define USART_UX                            USART1
#define USART_UX_IRQn                       USART1_IRQn
#define USART_UX_IRQHandler                 USART1_IRQHandler
#define USART_UX_CLK_ENABLE()               do{ __HAL_RCC_USART1_CLK_ENABLE(); }while(0)  /* USART1 时钟使能 */



//半双工配置
#define HALF_DUPLEX_USART_TX_GPIO_PORT                  GPIOB
#define HALF_DUPLEX_USART_TX_GPIO_PIN                   GPIO_PIN_10
#define HALF_DUPLEX_USART_TX_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PA口时钟使能 */

#define HALF_DUPLEX_USART_UX                            USART3
#define HALF_DUPLEX_USART_UX_IRQn                       USART3_IRQn
#define HALF_DUPLEX_USART_UX_IRQHandler                 USART3_IRQHandler
#define HALF_DUPLEX_USART_UX_CLK_ENABLE()               do{ __HAL_RCC_USART3_CLK_ENABLE(); }while(0)  /* USART1 时钟使能 */




#define BUFFER_SIZE_FOR_USART1 1


// 头文件内容，这里需要明确自己要什么，总目标是用pa9,10实现串口通信
//这个文件不是面向用户的，自己写就行，没必要整好用的api


void half_duplex_uart_init(uint32_t baudrate);//实现初始化
HAL_StatusTypeDef half_duplex_uart_transmit(uint8_t *pData, uint16_t Size, uint32_t Timeout);//阻塞性发送
HAL_StatusTypeDef half_duplex_uart_receive(uint8_t *pData, uint16_t Size, uint32_t Timeout);//阻塞性接受


void uart_init(uint32_t baudrate);
HAL_StatusTypeDef uart_transmit(uint8_t *pData, uint16_t Size, uint32_t Timeout);
