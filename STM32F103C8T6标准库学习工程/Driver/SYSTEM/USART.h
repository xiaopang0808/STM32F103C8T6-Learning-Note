/******************************************************************************
* 文件名：USART.h
* 作　者：Xiaopang
* 创建日期：2025-07-18
* 最后修改：2025-07-26
* 版　本：v1.1
* 功能描述：STM32F103C8T6串口(USART)驱动头文件
*           支持USART1/2/3的异步全双工通信，默认配置为USART1
*           提供数据发送、接收及格式化打印功能
*
* 修改记录：
*  v1.1 2025-07-26  修正GPIO初始化错误，完善状态机注释
*
* 注意事项：
* STM32F103C8T6 USART引脚定义:
*   USART1_TX -> PA9   USART1_RX -> PA10
*   USART2_TX -> PA2   USART2_RX -> PA3
*   USART3_TX -> PB10  USART3_RX -> PB11
******************************************************************************/

#ifndef __USART_H
#define __USART_H

#include "stdio.h"
#include "stdarg.h"

/*********************************************************************/
/* 引脚和串口配置 - 修改以下宏定义可切换串口 */
// USART1 TX引脚配置(PA9)
#define USART_TX_GPIO_PORT                  GPIOA       // TX引脚端口
#define USART_TX_GPIO_PIN                   GPIO_Pin_9  // TX引脚编号
#define USART_TX_GPIO_CLK_ENABLE()          do{ RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); }while(0)

// USART1 RX引脚配置(PA10)
#define USART_RX_GPIO_PORT                  GPIOA       // RX引脚端口
#define USART_RX_GPIO_PIN                   GPIO_Pin_10 // RX引脚编号
#define USART_RX_GPIO_CLK_ENABLE()          do{ RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); }while(0)

// USART1外设配置
#define USART_UX                            USART1      // 使用的USART外设
#define USART_UX_IRQn                       USART1_IRQn // 中断向量
#define USART_UX_CLK_ENABLE()               do{ RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE); }while(0)
/*********************************************************************/

/* 外部接口函数声明 */
void USART_Init_Func(void);                       // 串口初始化
void USART_SendByte(uint8_t Byte);                // 发送单字节
void USART_SendArray(uint8_t *Array, uint16_t Length); // 发送字节数组
void USART_SendString(char *String);              // 发送字符串
void USART_SendNumber(uint32_t Number, uint8_t Length); // 发送数字
void USART_Printf(char *format, ...);             // 格式化打印

/* 全局变量声明 */
extern char USART_RxPacket[100];   // 接收数据包缓冲区(格式:"@MSG\r\n")
extern uint8_t USART_RxFlag;       // 接收完成标志(1:收到完整数据包)

#endif /* __USART_H */
