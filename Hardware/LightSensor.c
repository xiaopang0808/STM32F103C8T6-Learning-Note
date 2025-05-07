#include "stm32f10x.h"                  // Device header

#define LightSensor GPIO_Pin_9

void LightSensor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = LightSensor;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

uint8_t LightSensor_Get(void){
    return GPIO_ReadInputDataBit(GPIOB,LightSensor);        //直接输出读取到的数据
}
