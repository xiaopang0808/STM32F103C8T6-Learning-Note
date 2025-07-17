#include "stm32f10x.h"                  // Device header
#include "PWM.h"

/* 电机驱动引脚定义 */
#define left1  GPIO_Pin_7                // PA7: 左电机方向控制引脚1
#define left2  GPIO_Pin_6                // PA6: 左电机方向控制引脚2
#define right1 GPIO_Pin_5                // PA5: 右电机方向控制引脚1
#define right2 GPIO_Pin_4                // PA4: 右电机方向控制引脚2

int8_t Car_Speed;                        // 全局速度变量，范围建议0~100

/**
  * @brief  初始化直流电机的GPIO和PWM配置
  * @param  无
  * @retval 无
  * @note   使能GPIOA时钟，配置PA4-PA7为推挽输出，调用PWM初始化函数
  */
  
void Motor_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = left1 | left2 | right1 | right2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    PWM_Init();                             // 初始化PWM输出
}

/**
  * @brief  设置左电机速度和方向
  * @param  Speed: 速度值(-100~100)，正数为正转，负数为反转，0为刹车
  * @retval 无
  * @note   正转逻辑: left1=1, left2=0
  *         反转逻辑: left1=0, left2=1
  *         刹车逻辑: left1=1, left2=1
  */
void Motor_SetLeftSpeed(int8_t Speed)
{
    if (Speed > 0) {
        GPIO_SetBits(GPIOA, left1);        // 正转
        GPIO_ResetBits(GPIOA, left2);
        PWM_SetCompare3(Speed);            // 设置PWM占空比
    } else if (Speed == 0) {
        GPIO_SetBits(GPIOA, left1);        // 刹车
        GPIO_SetBits(GPIOA, left2);
        PWM_SetCompare3(0);                // PWM输出关闭
    } else {
        GPIO_ResetBits(GPIOA, left1);      // 反转
        GPIO_SetBits(GPIOA, left2);
        PWM_SetCompare3(-Speed);           // 取绝对值设置PWM
    }
}

/**
  * @brief  设置右电机速度和方向（逻辑同左电机）
  * @param  Speed: 速度值(-100~100)
  * @retval 无
  * @note   方向控制引脚为right1/right2
  */
void Motor_SetRightSpeed(int8_t Speed)
{
    if (Speed > 0) {
        GPIO_SetBits(GPIOA, right1);       // 正转
        GPIO_ResetBits(GPIOA, right2);
        PWM_SetCompare3(Speed);
    } else if (Speed == 0) {
        GPIO_SetBits(GPIOA, right1);       // 刹车
        GPIO_SetBits(GPIOA, right2);
        PWM_SetCompare3(0);
    } else {
        GPIO_ResetBits(GPIOA, right1);     // 反转
        GPIO_SetBits(GPIOA, right2);
        PWM_SetCompare3(-Speed);
    }
}

/**​
  * @brief  控制小车前进
  * @param  无
  * @retval 无
  * @note   左右电机同时以Car_Speed正向转动
  */
void MoveForward(void)
{
    Motor_SetLeftSpeed(Car_Speed);
    Motor_SetRightSpeed(Car_Speed);
}

/**
  * @brief  控制小车后退
  * @param  无
  * @retval 无
  * @note   左右电机同时以Car_Speed反向转动
  */
void MoveBackward(void)
{
    Motor_SetLeftSpeed(-Car_Speed);
    Motor_SetRightSpeed(-Car_Speed);
}

/**
  * @brief  左转向（单侧驱动）
  * @param  无
  * @retval 无
  * @note   仅右电机工作，适合差速转向场景
  * @warning 需配合停止/反向操作实现完整转向
  */
void Turn_Left(void)
{
    Motor_SetRightSpeed(Car_Speed);        // 右电机驱动
}

/**
  * @brief  右转向（单侧驱动）
  * @param  无
  * @retval 无
  * @note   仅左电机工作
  */
void Turn_Right(void)
{
    Motor_SetLeftSpeed(Car_Speed);         // 左电机驱动
}

/**
  * @brief  紧急停止
  * @param  无
  * @retval 无
  * @note   左右电机PWM置零并进入刹车模式
  */
void Car_Stop(void)
{
    Motor_SetLeftSpeed(0);
    Motor_SetRightSpeed(0);
}

/**
  * @brief  原地右旋转
  * @param  无
  * @retval 无
  * @note   左电机正转，右电机反转实现原地转向
  */
void Self_Right(void)
{
    Motor_SetLeftSpeed(Car_Speed);
    Motor_SetRightSpeed(-Car_Speed);
}

/**​
  * @brief  原地左旋转
  * @param  无
  * @retval 无
  * @note   左电机反转，右电机正转
  */
void Self_Left(void)
{
    Motor_SetLeftSpeed(-Car_Speed);
    Motor_SetRightSpeed(Car_Speed);
}
/**
  * @brief  设置全局速度基准值
  * @param  speed: 新的速度值(0~100)
  * @retval 无
  * @warning 输入值超过范围可能导致异常
  */
void Set_Car_Speed(int8_t speed)
{
    Car_Speed = speed;
}
