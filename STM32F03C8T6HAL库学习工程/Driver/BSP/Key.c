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

#include "Key.h"
#include "delay.h"

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
    gpio_init_struct.Mode = GPIO_MODE_INPUT;    		// 输入模式
    gpio_init_struct.Pin = KEY0_GPIO_PIN;       		// KEY0引脚
    gpio_init_struct.Pull = GPIO_PULLDOWN;       		// 下拉电阻（按键未按下时保持低电平）
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH; 		// 高速模式
    HAL_GPIO_Init(KEY0_GPIO_PORT, &gpio_init_struct);
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
    if (HAL_GPIO_ReadPin(KEY0_GPIO_PORT, KEY0_GPIO_PIN) == GPIO_PIN_SET)
    {
        /* 消抖延时 */
        Delay_ms(20);
        /* 确认按键状态 */
        if (HAL_GPIO_ReadPin(KEY0_GPIO_PORT, KEY0_GPIO_PIN) == GPIO_PIN_SET)
        {
            /* 等待按键释放 */
            while (HAL_GPIO_ReadPin(KEY0_GPIO_PORT, KEY0_GPIO_PIN) == GPIO_PIN_SET)
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
    
    /* 配置按键引脚参数 */
    gpio_init_struct.Pin = KEY1_GPIO_PIN;        // KEY1引脚
    gpio_init_struct.Mode = GPIO_MODE_IT_RISING; // 上升沿中断触发模式
    gpio_init_struct.Pull = GPIO_PULLDOWN;       // 下拉电阻
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(KEY1_GPIO_PORT, &gpio_init_struct);
    
    /* 配置NVIC中断 */
    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);  // 设置抢占优先级0，子优先级0
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);          // 使能外部中断通道
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
    /* 处理KEY1触发的中断 */
    HAL_GPIO_EXTI_IRQHandler(KEY1_GPIO_PIN);
    /* 清除KEY1对应的中断标志位 */
    __HAL_GPIO_EXTI_CLEAR_IT(KEY1_GPIO_PIN); 
}

/**
  * @brief  外部中断回调函数
  * @param  GPIO_Pin: 触发中断的引脚编号
  * @retval 无
  * @note   实际按键检测在此执行，进行软件消抖后执行相应操作
  *         注意：中断服务程序中不宜执行耗时操作
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    /* 确认是KEY1触发的中断 */
    if (GPIO_Pin == KEY1_GPIO_PIN)
    {
        /* 消抖延时（实际应用中考虑使用其他消抖方式） */
        Delay_ms(20);
        /* 确认按键状态 */
        if (HAL_GPIO_ReadPin(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == GPIO_PIN_SET)
        {
            /* 执行按键操作 */
			
        }
    }
}

/******************************************************************************
* 测试代码示例
******************************************************************************/
#if 0
#include "stm32f1xx.h"
#include "sys.h"
#include "delay.h"
#include "LED.h"
#include "Key.h"
int main()
{
    HAL_Init();
    sys_stm32_clock_init(RCC_PLL_MUL9);		/* 初始化系统时钟 */
	LED_Init();
	Key0_Init();
	Key1_Init();
    while(1)
    {
		if(Key0_scan())
		{
			LED0_TOGGLE();
		}
	}
}
#endif
/******************************************************************************/
