#include "stm32f10x.h"                  // STM32标准库
#include "Delay.h"                      // 延时函数
#include "Motor.h"                      // 电机驱动
#include "Servo.h"                      // 舵机控制
#include <math.h>                       // 数学库（用于pow函数）
#include <stdlib.h>                     // 标准库（用于abs函数）

/* 超声波模块硬件定义 */
#define Trig GPIO_Pin_12                // PB12-超声波触发引脚
#define Echo GPIO_Pin_13                // PB13-超声波回波引脚

/* 避障参数配置 */
#define SCAN_STEP     30    // 舵机角度扫描步长(0°~180°每次增加30°)
#define MIN_DISTANCE  30    // 触发避障的最小安全距离(单位：cm)
#define SAFE_ANGLE    90    // 舵机安全回中角度

/*
 * 函数名称：UltraSound_Init
 * 功能说明：初始化超声波模块相关硬件
 * 参数说明：无
 * 返回值：无
 * 备注：配置Trig为输出，Echo为输入，初始化TIM4用于高精度计时
*/
void UltraSound_Init(void) {
    /* 启用TIM4和GPIOB的时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		
    
    /* 配置Trig引脚为推挽输出模式 */
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = Trig;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);	
    
    /* 配置Echo引脚为下拉输入模式（避免悬空） */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_InitStructure.GPIO_Pin = Echo;
    GPIO_Init(GPIOB, &GPIO_InitStructure);	
    
    /* 配置TIM4为向上计数模式，72MHz主频下：
       - 预分频72-1 => 1MHz计数频率（每计数1次=1us）
       - 周期60000-1 => 最大测量时间60ms（对应约10米） */
    TIM_InternalClockConfig(TIM4);
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 60000 - 1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);
}

/*
 * 函数名称：UltraSound_Measure
 * 功能说明：执行一次超声波测距
 * 参数说明：无
 * 返回值：float-测量距离（单位：厘米）
 * 备注：通过Trig触发测量，TIM4捕获Echo高电平时间计算距离
*/
float UltraSound_Measure(void) {
    float cnt, distance;
    
    /* 发送10us高电平触发信号 */
    GPIO_SetBits(GPIOB, Trig);
    Delay_us(20);
    GPIO_ResetBits(GPIOB, Trig);
    
    /* 等待Echo引脚变高并启动计时 */
    while (GPIO_ReadInputDataBit(GPIOB, Echo) == RESET); // 等待上升沿
    TIM_Cmd(TIM4, ENABLE);
    
    /* 等待Echo引脚变低并停止计时 */
    while (GPIO_ReadInputDataBit(GPIOB, Echo) == SET);   // 等待下降沿
    TIM_Cmd(TIM4, DISABLE);
    
    /* 计算高电平持续时间（单位：us） */
    cnt = TIM_GetCounter(TIM4);
    distance = (cnt * 0.034) / 2;       // 计算距离（声速340m/s=0.034cm/us）
    
    /* 重置计时器并返回结果 */
    TIM4->CNT = 0;
    Delay_ms(100);                      // 防止传感器频繁触发
    return distance;
}

/*
 * 函数名称：map
 * 功能说明：带非线性补偿的数值映射函数
 * 参数说明：
 *   x        : 输入值
 *   in_min   : 输入下限
 *   in_max   : 输入上限
 *   out_min  : 输出下限
 *   out_max  : 输出上限
 * 返回值：int-映射后的PWM脉冲宽度（500-1500us）
 * 备注：通过二次曲线补偿增强小角度转向的响应速度
 */
int map(int x, int in_min, int in_max, int out_min, int out_max) {
    /* 基础线性映射 */
    int linear = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    
    /* 非线性补偿（小角度时增加转向时间权重） */
    float compensation = 1.0 + pow((float)(abs(x - in_min)) / (in_max - in_min), 2);
    
    /* 约束输出在舵机有效范围内 */
    int result = (int)(linear * compensation);
    return (result < 500) ? 500 : (result > 1500) ? 1500 : result;
}

/*
 * 函数名称：ObstacleAvoidance
 * 功能说明：执行全自动避障决策流程
 * 参数说明：无
 * 返回值：无
 * 备注：包含多角度扫描、最优路径选择和转向策略
 */
void ObstacleAvoidance(void) {
    float max_distance = 0;            // 记录最大距离
    int optimal_angle = SAFE_ANGLE;     // 最优避障方向
    
    /* 多角度扫描（0°~180°，步长30°） */
    for (int angle = 0; angle <= 180; angle += SCAN_STEP) {
        Servo_SetAngle(angle);          // 转动舵机到指定角度
        Delay_ms(300);                  // 等待舵机稳定
        float current_dist = UltraSound_Measure();
        
        /* 更新最大距离和对应角度 */
        if (current_dist > max_distance) {
            max_distance = current_dist;
            optimal_angle = angle;
        }
    }
    
    Servo_SetAngle(SAFE_ANGLE);         // 扫描完成回中
    Delay_ms(300);                      // 等待舵机归位
    
    /* 避障决策执行 */
    if (max_distance < MIN_DISTANCE) {
        /* 所有方向均受阻：后退并右转调头 */
        MoveBackward();
        Delay_ms(800);                  // 持续后退800ms
        Self_Right();                   // 原地右转（调头）
        Delay_ms(500);                  // 持续右转500ms
    } else {
        /* 根据最优角度转向 */
        if (optimal_angle < SAFE_ANGLE) {
            // 右侧区域：动态调整右转时间（0°~90°映射700ms~300ms）
            Turn_Right();
            Delay_ms(map(optimal_angle, 0, SAFE_ANGLE, 700, 300));
        } else if (optimal_angle > SAFE_ANGLE) {
            // 左侧区域：动态调整左转时间（90°~180°映射300ms~700ms）
            Turn_Left();
            Delay_ms(map(optimal_angle, SAFE_ANGLE, 180, 300, 700));
        } else {
            // 正前方无障碍：直行
            MoveForward();
        }
    }
}
