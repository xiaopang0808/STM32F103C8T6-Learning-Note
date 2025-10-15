#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "OLED.h"
#include "Key.h"

int main()
{
	/* NVIC中断优先级分组 - 全局只进行一次分组 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	LED_Init();
	OLED_Init();
	Key0_Init();
	Key1_Init();
    while(1)
    {
	OLED_ShowString(1,1,"abc");
			
	}
    return 0;
}
