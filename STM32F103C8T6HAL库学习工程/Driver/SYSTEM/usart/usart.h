/******************************************************************************
* 文件名：USART.h
* 作　者：Xiaopang
* 创建日期：2025-07-18
* 最后修改：2025-07-18
* 版　本：v1.0
* 功能描述：STM32F103C8T6 USART驱动头文件，提供串口通信相关的宏定义和函数声明
*           支持异步全双工通信，默认使用USART1
*
* 修改记录：
*  版本   日期       修改人     修改内容
*  v1.0  2025-07-18  Xiaopang   初始版本创建
*
* 注意事项：
*   STM32F103C8T6 USART引脚映射:
*     USART1_TX -> PA9  USART1_RX -> PA10
*     USART2_TX -> PA2  USART2_RX -> PA3
*     USART3_TX -> PB10 USART3_RX -> PB11
*   通过修改宏定义可切换不同串口
******************************************************************************/

#ifndef __USART_H
#define __USART_H

#include "sys.h"
#include "stdio.h"

/******************************************************************************************/
/* 引脚和串口定义 
 * 默认配置为USART1（PA9-TX, PA10-RX）
 * 修改以下宏定义可切换到其他串口
 */
// USART1 TX引脚配置（PA9）
#define USART_TX_GPIO_PORT                  GPIOA       // TX引脚所在端口
#define USART_TX_GPIO_PIN                   GPIO_PIN_9  // TX引脚编号
#define USART_TX_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0) // PA端口时钟使能

// USART1 RX引脚配置（PA10）
#define USART_RX_GPIO_PORT                  GPIOA       // RX引脚所在端口
#define USART_RX_GPIO_PIN                   GPIO_PIN_10 // RX引脚编号
#define USART_RX_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0) // PA端口时钟使能

// USART1外设配置
#define USART_UX                            USART1      // 使用的USART外设
#define USART_UX_IRQn                       USART1_IRQn // 对应的中断向量
#define USART_UX_IRQHandler                 USART1_IRQHandler // 中断服务函数名
#define USART_UX_CLK_ENABLE()               do{ __HAL_RCC_USART1_CLK_ENABLE(); }while(0) // USART1时钟使能
/******************************************************************************************/

#define BUFFER_SIZE 1						//接收数据缓冲区大小
extern UART_HandleTypeDef g_uart_handle;   // 全局UART处理结构体
extern uint8_t g_rx_buffer[BUFFER_SIZE];             // 接收数据缓冲区（单字节）
extern uint8_t g_usart_rx_flag;            // 接收到新数据标志位（1=接收到数据）

// 函数声明
void USART_Init(uint32_t baudrate);         // USART初始化函数

#endif /* __USART_H */