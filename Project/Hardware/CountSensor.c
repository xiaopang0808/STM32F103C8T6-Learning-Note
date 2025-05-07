#include "stm32f10x.h"                  // Device header
//红外对射式传感器
uint16_t CountSensor_Count;
#define CountPin GPIO_Pin_10

void CountSensor_Init(void){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);        //打开GPIOB的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);         //打开AFIO的时钟
    
    //配置GPIO 
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;     //由参考手册得知：浮空，上拉，下拉输入皆可
    GPIO_InitStructure.GPIO_Pin=CountPin;           
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
    
    //配置AFIO
    //将 GPIOB 的第 10 号引脚（PB10）配置为外部中断（EXTI）的触发源
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource10);
    
    //配置EXTI
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line=EXTI_Line10 ;      //10号线路
    EXTI_InitStructure.EXTI_LineCmd=ENABLE ;        //使能
    EXTI_InitStructure.EXTI_Mode= EXTI_Mode_Interrupt;      //中断模式
    EXTI_InitStructure.EXTI_Trigger= EXTI_Trigger_Rising;  //下降沿触发
    EXTI_Init(&EXTI_InitStructure);
    
    //NVIC中断优先级分组
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel=EXTI15_10_IRQn;      //指定通道，14通道在15——10的合并通道里
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1; //指定抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;        //指定相应优先级
    NVIC_Init(&NVIC_InitStructure);

}

uint16_t CountSensor_Get(void){
    return CountSensor_Count;           //返回计数值
}

//中断函数，变量名不可自定义，函数名选取于Start文件夹startup_...文件
//中断函数不需要在.h文件处声明，触发中断时会自动跳转进中断函数
void EXTI15_10_IRQHandler(void){        
    if(EXTI_GetITStatus(EXTI_Line10)==SET){
        CountSensor_Count++;
        EXTI_ClearITPendingBit(EXTI_Line10);
    }
}






