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
* 
*
******************************************************************************/

#include "stm32f10x.h"                  // Device header
#include "LED.h"

void LED_Init(void)
{
	 GPIO_InitTypeDef gpio_init_struct;
	/* LED时钟使能 */
	LED0_GPIO_CLK_ENABLE();
	LED1_GPIO_CLK_ENABLE();
	
	gpio_init_struct.GPIO_Pin=LED0_GPIO_PIN;		/* LED0引脚 */
	gpio_init_struct.GPIO_Mode=GPIO_Mode_Out_PP;	/* 推挽输出 */
	gpio_init_struct.GPIO_Speed=GPIO_Speed_50MHz;	/* 高速 */
	GPIO_Init(LED0_GPIO_PORT,&gpio_init_struct);	/* 初始化LED0引脚 */
	
	gpio_init_struct.GPIO_Pin=LED1_GPIO_PIN;		/* LED1引脚 */
	GPIO_Init(LED0_GPIO_PORT,&gpio_init_struct);	/* 初始化LED1引脚 */
	
	/* LED采用共阳极接法 */
	LED0(1);	/* 关闭LED0 */
	LED1(1);	/* 关闭LED1 */

}


/******************************************************************************
* 测试代码示例
******************************************************************************/
#if 0
#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
/**
  * @brief  主函数 - LED闪烁示例程序
  * @note   演示LED0和LED1交替闪烁效果
  */
int main()
{
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

