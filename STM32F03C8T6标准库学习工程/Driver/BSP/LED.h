#ifndef __LED_H
#define __LED_H

/*********************************************************************/
/* 引脚定义 */

#define LED0_GPIO_PORT					GPIOA
#define LED0_GPIO_PIN					GPIO_Pin_0
#define LED0_GPIO_CLK_ENABLE()			do{ RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); }while(0)

#define LED1_GPIO_PORT					GPIOA
#define LED1_GPIO_PIN					GPIO_Pin_1
#define LED1_GPIO_CLK_ENABLE()			do{ RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); }while(0)

/*********************************************************************/
/* LED端口定义 */
#define LED0(x)		do{	x ?  \
						GPIO_SetBits(LED0_GPIO_PORT, LED0_GPIO_PIN) : \
						GPIO_ResetBits(LED0_GPIO_PORT, LED0_GPIO_PIN); \
					}while(0)	/* LED0电平控制 */

#define LED1(x)		do{	x ?  \
						GPIO_SetBits(LED1_GPIO_PORT, LED1_GPIO_PIN) : \
						GPIO_ResetBits(LED1_GPIO_PORT, LED1_GPIO_PIN); \
					}while(0)	/* LED1电平控制 */

/*********************************************************************/
/* 外部接口函数 */
void LED_Init(void);

#endif
