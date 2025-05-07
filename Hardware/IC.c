#include "stm32f10x.h"                  // Device header

//输入捕获

void IC_Init(void){
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);     //开启TIM3时钟    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);    //打开GPIO时钟
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;   //复用推挽输出
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    
    /*
        配置TIM2使用内部时钟
        如果不调用此函数，也默认使用内部时钟
    */
    TIM_InternalClockConfig(TIM3);   
    
    //初始化时基单元
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;   //时钟分频，对时基单元影响不大
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;   //计数模式 
    /*
        参考公式：计数器溢出频率=时钟源频率/（PSC+1）/（ARR+1）
    */
    TIM_TimeBaseInitStructure.TIM_Period=65536-1;       //自动重装载值ARR
    TIM_TimeBaseInitStructure.TIM_Prescaler=72-1;     //预分频器值PSC  
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0; /*重复计数器值，高级定时器才有，
                                                        此处为通用定时器，不需要用到，设置为0*/
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);  
    
    //配置捕获通道
    TIM_ICInitTypeDef TIM_ICInitStructure;
    TIM_ICInitStructure.TIM_Channel=TIM_Channel_1;      //输入捕获通道
    TIM_ICInitStructure.TIM_ICFilter=0xF;               //滤波器
    TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;   //极性
    TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;         //分频器
    TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;   //输入通道，直连通道
//    TIM_ICInit(TIM3,&TIM_ICInitStructure);
    //配置第二通道，实现PWMI模式，普通IC可去掉
    //第一种做法
    TIM_PWMIConfig(TIM3,&TIM_ICInitStructure);  //需要去掉TIM_ICInit
    //第二种做法
    
//    TIM_ICInitStructure.TIM_Channel=TIM_Channel_2;      //输入捕获通道
//    TIM_ICInitStructure.TIM_ICFilter=0xF;               //滤波器
//    TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Falling;   //极性
//    TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;         //分频器
//    TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_IndirectTI;   //输入通道，直连通道
//    TIM_ICInit(TIM3,&TIM_ICInitStructure); 
    //主从模式配置
    TIM_SelectInputTrigger(TIM3,TIM_TS_TI1FP1);     //触发源
    TIM_SelectSlaveMode(TIM3,TIM_SlaveMode_Reset);  //从模式
    
    //启动定时器
    TIM_Cmd(TIM3,ENABLE);
    
}
    
//获取输入捕获频率
int IC_GetFreq(void){
    return 1000000/TIM_GetCapture1(TIM3);
}

//获取占空比
int IC_GetDuty(void){
    return TIM_GetCapture2(TIM3)*100/TIM_GetCapture1(TIM3);
}
