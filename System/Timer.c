#include "stm32f10x.h"                  // Device header

void Timer_Init(void){
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);     //开启TIM2时钟    
    /*
        配置TIM2使用内部时钟
        如果不调用此函数，也默认使用内部时钟
    */
    TIM_InternalClockConfig(TIM2);         
    
    //配置TIM2通过外部引脚ETR（PA0）接收外部时钟信号，作为定时器的时钟源。
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    
    TIM_ETRClockMode2Config(
    TIM2,                       // 定时器2
    TIM_ExtTRGPSC_OFF,          // 外部触发预分频器关闭（直接使用ETR信号频率）
    TIM_ExtTRGPolarity_NonInverted, // ETR信号极性不反转（上升沿有效）
    0x05                        // 滤波器关闭（不滤波）
    );
    
    
    
    //初始化时基单元
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;   //时钟分频，对时基单元影响不大
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;   //计数模式
    
    /*
        参考公式：计数器溢出频率=时钟源频率/（PSC+1）/（ARR+1）
    */
    TIM_TimeBaseInitStructure.TIM_Period=10000-1;       //自动重装载值ARR
    TIM_TimeBaseInitStructure.TIM_Prescaler=7200-1;     //预分频器值PSC
    
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0; /*重复计数器值，高级定时器才有，
                                                        此处为通用定时器，不需要用到，设置为0*/
    TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);  /*此函数调用完后，会立即进入中断一次
                                                          所以需要手动清除中断标志位*/
    TIM_ClearFlag(TIM2,TIM_FLAG_Update);                //清除中断标志位
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);            //使能更新中断
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;      //指定通道
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2; //指定抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;        //指定相应优先级
    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(TIM2,ENABLE);       //打开定时器
}

uint16_t Timer_GetCount(void){
        return TIM_GetCounter(TIM2);
}


/*
定时中断函数
一般可以放在被调用的文件处定义，此处作为模板
当需要用到时，可直接复制使用
*/
//void TIM2_IRQHandler(void){
//    if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET){
//        
//        TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
//    }
//}
