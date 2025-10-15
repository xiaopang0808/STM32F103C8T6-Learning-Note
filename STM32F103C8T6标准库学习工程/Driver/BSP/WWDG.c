/******************************************************************************
* 文件名：WWDG.c
* 作　者：Xiaopang
* 创建日期：2025-07-28
* 最后修改：2025-07-28
* 版　本：v1.0
* 功能描述：
*
*   本文件实现窗口看门狗(WWDG)硬件控制模块，主要功能包括：
*    1. 初始化窗口看门狗定时器
*    2. 配置看门狗时钟和中断
*    3. 实现中断服务例程和早期唤醒回调
*
* 修改记录：
*  版本   日期       修改人     修改内容
*  v1.0  2025-07-28  Xiaopang   初始版本创建
*
* 注意事项：
*  1. WWDG特性：只能在"窗口"范围内刷新计数器
*  2. 计数器值必须大于0x40
*  3. 时钟源为PCLK1（通常36/72MHz）
*
******************************************************************************/

#include "WWDG.h"


/*********************************************************************/
/* 主要功能函数 */

/**
  * @brief  窗口看门狗初始化
  * @param  tr: 计数器初始值 (0x40-0x7F)
  * @param  wr: 窗口值 (0x40-0x7F, 必须大于计数器值)
  * @param  fprer: 预分频系数
  *         @arg WWDG_PRESCALER_1: 1分频
  *         @arg WWDG_PRESCALER_2: 2分频
  *         @arg WWDG_PRESCALER_4: 4分频
  *         @arg WWDG_PRESCALER_8: 8分频
  * @note   实际超时时间计算公式:
  *         T_watchdog = (4 * 2^fprer * tr) / PCLK1 * 10^6 (μs)
  *         示例: PCLK1=36MHz, fprer=8, tr=0x7F ≈ 58.25ms
  */
void wwdg_init(uint8_t tr, uint8_t wr, uint32_t fprer)
{
    /*开启时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);	//开启WWDG的时钟
	
	/*WWDG初始化*/
	WWDG_SetPrescaler(fprer);			
	WWDG_SetWindowValue(wr);					
	WWDG_Enable(tr);	
	
	WWDG_EnableIT();
	
	NVIC_InitTypeDef NVIC_InitStructure;

	// 设置NVIC中断
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  // 选择中断优先级分组
	NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;  // WWDG中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  // 抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;          // 响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            // 启用中断
	NVIC_Init(&NVIC_InitStructure);
	
	
}


/*********************************************************************/
/* 中断服务函数 */

/**
  * @brief  WWDG中断服务例程
  * @note   调用HAL库中断处理函数
  */
void WWDG_IRQHanddler(void)
{
    if(WWDG_GetFlagStatus() == SET)
    {
        WWDG_ClearFlag();            // 清除中断标志
        
        // 其他处理逻辑...
    }
}



/******************************************************************************
* 测试代码示例 
******************************************************************************/
#if 0
#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Key.h"
#include "WWDG.h"
/**
  * @brief  主函数 - WWGD窗口看门狗演示程序
  * @note   演示功能：
  *         1. 检测上次复位源
  *         2. 30ms周期性喂狗
  *         3. OLED显示喂狗状态
  */
int main()
{
    /* 系统初始化 */
    OLED_Init();                                    // OLED显示屏初始化
	
    /*判断复位信号来源*/
	if (RCC_GetFlagStatus(RCC_FLAG_WWDGRST) == SET)	//如果是窗口看门狗复位
	{
		OLED_ShowString(2, 1, "WWDGRST");			//OLED闪烁WWDGRST字符串
		Delay_ms(500);
		OLED_ShowString(2, 1, "       ");
		Delay_ms(100);
		
		RCC_ClearFlag();							//清除标志位
	}
	else											//否则，即为其他复位
	{
		OLED_ShowString(3, 1, "RST");				//OLED闪烁RST字符串
		Delay_ms(500);
		OLED_ShowString(3, 1, "   ");
		Delay_ms(100);
	}
    
    Delay_ms(500);                                 // 延时便于观察复位状态
    
    /* 初始化窗口看门狗 */
    // 计数器初值：0x7F (127)
    // 窗口值：0x5F (95) - 计数器值必须小于95时刷新
    // 预分频：8分频
    // 理论超时范围：29.13ms~58.25ms (PCLK1=36MHz时)
    wwdg_init(0x7f, 0x5f, WWDG_Prescaler_8); 
    
    /* 主循环 - 定时喂狗操作 */
    while(1)
    {
        Delay_ms(40);                               // 30ms延时（在窗口期内）
        WWDG_SetCounter(0x7F);        				// 刷新看门狗计数器
        OLED_ShowString(1, 1, "Feed");              // 显示喂狗状态
    }
}
#endif
/******************************************************************************/