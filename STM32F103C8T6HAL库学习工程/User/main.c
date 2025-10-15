#include "stm32f1xx.h"
#include "stdio.h"
#include "sys.h"
#include "delay.h"
#include "LED.h"
#include "OLED.h"
#include "General_Timer_IC.h"
#include "General_Timer_OC.h"

int main()
{
	uint32_t temp = 0;
    HAL_Init();
    sys_stm32_clock_init(RCC_PLL_MUL9);		/* 初始化系统时钟 */
	OLED_Init();
//	LED_Init();
	gtim_timx_cap_chy_ic_init(72-1,0xFFFF);
	gtim_timx_pwm_chy_oc_init(7200-1,10000-1);
    while(1)
    {
		if(g_timxchy_cap_sta & 0x80)
		{
			temp = g_timxchy_cap_sta & 0x3F;
			temp *= 65536;
			temp += g_timxchy_cap_val;
			OLED_ShowNum(1,1,temp,16);
			g_timxchy_cap_sta = 0;
		}
	}
}
