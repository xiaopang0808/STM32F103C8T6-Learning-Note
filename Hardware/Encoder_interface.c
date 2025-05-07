#include "stm32f10x.h"                  // Device header

//旋转编码器接口

void Encoder_interface_Init(void){
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);     //开启TIM3时钟    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);    //打开GPIO时钟
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;   
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    
      
    
    //初始化时基单元
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;   //时钟分频，对时基单元影响不大
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;   //计数模式 ，此处有编码器托管
                                                                //计数模式无效
    TIM_TimeBaseInitStructure.TIM_Period=65536-1;       //自动重装载值ARR
    TIM_TimeBaseInitStructure.TIM_Prescaler=1-1;     //预分频器值PSC  ，不分频
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0; /*重复计数器值，高级定时器才有，
                                                        此处为通用定时器，不需要用到，设置为0*/
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);  
    
    //配置捕获通道
    TIM_ICInitTypeDef TIM_ICInitStructure;
    TIM_ICStructInit(&TIM_ICInitStructure);         //为结构体赋初始值
    TIM_ICInitStructure.TIM_Channel=TIM_Channel_1;      //输入捕获通道
    TIM_ICInitStructure.TIM_ICFilter=0xF;               //滤波器
//    TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;   //极性，不代表上升沿有效
                                                                //指极性不反相，可去掉
    TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;   //输入通道，直连通道
    TIM_ICInit(TIM3,&TIM_ICInitStructure);
    
    TIM_ICInitStructure.TIM_Channel=TIM_Channel_2;      //输入捕获通道
    TIM_ICInitStructure.TIM_ICFilter=0xF;               //滤波器
//    TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;   
    TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;   //输入通道，直连通道
    TIM_ICInit(TIM3,&TIM_ICInitStructure);
    
    TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
    
    TIM_Cmd(TIM3,ENABLE);
}

int16_t Encoder_interface_Get(void){
    int16_t temp=TIM_GetCounter(TIM3);
    TIM_SetCounter(TIM3,0);         //清零计数器
    return temp;
}

    