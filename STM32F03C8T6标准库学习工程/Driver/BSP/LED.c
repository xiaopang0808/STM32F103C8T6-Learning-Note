#include "stm32f10x.h"                  // Device header
#include "LED.h"

void LED_Init(void)
{
	 GPIO_InitTypeDef gpio_init_struct;
	/* LED时钟使能 */
	LED0_GPIO_CLK_ENABLE();
	LED1_GPIO_CLK_ENABLE();
	
	gpio_init_struct.GPIO_Pin=LED0_GPIO_PIN;		/* LED0引脚 */
	gpio_init_struct.GPIO_Mode=GPIO_Mode_Out_PP;	/* 推挽输出 */
	gpio_init_struct.GPIO_Speed=GPIO_Speed_50MHz;	/* 高速 */
	GPIO_Init(LED0_GPIO_PORT,&gpio_init_struct);	/* 初始化LED0引脚 */
	
	gpio_init_struct.GPIO_Pin=LED1_GPIO_PIN;		/* LED1引脚 */
	GPIO_Init(LED0_GPIO_PORT,&gpio_init_struct);	/* 初始化LED1引脚 */
	
	/* LED采用共阳极接法 */
	LED0(1);	/* 关闭LED0 */
	LED1(1);	/* 关闭LED1 */

}


//#include "stm32f10x.h"                  // Device header
//#include "Delay.h"
//#include "LED.h"

//int main()
//{
//	LED_Init();
//    
//    while(1)
//    {
//        LED0(0);
//		LED1(1);
//		Delay_ms(500);
//		LED0(1);
//		LED1(0);
//		Delay_ms(500);
//    }
//    return 0;
//}
