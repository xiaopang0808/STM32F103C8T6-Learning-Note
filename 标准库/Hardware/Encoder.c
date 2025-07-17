#include "stm32f10x.h"                  // Device header
//旋转编码器--外部中断
int16_t Encoder_Count;  
#define CountPin GPIO_Pin_0 | GPIO_Pin_1

void Encoder_Init(void){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);        
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);         //打开AFIO的时钟
    
    //配置GPIO 
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;     //浮空，上拉，下拉输入皆可
    GPIO_InitStructure.GPIO_Pin=CountPin;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
    
    //将 GPIOB 的第 0、1 号引脚（PB10）配置为外部中断（EXTI）的触发源
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource1);
    
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line=EXTI_Line0 | EXTI_Line1;      //10号线路
    EXTI_InitStructure.EXTI_LineCmd=ENABLE ;        
    EXTI_InitStructure.EXTI_Mode= EXTI_Mode_Interrupt;      //中断模式
    EXTI_InitStructure.EXTI_Trigger= EXTI_Trigger_Falling;  //下降沿触发
    EXTI_Init(&EXTI_InitStructure);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel=EXTI0_IRQn;      //指定通道
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel=EXTI1_IRQn;      //指定通道
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
    NVIC_Init(&NVIC_InitStructure);
}

int16_t Encoder_Get(void){
    int16_t temp;
    temp=Encoder_Count;
    Encoder_Count=0;
    return temp;
}


void EXTI0_IRQHandler(void){
    if(EXTI_GetITStatus(EXTI_Line0)==SET){
        if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0){
            Encoder_Count--;
        }
        EXTI_ClearITPendingBit(EXTI_Line0);
    }        
}

void EXTI1_IRQHandler(void){
    if(EXTI_GetITStatus(EXTI_Line1)==SET){
        if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)==0){
            Encoder_Count++;
        }
        EXTI_ClearITPendingBit(EXTI_Line1);
    }     
}

