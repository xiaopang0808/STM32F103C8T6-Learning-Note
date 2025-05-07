#include "stm32f10x.h"                  // Device header

#define LED1 GPIO_Pin_10


void LED_Init(void)//初始化LED灯，配置GPIO端口
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = LED1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB,LED1);
}

void LED1_ON(void)//灯点亮
{
	GPIO_SetBits(GPIOB, LED1);
}

void LED1_OFF(void)//灯熄灭
{
	GPIO_ResetBits(GPIOB, LED1);
}
	
void LED1_Turn(void)//电平翻转，高变低，低变高
{
	if((GPIO_ReadOutputDataBit(GPIOB, LED1)) == 0)
	{
		GPIO_SetBits(GPIOB, LED1);
	}
	else
	{
		GPIO_ResetBits(GPIOB, LED1);
	}
}
