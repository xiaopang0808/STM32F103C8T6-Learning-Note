/******************************************************************************
* 文件名：OLED.h
* 作　者：Xiaopang
* 创建日期：2025-07-15
* 最后修改：2025-07-15
* 版　本：v2.0
* 功能描述：
*
*   本文件为OLED显示屏控制模块的接口定义，提供OLED初始化和显示功能的封装
*   核心功能包括：
*    1. OLED通信引脚配置定义（I2C接口）
*    2. OLED电平控制宏（SCL/SDA信号线控制）
*    3. OLED显示控制函数声明（清屏/字符/字符串/数字显示等）
*
* 修改记录：
*  版本   日期       修改人     修改内容
*  v1.0  2025-07-15  Xiaopang   移植江协科技0.96英寸四针脚OLED显示屏代码
*  v2.0  2025-07-15  Xiaopang   修改库函数调用，适配HAL库
*
* 注意事项：
*  1. 使用前需确保HAL库初始化完成
*  2. 使用I2C协议通信，需正确配置GPIO引脚模式
*  3. 显示坐标参数：Line表示行号(0~7)，Column表示列号(0~16)
*
******************************************************************************/

#ifndef __OLED_H
#define __OLED_H

#include "sys.h"

/* OLED控制引脚 -----------------------------------------------------------*/
/* 使用GPIOB组的引脚8作为SCL时钟线，引脚9作为SDA数据线 */

#define OLED_GPIO_PORT               GPIOB      /* GPIO端口组 */
#define OLED_SCL                     GPIO_PIN_8 /* 时钟线引脚 */
#define OLED_SDA                     GPIO_PIN_9 /* 数据线引脚 */
#define OLED_GPIO_CLK_ENABLE()       do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0) /* 时钟使能宏 */

/* 引脚配置宏 -------------------------------------------------------------*/
/**
  * @brief  控制OLED_SCL引脚电平状态
  * @param  x: 0=置低电平, 非0=置高电平
  */
#define OLED_W_SCL(x)    do{ x ?  \
                            HAL_GPIO_WritePin(OLED_GPIO_PORT, OLED_SCL, GPIO_PIN_SET) : \
                            HAL_GPIO_WritePin(OLED_GPIO_PORT, OLED_SCL, GPIO_PIN_RESET); \
                        }while(0)

/**
  * @brief  控制OLED_SDA引脚电平状态
  * @param  x: 0=置低电平, 非0=置高电平
  */
#define OLED_W_SDA(x)    do{ x ?  \
                            HAL_GPIO_WritePin(OLED_GPIO_PORT, OLED_SDA, GPIO_PIN_SET) : \
                            HAL_GPIO_WritePin(OLED_GPIO_PORT, OLED_SDA, GPIO_PIN_RESET); \
                        }while(0)

/* 外部接口函数 -----------------------------------------------------------*/
void OLED_Init(void);                           /* OLED初始化 */
void OLED_Clear(void);                          /* 清空屏幕 */
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);          /* 显示单个字符 */
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);     /* 显示字符串 */
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);       /* 显示无符号数字 */
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);  /* 显示有符号数字 */
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);   /* 显示十六进制数 */
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);   /* 显示二进制数 */

#endif


