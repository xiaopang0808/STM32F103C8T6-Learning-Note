/******************************************************************************
* 文件名：IWDG.c
* 作　者：Xiaopang
* 创建日期：2025-07-28
* 最后修改：2025-07-28
* 版　本：v1.0
* 功能描述：
*
*   本文件实现独立看门狗(IWDG)硬件控制模块，主要功能包括：
*    1. 初始化看门狗定时器
*    2. 提供系统故障检测和自动复位功能
*    3. 实现喂狗操作
*
* 修改记录：
*  版本   日期       修改人     修改内容
*  v1.0  2025-07-28  Xiaopang   初始版本创建
*
* 注意事项：
*  1. LSI时钟频率约40kHz（具体需参考芯片手册）
*  2. 重载值必须小于0xFFF
*  3. 启用后必须在超时时间内持续喂狗
*
******************************************************************************/

#include "IWDG.H"

/* 看门狗句柄声明 */
IWDG_HandleTypeDef g_iwdg_handle;  // 全局看门狗实例句柄

/*********************************************************************/
/* 函数实现 */

/**
  * @brief  独立看门狗初始化
  * @param  prer: 预分频系数 @ref IWDG_Prescaler_Definitions
  *         可选值: 
  *           IWDG_PRESCALER_4   (0x00)   // 4分频
  *           IWDG_PRESCALER_8   (0x01)   // 8分频
  *           IWDG_PRESCALER_16  (0x02)   // 16分频
  *           IWDG_PRESCALER_32  (0x03)   // 32分频
  *           IWDG_PRESCALER_64  (0x04)   // 64分频
  *           IWDG_PRESCALER_128 (0x05)   // 128分频
  *           IWDG_PRESCALER_256 (0x06)   // 256分频
  * @param  rlr: 看门狗重装载值 (0-0x0FFF)
  * @retval 无
  *
  * 功能说明：
  *  1. 配置看门狗预分频系数
  *  2. 设置计数器重载值
  *  3. 激活独立看门狗
  *  4. 计算说明：超时时间(ms) = (4 * 2^prer) * rlr / LSI_freq * 1000
  *     当LSI=40kHz时，IWDG_PRESCALER_32+rlr=1250 ≈ 1000ms
  */
void iwdg_init(uint8_t prer, uint16_t rlr)
{
    /* 配置看门狗实例参数 */
    g_iwdg_handle.Instance = IWDG;                      // 绑定IWDG外设
    g_iwdg_handle.Init.Prescaler = prer;                // 设置预分频系数
    g_iwdg_handle.Init.Reload = rlr;                    // 设置重装载值
    HAL_IWDG_Init(&g_iwdg_handle);                      // 初始化看门狗
}

/**
  * @brief  喂狗(重载看门狗计数器)
  * @note   调用此函数将计数器重置为重装载值
  *         必须定期调用防止系统复位
  */
void iwdg_feed(void)
{
    /* 刷新看门狗计数器 */
    HAL_IWDG_Refresh(&g_iwdg_handle);  // 重置计数器防止超时复位
}


/******************************************************************************
* 测试代码示例
******************************************************************************/
#if 0
#include "stm32f1xx.h"
#include "sys.h"
#include "delay.h"
#include "OLED.h"
#include "IWDG.h"

/**
  * @brief  主函数 - 看门狗演示程序
  * @note   1秒喂狗一次，OLED显示状态
  */
int main()
{
    /* 系统初始化 */
    HAL_Init();                                 // HAL库初始化
    sys_stm32_clock_init(RCC_PLL_MUL9);         // 系统时钟配置
    OLED_Init();                                // OLED显示屏初始化
    
    /* 看门狗初始化：预分频32，重载值1250，约1秒超时 */
    iwdg_init(IWDG_PRESCALER_32, 1250);         // 初始化独立看门狗

    /* 主循环 - 交替显示状态并喂狗 */
    while(1)
    {
        OLED_ShowString(1, 1, "Hungry");       // 第1行显示饥饿状态
        Delay_ms(500);                         // 延时500ms
        
        iwdg_feed();                           // 喂狗操作(必须在1秒内执行)
        OLED_ShowString(2, 1, "Feed");         // 第2行显示喂狗状态
    }
}
#endif
/******************************************************************************/
