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

#include "General_Timer_OC.h"

/* 全局定时器PWM句柄 */
TIM_HandleTypeDef g_timx_pwm_chy_handle;  // TIM3 PWM配置句柄

/**
  * @brief  通用定时器PWM初始化函数
  * @param  arr: 自动重装载值(决定PWM频率)
  *         psc: 预分频系数
  * @note   配置TIM3通道2输出PWM
  *         示例：gtim_timx_pwm_chy_init(100-1,7200-1)
  *              PWM频率 = 72MHz / (7200 * 100) = 100Hz
  */
void gtim_timx_pwm_chy_oc_init(uint16_t arr, uint16_t psc)
{
    TIM_OC_InitTypeDef timx_oc_pwm_chy = {0};
    
    /* 基础定时器配置 */
    g_timx_pwm_chy_handle.Instance = TIM3;              // 使用TIM3
    g_timx_pwm_chy_handle.Init.Prescaler = psc;         // 预分频系数
    g_timx_pwm_chy_handle.Init.Period = arr;            // 自动重装载值
    g_timx_pwm_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;      // 向上计数
    g_timx_pwm_chy_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; // 时钟分频
    g_timx_pwm_chy_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE; // 禁止自动重载预装载
    HAL_TIM_PWM_Init(&g_timx_pwm_chy_handle);           // 初始化PWM
    
    /* 通道2输出配置 */
    timx_oc_pwm_chy.OCMode = TIM_OCMODE_PWM1;           // PWM模式1
    timx_oc_pwm_chy.Pulse = arr/2;                      // 默认占空比50%
    timx_oc_pwm_chy.OCPolarity = TIM_OCPOLARITY_LOW;    // 输出极性：低电平有效
    timx_oc_pwm_chy.OCFastMode = TIM_OCFAST_DISABLE;    // 关闭快速模式
    timx_oc_pwm_chy.OCNPolarity = TIM_OCNPOLARITY_HIGH; // 互补通道极性
    timx_oc_pwm_chy.OCNIdleState = TIM_OCNIDLESTATE_RESET; // 空闲状态
    timx_oc_pwm_chy.OCIdleState = TIM_OCIDLESTATE_RESET;    // 空闲状态
    HAL_TIM_PWM_ConfigChannel(&g_timx_pwm_chy_handle, &timx_oc_pwm_chy, TIM_CHANNEL_2); // 配置通道
    HAL_TIM_PWM_Start(&g_timx_pwm_chy_handle, TIM_CHANNEL_2); // 启动PWM输出
}

/**
  * @brief  PWM硬件初始化回调函数(HAL库内部调用)
  * @param  htim: TIM句柄
  * @note   使能TIM3和GPIOA时钟
  *         PA7配置为复用推挽输出模式
  */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM3)
	{
		GPIO_InitTypeDef gpio_init_struct;
        
        /* 时钟使能 */
        __HAL_RCC_TIM3_CLK_ENABLE();       // TIM3时钟使能
        __HAL_RCC_GPIOA_CLK_ENABLE();      // GPIOA时钟使能
        __HAL_RCC_AFIO_CLK_ENABLE();       // 复用功能时钟使能
			
        /* PA7配置：TIM3通道2复用输出 */
		gpio_init_struct.Pin = GPIO_PIN_7;                // PA7
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;           // 复用推挽输出
        gpio_init_struct.Pull = GPIO_PULLUP;               // 上拉模式
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;     // 高速输出
        HAL_GPIO_Init(GPIOA, &gpio_init_struct);
	}
}

/******************************************************************************
* 测试代码示例说明：
*   - 实现呼吸灯效果
*   - 通过改变比较值实现PWM占空比渐变
*   - 占空比范围: 0~99
*   - 频率: 100Hz (72000000/(7200 * 100))
******************************************************************************/
#if 0
#include "stm32f1xx.h"
#include "sys.h"
#include "delay.h"
#include "General_Timer_OC.h"
int main()
{
	uint16_t ledpwmval = 0;    // PWM比较值
	uint8_t dir = 1;           // 方向标志
    HAL_Init();
    sys_stm32_clock_init(RCC_PLL_MUL9);	// 初始化系统时钟为72MHz
	gtim_timx_pwm_chy_oc_init(100-1,7200-1); // 100Hz PWM
    while(1)
    {
		Delay_ms(10);     // 10ms延时
		if(dir) ledpwmval++;
		else ledpwmval--;
		
		if(ledpwmval > 99) dir = 0;   // 达到最大值反转
		if(ledpwmval == 0) dir = 1;   // 达到最小值反转
		
		__HAL_TIM_SET_COMPARE(&g_timx_pwm_chy_handle, TIM_CHANNEL_2, ledpwmval);
	}
}
#endif
