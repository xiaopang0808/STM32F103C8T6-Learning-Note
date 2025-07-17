#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "OLED.h"
#include "Key.h"

int main()
{
	LED_Init();
	OLED_Init();
	Key0_Init();
	Key1_Init();
    while(1)
    {
		if(Key0_scan())
		{
			if(GPIO_ReadOutputDataBit(LED0_GPIO_PORT,LED0_GPIO_PIN))
			{
				LED0(0);
				OLED_ShowString(1,1,"LED0 ON");
			}
			else
			{
				LED0(1);
				OLED_ShowString(1,1,"LED0 OFF");
			}
		}
		
			
			
	}
    return 0;
}
