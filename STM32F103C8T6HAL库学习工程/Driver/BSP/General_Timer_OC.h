/******************************************************************************
* 文件名：General_Timer_OC.h
* 作　者：Xiaopang
* 创建日期：2025-08-19
* 最后修改：2025-08-19
* 版　本：v1.0
* 功能描述：
*   - 通用定时器(TIM3) PWM输出头文件
*   - 声明外部可用的PWM配置函数和句柄
*
* 修改记录：
*  版本   日期       修改人     修改内容
*  v1.0  2025-08-19  Xiaopang   初始版本创建
*
* 注意事项：
*   - 需要配合HAL库使用
*   - 仅支持TIM3通道2配置
******************************************************************************/

#ifndef __GENERAL_TIMER_OC_H
#define __GENERAL_TIMER_OC_H

#include "sys.h"
	
/* 外部全局变量声明 ----------------------------------------------------------*/
extern TIM_HandleTypeDef g_timx_pwm_chy_handle;  // 定时器PWM句柄(在gtim.c中定义)

/******************** 外部接口函数声明 ********************/
/**
  * @brief  PWM初始化函数声明
  * @param  psc: 预分频系数
  *         arr: 自动重装载值
  */
void gtim_timx_pwm_chy_oc_init(uint16_t psc, uint16_t arr);

#endif /* __GENERAL_TIMER_OC_H */
