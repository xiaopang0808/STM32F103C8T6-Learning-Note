/******************************************************************************
* 文件名：KEY.c
* 作　者：Xiaopang
* 创建日期：2025-07-16
* 最后修改：2025-07-16
* 版　本：v1.0
*
* 功能描述：
*   该文件实现了按键检测功能：
*   - KEY0 通过轮询扫描方式检测
*   - KEY1 通过外部中断方式检测
*   当按键按下时触发相应操作
*
* 修改记录：
*  版本   日期       修改人     修改内容
*  v1.0  2025-07-16  Xiaopang   初始版本创建
*
* 注意事项：
*   1. KEY0 使用PB12引脚，检测时采用软件消抖机制（阻塞式）
*   2. KEY1 使用PB13引脚，配置为上升沿触发的外部中断
*   3. 中断服务程序中需要清除对应中断标志位
*
******************************************************************************/

#include "stm32f10x.h"                  // Device header
#include "Key.h"
#include "Delay.h"
#include "LED.h"
#include "OLED.h"


/**
  * @brief  KEY0 按键初始化
  * @param  无
  * @retval 无
  * @note   配置PB12为下拉输入模式，准备检测按键信号
  */
void Key0_Init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    
    /* 使能GPIO时钟 */
    KEY0_GPIO_CLK_ENABLE();
    
    /* 配置按键引脚参数 */
    gpio_init_struct.GPIO_Mode = GPIO_Mode_IPD;    			// 下拉输入模式
    gpio_init_struct.GPIO_Pin = KEY0_GPIO_PIN;       		// KEY0引脚
    gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz; 		// 高速模式
    GPIO_Init(KEY0_GPIO_PORT, &gpio_init_struct);
}

/**
  * @brief  KEY0 按键状态检测
  * @param  无
  * @retval uint8_t 
  *         @arg 1: 检测到有效按键按下
  *         @arg 0: 无按键按下
  * @note   采用软件消抖机制，检测到高电平后延时20ms再次确认
  *         等待按键释放后才返回检测结果，避免连续触发
  */
uint8_t Key0_scan(void)
{
    /* 检测按键引脚是否变为高电平 */
    if (GPIO_ReadInputDataBit(KEY0_GPIO_PORT, KEY0_GPIO_PIN) == Bit_SET)
    {
        /* 消抖延时 */
        Delay_ms(20);
        /* 确认按键状态 */
        if (GPIO_ReadInputDataBit(KEY0_GPIO_PORT, KEY0_GPIO_PIN) == Bit_SET)
        {
            /* 等待按键释放 */
            while (GPIO_ReadInputDataBit(KEY0_GPIO_PORT, KEY0_GPIO_PIN) == Bit_SET)
            {
                // 保持循环直到按键松开
            }
            return 1;  // 返回有效按键按下次号
        }
    }
    return 0;  // 无按键按下
}

/**
  * @brief  KEY1 按键初始化（外部中断方式）
  * @param  无
  * @retval 无
  * @note   配置PB13为上升沿触发的外部中断模式
  *         初始化NVIC设置中断优先级并使能中断
  */
void Key1_Init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    
    /* 使能GPIO时钟 */
    KEY1_GPIO_CLK_ENABLE();
	/* 打开AFIO的时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);         
    
    /* 配置按键引脚参数 */
    gpio_init_struct.GPIO_Pin = KEY1_GPIO_PIN;        		// KEY1引脚
    gpio_init_struct.GPIO_Mode = GPIO_Mode_IPD; 			// 下拉输入模式 参考手册：三种输入模式皆可
    gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(KEY1_GPIO_PORT, &gpio_init_struct);
    
	/* 配置AFIO */
    /* 将 GPIOB 的第 13 号引脚（PB13）配置为外部中断（EXTI）的触发源 */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource13);
	
	/* 配置EXTI */
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line=EXTI_Line13 ;      		//13号线路
    EXTI_InitStructure.EXTI_LineCmd=ENABLE ;        		//使能
    EXTI_InitStructure.EXTI_Mode= EXTI_Mode_Interrupt;      //中断模式
    EXTI_InitStructure.EXTI_Trigger= EXTI_Trigger_Rising; 	//上升沿触发
    EXTI_Init(&EXTI_InitStructure);
    
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel=EXTI15_10_IRQn;      //指定通道，13通道在15——10的合并通道里
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;			//使能
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0; //指定抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;        //指定相应优先级
    NVIC_Init(&NVIC_InitStructure);

}

void Key1_scan(void)
{
    /* 消抖延时（实际应用中考虑使用其他消抖方式） */
	Delay_ms(20);
    /* 确认按键状态 */
    if (GPIO_ReadInputDataBit(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == SET)
    {
		/* 执行按键操作 */
		if(GPIO_ReadOutputDataBit(LED1_GPIO_PORT,LED1_GPIO_PIN))
		{
			LED1(0);
			OLED_ShowString(2,1,"LED1 ON");
		}
		else
		{
			LED1(1);
			OLED_ShowString(2,1,"LED1 OFF");
		}
    }

}

/**
  * @brief  EXTI15_10中断服务函数
  * @param  无
  * @retval 无
  * @note   处理PB10-PB15引脚触发的外部中断
  *         调用HAL库中断处理函数
  */
void EXTI15_10_IRQHandler(void)
{        
    if(EXTI_GetITStatus(EXTI_Line13)==SET){
		Key1_scan();
        EXTI_ClearITPendingBit(EXTI_Line13);
    }
}



/******************************************************************************
* 测试代码示例
******************************************************************************/
#if 0
#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "OLED.h"
#include "Key.h"
int main()
{
	LED_Init();
	OLED_Init();
	Key0_Init();
	Key1_Init();
    while(1)
    {
		if(Key0_scan())
		{
			if(GPIO_ReadOutputDataBit(LED0_GPIO_PORT,LED0_GPIO_PIN))
			{
				LED0(0);
				OLED_ShowString(1,1,"LED0 ON");
			}
			else
			{
				LED0(1);
				OLED_ShowString(1,1,"LED0 OFF");
			}
		}		
	}
    return 0;
}
#endif
/******************************************************************************/
