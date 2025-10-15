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

/* 全局看门狗句柄定义 */
WWDG_HandleTypeDef g_wwdg_handle;  

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
    /* 配置看门狗参数结构体 */
    g_wwdg_handle.Instance = WWDG;             // 绑定WWDG外设
    g_wwdg_handle.Init.Counter = tr;          // 设置计数器初始值
    g_wwdg_handle.Init.EWIMode = WWDG_EWI_ENABLE; // 启用早期唤醒中断
    g_wwdg_handle.Init.Window = wr;           // 设置窗口值
    g_wwdg_handle.Init.Prescaler = fprer;     // 设置预分频系数
    HAL_WWDG_Init(&g_wwdg_handle);            // 初始化窗口看门狗
}

/*********************************************************************/
/* HAL库底层硬件初始化回调函数 */

/**
  * @brief  WWDG硬件初始化回调
  * @param  hwwdg: WWDG句柄指针
  * @note   使能WWDG时钟和配置中断优先级
  */
void HAL_WWDG_MspInit(WWDG_HandleTypeDef *hwwdg)
{
    /* 启用WWDG外设时钟 */
    __HAL_RCC_WWDG_CLK_ENABLE();
    
    /* 配置看门狗中断 */
    HAL_NVIC_SetPriority(WWDG_IRQn, 2, 3);  // 设置中断优先级
    HAL_NVIC_EnableIRQ(WWDG_IRQn);           // 使能WWDG中断
}

/*********************************************************************/
/* 中断服务函数 */

/**
  * @brief  WWDG中断服务例程
  * @note   调用HAL库中断处理函数
  */
void WWDG_IRQHanddler(void)
{
    HAL_WWDG_IRQHandler(&g_wwdg_handle);  // 处理看门狗中断
}

/**
  * @brief  WWDG早期唤醒回调函数
  * @param  hwwdg: WWDG句柄指针
  * @note   用户可在此函数中添加紧急处理逻辑
  *         通常在此进行关键数据保存或安全关机操作
  */
void HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef *hwwdg)
{
    /* 示例处理方案: */
    // 1. 保存关键数据到非易失存储器
    // 2. 系统安全关闭操作
    // 3. 记录看门狗复位事件
    
    /* 此处可添加用户自定义紧急处理代码 */
}

/******************************************************************************
* 测试代码示例 
******************************************************************************/
#if 0
#include "stm32f1xx.h"
#include "stdio.h"
#include "sys.h"
#include "delay.h"
#include "OLED.h"
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
    HAL_Init();                                     // HAL库初始化
    sys_stm32_clock_init(RCC_PLL_MUL9);             // 配置系统时钟为72MHz
    OLED_Init();                                    // OLED显示屏初始化
    LED_Init();                                     // LED指示灯初始化
    
    /* 检测系统复位源 */
    if(__HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST) != RESET) 
    {
        OLED_ShowString(2, 1, "WWDGRST");          // 显示看门狗复位标识
        __HAL_RCC_CLEAR_RESET_FLAGS();             // 清除复位标志
    }
    else 
    {
        OLED_ShowString(3, 1, "OTHERST");          // 显示其他复位原因
    }
    
    Delay_ms(500);                                 // 延时便于观察复位状态
    
    /* 初始化窗口看门狗 */
    // 计数器初值：0x7F (127)
    // 窗口值：0x5F (95) - 计数器值必须小于95时刷新
    // 预分频：8分频
    // 理论超时范围：29.13ms~58.25ms (PCLK1=36MHz时)
    wwdg_init(0x7f, 0x5f, WWDG_PRESCALER_8); 
    
    /* 主循环 - 定时喂狗操作 */
    while(1)
    {
        Delay_ms(30);                               // 30ms延时（在窗口期内）
        HAL_WWDG_Refresh(&g_wwdg_handle);           // 刷新看门狗计数器
        OLED_ShowString(1, 1, "Feed");              // 显示喂狗状态
    }
}
#endif
/******************************************************************************/