/******************************************************************************
* 文件名：Base_Timer.c
* 作　者：Xiaopang
* 创建日期：2025-08-01
* 最后修改：2025-08-01
* 版　本：v1.0
*
* 功能描述：
*   - 实现TIM2定时器初始化配置
*   - 处理定时器中断服务流程：
*       1. TIM2_IRQHandler: 中断入口 
*       2. HAL_TIM_IRQHandler: HAL库中断分发
*       3. HAL_TIM_PeriodElapsedCallback: 周期中断回调函数
*   - LED0状态翻转作为定时中断触发标识
*
* 修改记录：
*  版本   日期       修改人     修改内容
*  v1.0  2025-08-01  Xiaopang   初始版本创建
*
* 注意事项：
*   1. 使用TIM2替代基本定时器，主频72MHz时:
*       定时周期T = (arr+1)*(psc+1)/72 (us)
*   2. 时钟使能/NVIC配置在MspInit回调实现
*   3. LED0引脚需预先在LED.h中正确配置
******************************************************************************/

#include "Base_Timer.h"

TIM_HandleTypeDef g_timx_handle;  // 定时器全局句柄

/*******************************************************
* 函数：Base_Timx_Init
* 描述：初始化TIM2定时器并启动中断
* 参数：
*   psc - 预分频系数（输入时钟分频因子）
*   arr - 自动重装载值（计数上限）
* 备注：
*   预分频器实际值 = psc + 1
*   自动重装载实际值 = arr + 1
*******************************************************/
void Base_Timx_Init(uint16_t psc, uint16_t arr)
{
	g_timx_handle.Instance = TIM2;		// 选择TIM2定时器
	g_timx_handle.Init.Prescaler = psc; // 设置预分频系数
	g_timx_handle.Init.Period = arr;    // 设置自动重装载值
	HAL_TIM_Base_Init(&g_timx_handle);  // 初始化定时器
	
	HAL_TIM_Base_Start_IT(&g_timx_handle); // 启动定时器中断模式
}

/*******************************************************
* HAL库硬件初始化回调函数（时钟/NVIC配置）
* 注意：此函数由HAL_TIM_Base_Init()自动调用
*******************************************************/
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM2)
	{
		__HAL_RCC_TIM2_CLK_ENABLE();       // 使能TIM2时钟
		HAL_NVIC_SetPriority(TIM2_IRQn,1,3); // 设置中断优先级
		HAL_NVIC_EnableIRQ(TIM2_IRQn);     // 使能TIM2中断通道
	}
}

/*******************************************************
* TIM2中断服务函数
* 备注：所有TIM2中断都由此入口处理
*******************************************************/
//void TIM2_IRQHandler(void)
//{
//	HAL_TIM_IRQHandler(&g_timx_handle); // HAL库中断处理
//}

/*******************************************************
* 定时器周期更新中断回调函数
* 备注：
*   当计数器达到自动重装载值时触发
*   此处实现LED0状态翻转作为中断触发指示
*******************************************************/
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//	if(htim->Instance == TIM2)
//	{
//		LED0_TOGGLE();  // 翻转LED0状态
//	}
//}

/******************************************************************************
* 测试代码示例说明：
*   1. 初始化系统时钟72MHz
*   2. 配置定时器参数：预分频5000-1，重装载7200-1
*      定时周期计算：T = (5000)*(7200)/72,000,000 = 0.5秒
*   3. LED0将以1Hz频率闪烁（0.5秒亮，0.5秒灭）
******************************************************************************/
#if 0
#include "stm32f1xx.h"
#include "sys.h"
#include "delay.h"
#include "LED.h"
#include "Base_Timer.h"
int main()
{
    HAL_Init();
    sys_stm32_clock_init(RCC_PLL_MUL9);		/* 初始化系统时钟 */
	OLED_Init();
	LED_Init();
	Base_Timx_Init(5000-1,7200-1);
    while(1)
    {

	}
}
#endif
/******************************************************************************/
