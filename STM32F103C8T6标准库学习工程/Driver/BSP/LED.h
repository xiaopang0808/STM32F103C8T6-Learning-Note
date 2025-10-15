/******************************************************************************
* 文件名：LED.h
* 作　者：Xiaopang
* 创建日期：2025-07-15
* 最后修改：2025-07-15
* 版　本：v1.0
* 功能描述：
*
*   本文件为LED硬件控制模块的接口定义，提供LED初始化和控制的宏封装
*   核心功能包括：
*    1. LED引脚配置定义（端口/引脚/时钟使能）
*    2. LED电平控制宏（开关/翻转）
*    3. LED初始化函数声明
*
* 修改记录：
*  版本   日期       修改人     修改内容
*  v1.0  2025-07-15  Xiaopang   初始版本创建
*
* 注意事项：
*  1. LEDx(x)参数: 0=关闭, 1=开启
*  2. 所有宏设计为无返回值
*
******************************************************************************/

#ifndef __LED_H
#define __LED_H

/*********************************************************************/
/* 引脚定义 */

/* LED0控制引脚 */
#define LED0_GPIO_PORT					GPIOA
#define LED0_GPIO_PIN					GPIO_Pin_0
#define LED0_GPIO_CLK_ENABLE()			do{ RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); }while(0)

/* LED1控制引脚 */
#define LED1_GPIO_PORT					GPIOA
#define LED1_GPIO_PIN					GPIO_Pin_1
#define LED1_GPIO_CLK_ENABLE()			do{ RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); }while(0)

/*********************************************************************/
/* LED端口定义 */

/**
  * @brief  控制LED0开关状态
  * @param  x: 0=关闭, 非0=开启
  */
#define LED0(x)		do{	x ?  \
						GPIO_SetBits(LED0_GPIO_PORT, LED0_GPIO_PIN) : \
						GPIO_ResetBits(LED0_GPIO_PORT, LED0_GPIO_PIN); \
					}while(0)	/* LED0电平控制 */
/**
  * @brief  控制LED1开关状态
  * @param  x: 0=关闭, 非0=开启
  */
#define LED1(x)		do{	x ?  \
						GPIO_SetBits(LED1_GPIO_PORT, LED1_GPIO_PIN) : \
						GPIO_ResetBits(LED1_GPIO_PORT, LED1_GPIO_PIN); \
					}while(0)	/* LED1电平控制 */

/*********************************************************************/
/* 外部接口函数 */

void LED_Init(void);			/* LED模块初始化 */

#endif /* __LED_H */
