/******************************************************************************
* 文件名：General_Timer_OC.c
* 作　者：Xiaopang
* 创建日期：2025-08-19
* 最后修改：2025-08-19
* 版　本：v1.0
*
* 功能描述：
*   - 实现通用定时器(TIM3)的PWM初始化及配置
*   - 配置TIM3通道2输出PWM信号
*   - GPIO配置：PA7复用为TIM3_CH2引脚
*   - 支持设置自动重装载值和预分频系数
*
* 修改记录：
*  版本   日期       修改人     修改内容
*  v1.0  2025-08-19  Xiaopang   初始版本创建
*
* 注意事项：
*   - 使用TIM3通道2(对应PA7引脚)
*   - PWM输出模式：PWM1模式
*   - 默认占空比：50%(arr/2)
******************************************************************************/

#include "General_Timer_IC.h"

TIM_HandleTypeDef g_timx_cap_chy_handle;

void gtim_timx_cap_chy_ic_init(uint16_t psc,uint16_t arr)
{
	TIM_IC_InitTypeDef timx_ic_cap_chy = {0};
	
	g_timx_cap_chy_handle.Instance = TIM2;
	g_timx_cap_chy_handle.Init.Prescaler = psc;
	g_timx_cap_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	g_timx_cap_chy_handle.Init.Period = arr;
	HAL_TIM_IC_Init(&g_timx_cap_chy_handle);
	
	timx_ic_cap_chy.ICPolarity = TIM_ICPOLARITY_RISING;
	timx_ic_cap_chy.ICSelection = TIM_ICSELECTION_INDIRECTTI;
	timx_ic_cap_chy.ICPrescaler = TIM_ICPSC_DIV1;
	timx_ic_cap_chy.ICFilter = 0;
	HAL_TIM_IC_ConfigChannel(&g_timx_cap_chy_handle,&timx_ic_cap_chy,TIM_CHANNEL_4);
	
	__HAL_TIM_ENABLE_IT(&g_timx_cap_chy_handle,TIM_IT_UPDATE);
	HAL_TIM_IC_Start_IT(&g_timx_cap_chy_handle,TIM_CHANNEL_4);
}

void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM2)
	{
		GPIO_InitTypeDef gpio_init_struct;
		__HAL_RCC_TIM2_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		
		gpio_init_struct.Pin = GPIO_PIN_3;
		gpio_init_struct.Mode = GPIO_MODE_AF_INPUT;
		gpio_init_struct.Pull = GPIO_PULLDOWN;
		gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOA,&gpio_init_struct);
		
		HAL_NVIC_SetPriority(TIM2_IRQn,1,3);
		HAL_NVIC_EnableIRQ(TIM2_IRQn);
	}
}

/* 输入捕获状态(g_timxchy_cap_sta)
 * [7]  0，没有成功的捕获；1，成功捕获到一次
 * [6]	0，还没捕获到高电平；1，已经捕获到高电平了
 * [5:0]：捕获高电平后溢出的次数，最多溢出63次，所以最长捕获值 = 63*65536 + 65535 = 4194303
 *		  注意：为了通用，默认ARR和CCTy都是16位寄存器，对于32位定时器
 */
uint8_t g_timxchy_cap_sta = 0;
uint16_t g_timxchy_cap_val = 0;



void TIM2_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&g_timx_cap_chy_handle);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM2)
	{
		if((g_timxchy_cap_sta & 0x80) == 0)
		{
			if(g_timxchy_cap_sta & 0x40)
			{
				g_timxchy_cap_sta |=0x80;
				g_timxchy_cap_val = HAL_TIM_ReadCapturedValue(&g_timx_cap_chy_handle,TIM_CHANNEL_4);
				TIM_RESET_CAPTUREPOLARITY(&g_timx_cap_chy_handle,TIM_CHANNEL_4);
				TIM_SET_CAPTUREPOLARITY(&g_timx_cap_chy_handle,TIM_CHANNEL_4,TIM_ICPOLARITY_RISING);
			}
			else
			{
				g_timxchy_cap_sta = 0;
				g_timxchy_cap_val = 0;
				g_timxchy_cap_sta |= 0x40;
				__HAL_TIM_DISABLE(&g_timx_cap_chy_handle);
				__HAL_TIM_SET_COUNTER(&g_timx_cap_chy_handle,0);
				TIM_RESET_CAPTUREPOLARITY(&g_timx_cap_chy_handle,TIM_CHANNEL_4);
				TIM_SET_CAPTUREPOLARITY(&g_timx_cap_chy_handle,TIM_CHANNEL_4,TIM_ICPOLARITY_FALLING);
				__HAL_TIM_ENABLE(&g_timx_cap_chy_handle);
			}
		}
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM2)
	{
		if((g_timxchy_cap_sta & 0x80) == 0)
		{
			if(g_timxchy_cap_sta & 0x40)
			{
				if((g_timxchy_cap_sta & 0x3F) == 0x3F)
				{
					TIM_RESET_CAPTUREPOLARITY(&g_timx_cap_chy_handle,TIM_CHANNEL_4);
					TIM_SET_CAPTUREPOLARITY(&g_timx_cap_chy_handle,TIM_CHANNEL_4,TIM_ICPOLARITY_RISING);
					g_timxchy_cap_sta |= 0x80;
					g_timxchy_cap_val = 0xFFFF;
				}
				else
				{
					g_timxchy_cap_sta++;
				}
			}
		}
	}
}


/******************************************************************************
* 测试代码示例说明：
******************************************************************************/
#if 0

#endif
