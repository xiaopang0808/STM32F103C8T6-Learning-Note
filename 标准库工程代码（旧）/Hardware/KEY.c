#include "stm32f10x.h"                  // Device header
#include "Delay.h"

#define KEY_GPIO_PORT	GPIOB
#define KEY_GPIO_PIN		GPIO_Pin_13



#define LONG_PRESS_TIME  1000  // 长按时间1000ms
#define DOUBLE_CLICK_TIME 300  // 双击间隔时间300ms
#define SCAN_DELAY       5     // 扫描延时5ms

// 按键事件枚举
typedef enum {
    KEY_EVENT_NONE = 0,    // 无事件
    KEY_EVENT_SHORT,       // 短按
    KEY_EVENT_LONG,        // 长按
    KEY_EVENT_DOUBLE       // 双击
} key_event_t;

// 函数声明
void Delay_ms(unsigned int ms);
key_event_t Key_Scan_Loop(void);
void KEY_Init(void);	//初始化按键




///**
// * @brief 主函数
// */
//int main(void)
//{
//	
//    while(1)
//    {
//        key_event_t event = Key_Scan_Loop();
//        
//        switch(event)
//        {
//            case KEY_EVENT_SHORT:
//                break;
//                
//            case KEY_EVENT_LONG:
//                break;
//                
//            case KEY_EVENT_DOUBLE:
//                break;
//                
//            default:
//                break;
//        }

//        Delay_ms(SCAN_DELAY);
//    }
//    
//    return 0;
//}

void KEY_Init(void)//初始化按键
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;       //上拉输入，端口电平通过按键输入
	GPIO_InitStructure.GPIO_Pin = KEY_GPIO_PIN;          
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

/**
 * @brief 循环扫描按键函数
 * @return 按键事件类型
 */
key_event_t Key_Scan_Loop(void)
{
	key_event_t event = KEY_EVENT_NONE;
	uint16_t key_status = 0;
    uint16_t click_count = 0;
	if(key_status == 0){
		if(GPIO_ReadInputDataBit(KEY_GPIO_PORT, KEY_GPIO_PIN) == RESET){
			Delay_ms(SCAN_DELAY);
			if(GPIO_ReadInputDataBit(KEY_GPIO_PORT, KEY_GPIO_PIN) == RESET){
				event = KEY_EVENT_SHORT;
			}
			
		}
	}
	Delay_ms(DOUBLE_CLICK_TIME);
	if(GPIO_ReadInputDataBit(KEY_GPIO_PORT, KEY_GPIO_PIN) == RESET){
		Delay_ms(SCAN_DELAY);
		if(GPIO_ReadInputDataBit(KEY_GPIO_PORT, KEY_GPIO_PIN) == RESET){
			event = KEY_EVENT_DOUBLE;
		}
	}
	Delay_ms(LONG_PRESS_TIME-DOUBLE_CLICK_TIME);
	if(GPIO_ReadInputDataBit(KEY_GPIO_PORT, KEY_GPIO_PIN) == RESET){
		Delay_ms(SCAN_DELAY);
		if(GPIO_ReadInputDataBit(KEY_GPIO_PORT, KEY_GPIO_PIN) == RESET){
			event = KEY_EVENT_LONG;
		}
	}
	return event;


}
