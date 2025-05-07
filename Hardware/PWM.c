#include "stm32f10x.h"                  // Device header


void PWM_Init(void){
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);     //开启TIM2时钟    
    /*
        配置TIM2使用内部时钟
        如果不调用此函数，也默认使用内部时钟
    */
    TIM_InternalClockConfig(TIM2);      

    //打开GPIO时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;   //复用推挽输出
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    
    
    
    //初始化时基单元
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;   //时钟分频，对时基单元影响不大
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;   //计数模式
    
    /*
        参考公式：计数器溢出频率=时钟源频率/（PSC+1）/（ARR+1）
    */
    TIM_TimeBaseInitStructure.TIM_Period=100-1;       //自动重装载值ARR
    TIM_TimeBaseInitStructure.TIM_Prescaler=720-1;     //预分频器值PSC  
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0; /*重复计数器值，高级定时器才有，
                                                        此处为通用定时器，不需要用到，设置为0*/
    TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);  
    
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure);    //给结构体中没有初始化的变量赋默认值，避免错误
    TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;     //输出比较模式
    TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable; //输出使能
    TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;  //极性选择 
    TIM_OCInitStructure.TIM_Pulse=0;              //CCR
    TIM_OC1Init(TIM2,&TIM_OCInitStructure);
                                                                                                                  
    TIM_Cmd(TIM2,ENABLE);       //打开定时器
    
}

//调节PWM占空比
void PWM_SetCompare1(int compare){
    TIM_SetCompare1(TIM2,compare);
}

//调节频率
void PWM_SetPrescaler(int Prescaler){
    TIM_PrescalerConfig(TIM2,Prescaler,TIM_PSCReloadMode_Immediate);
}

