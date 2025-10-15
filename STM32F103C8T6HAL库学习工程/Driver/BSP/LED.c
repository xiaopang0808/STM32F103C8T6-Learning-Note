/******************************************************************************
* 文件名：LED.c
* 作　者：Xiaopang
* 创建日期：2025-07-15
* 最后修改：2025-07-15
* 版　本：v1.0
*
* 功能描述：
*   本文件实现LED硬件控制模块的初始化和配置功能，主要功能包括：
*    1. 初始化LED对应的GPIO引脚
*    2. 配置引脚为推挽输出模式
*    3. 设置LED初始状态（共阳极设计下关闭所有LED）
*
* 修改记录：
*  版本   日期       修改人     修改内容
*  v1.0  2025-07-15  Xiaopang   初始版本创建
*
* 注意事项：
*  1. 本模块假设LED采用共阳极接法：
*     - 高电平关闭LED
*     - 低电平点亮LED
*  2. 使用前需确保HAL库已初始化
*  3. 需正确配置系统时钟（如main函数中的sys_stm32_clock_init）
*
******************************************************************************/


#include "LED.h"

/**
  * @brief  LED硬件初始化函数
  * @note   配置所有LED控制引脚为推挽输出模式，并初始化LED为关闭状态
  * @param  无
  * @retval 无
  *
  * 功能说明：
  *  1. 使能LED控制引脚的GPIO时钟
  *  2. 配置GPIO引脚为高速推挽输出
  *  3. 初始状态设置为关闭所有LED（输出高电平）
  *
  * 硬件设计：
  *  - 本开发板LED采用共阳极接法，高电平关闭，低电平点亮
  */
void LED_Init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    
    /* LED0引脚时钟使能 */
    LED0_GPIO_CLK_ENABLE();
    /* LED1引脚时钟使能 */
    LED1_GPIO_CLK_ENABLE();
    
    /* LED0引脚配置 */
    gpio_init_struct.Pin = LED0_GPIO_PIN;                 /* 设置LED0控制引脚 */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;          /* 推挽输出模式 */
    gpio_init_struct.Pull = GPIO_NOPULL;                  /* 无上下拉电阻 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;        /* 高速模式 */
    HAL_GPIO_Init(LED0_GPIO_PORT, &gpio_init_struct);     /* 应用LED0引脚配置 */

    /* LED1引脚配置 */
    gpio_init_struct.Pin = LED1_GPIO_PIN;                 /* 设置LED1控制引脚 */
    HAL_GPIO_Init(LED1_GPIO_PORT, &gpio_init_struct);     /* 应用LED1引脚配置 */
    
    /* 初始状态：关闭所有LED（共阳极接法，输出高电平关闭） */
    LED0(1);    /* 关闭LED0 */
    LED1(1);    /* 关闭LED1 */
}


/******************************************************************************
* 测试代码示例
******************************************************************************/
#if 0
#include "stm32f1xx.h"
#include "sys.h"
#include "delay.h"
#include "led.h"

/**
  * @brief  主函数 - LED闪烁示例程序
  * @note   演示LED0和LED1交替闪烁效果
  */
int main()
{
    /* 系统初始化 */
    HAL_Init();                                 /* HAL库初始化 */
    sys_stm32_clock_init(RCC_PLL_MUL9);         /* 系统时钟配置 */
    LED_Init();                                 /* LED硬件初始化 */
    
    /* 主循环 - LED交替闪烁 */
    while(1)
    {
        LED0(0);  /* 点亮LED0 */
        LED1(1);  /* 关闭LED1 */
        Delay_ms(500);  /* 延时500ms */
        
        LED0(1);  /* 关闭LED0 */
        LED1(0);  /* 点亮LED1 */
        Delay_ms(500);  /* 延时500ms */
    }
}
#endif
/******************************************************************************/
