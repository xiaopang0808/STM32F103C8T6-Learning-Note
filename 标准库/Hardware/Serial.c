#include "stm32f10x.h"                  // Device header
#include "stdio.h"
#include "stdarg.h"


uint8_t Serial_TxPacket[4];
uint8_t Serial_RxPacket[4];
uint8_t Serial_RxFlag;



void Serial_Init(void){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
    
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate=9600;            //波特率
    USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;   //硬件流控制
    USART_InitStructure.USART_Mode=USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStructure.USART_Parity=USART_Parity_No;           //校验位
    USART_InitStructure.USART_StopBits=USART_StopBits_1;        //停止位长度
    USART_InitStructure.USART_WordLength=USART_WordLength_8b;   //字长
    USART_Init(USART1,&USART_InitStructure);
    
    USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;      
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;       
    NVIC_Init(&NVIC_InitStructure);
    
    USART_Cmd(USART1,ENABLE);
}

void Serial_SendByte(uint8_t Byte){
    USART_SendData(USART1,Byte);
    while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
    
}

void Serial_SendArray(uint8_t *Array,uint16_t Length){
    for(uint16_t i=0;i<Length;i++){
        Serial_SendByte(Array[i]);
    }
}

void Serial_SendString(char *String){
    for(uint16_t i=0;String[i]!='\0';i++){
        Serial_SendByte(String[i]);
    }
}

uint32_t Pow(uint32_t x,uint32_t y){
    uint32_t result=1;
    while(y--){
        result*=x;
    }
    return result;
}

void Serial_SendNumber(uint32_t Number,uint8_t Length){
    for(uint8_t i=0;i<Length;i++){
        Serial_SendByte(Number/Pow(10,Length-i-1)%10+'0');
    }
}

int fputc(int ch,FILE *f){
    Serial_SendByte(ch);
    return ch;
}

void Serial_printf(char *format,...){
    char String[100];
    va_list arg;
    va_start(arg,format);
    vsprintf(String,format,arg);
    va_end(arg);
    Serial_SendString(String);
}

void Serial_SendPacket(void){
    Serial_SendByte(0xFF);
    Serial_SendArray(Serial_TxPacket,4);
    Serial_SendByte(0xFE);
}

uint8_t Serial_GetRXFlag(void){
    if(Serial_RxFlag==1){
        Serial_RxFlag=0;
        return 1;
    }
    return 0;
    
}

void USART1_IRQHandler(void)
{
    static uint8_t RxState=0;
    static uint8_t pRxPacket=0;
    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET){       //判断是否是USART1的接收事件触发的中断
        uint8_t RxData=USART_ReceiveData(USART1);
        if(RxState==0){
            if(RxData==0xFF){
                RxState=1;
                pRxPacket=0;
            }
        }
        else if(RxState==1){
            Serial_RxPacket[pRxPacket++]=RxData;
            if(pRxPacket>=4){
                RxState=2;
            }
        }
        else if(RxState==2){
            if(RxData==0xFE){
                RxState=0;
                Serial_RxFlag=1;
            }
        }
        
    }
}
 
