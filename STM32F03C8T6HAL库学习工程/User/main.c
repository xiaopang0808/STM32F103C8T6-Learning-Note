#include "stm32f1xx.h"
#include "sys.h"
#include "delay.h"
#include "LED.h"
#include "OLED.h"
#include "Key.h"


int main()
{
    HAL_Init();
    sys_stm32_clock_init(RCC_PLL_MUL9);		/* 初始化系统时钟 */
	OLED_Init();
	LED_Init();
	Key0_Init();
	Key1_Init();
	  
    while(1)
    {
		if(Key0_scan())
		{
			LED0_TOGGLE();
			OLED_ShowString(1,1,"KEY0 DOWN");
			Delay_ms(500);
			OLED_ShowString(1,1,"         ");
		}
		
	}
}
