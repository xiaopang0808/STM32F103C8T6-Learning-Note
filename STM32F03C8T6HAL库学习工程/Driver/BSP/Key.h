/******************************************************************************
* 文件名：Key.h
* 作　者：Xiaopang
* 创建日期：2025-07-16
* 最后修改：2025-07-16
* 版　本：v1.0
*
* 功能描述：
*   按键模块头文件，定义：
*   - 按键硬件引脚映射
*		1. KEY0 -> PB12
*		2. KEY1 -> PB13
*   - 按键初始化及检测函数声明
*
* 修改记录：
*  版本   日期       修改人     修改内容
*  v1.0  2025-07-16  Xiaopang   初始版本创建
*
******************************************************************************/

#ifndef __KEY_H
#define __KEY_H

#include "sys.h"

/*********************************************************************/
/* 引脚定义 */

/* KEY0引脚配置 */
#define KEY0_GPIO_PORT                  GPIOB
#define KEY0_GPIO_PIN                   GPIO_PIN_12
#define KEY0_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)

/* KEY1引脚配置 */
#define KEY1_GPIO_PORT                  GPIOB
#define KEY1_GPIO_PIN                   GPIO_PIN_13
#define KEY1_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)
					
/*********************************************************************/
/* 外部接口函数 */

void Key0_Init(void);       // 初始化KEY0（轮询检测方式）
uint8_t Key0_scan(void);    // 轮询检测KEY0按键状态
void Key1_Init(void);       // 初始化KEY1（外部中断方式）

#endif /* __KEY_H */

