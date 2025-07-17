#include "stm32f10x.h"                  // Device header

#define Buzzer GPIO_Pin_1



void Buzzer_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = Buzzer;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA,Buzzer);
}

void Buzzer_ON(void)
{
	GPIO_ResetBits(GPIOA, Buzzer);          //低电平触发
}

void Buzzer_OFF(void)
{
	GPIO_SetBits(GPIOA, Buzzer);
}
	
void Buzzer_Turn(void)
{
	if((GPIO_ReadOutputDataBit(GPIOA, Buzzer)) == 0)
	{
		GPIO_SetBits(GPIOA, Buzzer);
	}
	else
	{
		GPIO_ResetBits(GPIOA, Buzzer);
	}
}

