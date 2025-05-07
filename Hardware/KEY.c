#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#define KEY1 GPIO_Pin_5


void KEY_Init(void)//初始化按键
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;       //上拉输入，端口电平通过按键输入
	GPIO_InitStructure.GPIO_Pin = KEY1;          
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

int Key_GetNum(void)
{
	int key_num = 0;
	
	if((GPIO_ReadInputDataBit(GPIOB, KEY1)) == 0)//按键按下
	{
		Delay_ms(20);//按下时按键内部弹簧片抖动，设置延迟消抖，防止读取端口电平时不稳定
		while((GPIO_ReadInputDataBit(GPIOB, KEY1)) == 0);//按键一直按下不松开
		//若没有此while语句，则用户还没松开按键，程序已经继续执行下去，程序现象变乱
		Delay_ms(20);//松开按键时消抖
		key_num = 1;//表示按下第一个按键
	}
	return key_num;
}
