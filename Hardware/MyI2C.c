#include "stm32f10x.h"                  // Device header
#include "Delay.h"

//软件I2C

#define SCL_SDA_PORT GPIOB
#define SCL GPIO_Pin_10
#define SDA GPIO_Pin_11

void MyI2C_W_SCL(uint8_t BitValue){
    GPIO_WriteBit(SCL_SDA_PORT,SCL,(BitAction)BitValue);
    Delay_us(10);
}

void MyI2C_W_SDA(uint8_t BitValue){
    GPIO_WriteBit(SCL_SDA_PORT,SDA,(BitAction)BitValue);
    Delay_us(10);
}

uint8_t MyI2C_R_SDA(void){
    uint8_t BitValue;
    BitValue=GPIO_ReadInputDataBit(SCL_SDA_PORT,SDA);
    Delay_us(10);
    return BitValue;
}

void MyI2C_Init(void){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = SCL | SDA;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SCL_SDA_PORT,&GPIO_InitStructure);
	
	GPIO_SetBits(SCL_SDA_PORT,SCL | SDA);      //初始值置高电平
    
}

void MyI2C_Start(void){   
    MyI2C_W_SDA(1);     //先拉高SDA，确保SCL为高电平时，SDA为高电平可以产生下降沿
    MyI2C_W_SCL(1);
    MyI2C_W_SDA(0);
    MyI2C_W_SCL(0);
    
}

void MyI2C_Stop(void){
    MyI2C_W_SDA(0);     //先拉低SDA，确保SCL为高电平时，SDA为低电平可以产生上升沿
    MyI2C_W_SCL(1);
    MyI2C_W_SDA(1);
    
}

void MyI2C_SendByte(uint8_t Byte){
    for(int i=0;i<8;i++){
        MyI2C_W_SDA(Byte & (0x80>>i));
        MyI2C_W_SCL(1);
        MyI2C_W_SCL(0);
    }    
}

void MyI2C_SendAck(uint8_t AckBit){

    MyI2C_W_SDA(AckBit);
    MyI2C_W_SCL(1);
    MyI2C_W_SCL(0);
  
}


uint8_t MyI2C_ReceiveByte(void){
    uint8_t Byte=0x00;
    MyI2C_W_SDA(1);
    for(int i=0;i<8;i++){
        MyI2C_W_SCL(1);
        if(MyI2C_R_SDA()==1) {Byte|=(0x80>>i);}
        MyI2C_W_SCL(0);
    }
    return Byte;
}

uint8_t MyI2C_ReceiveAck(void){
    uint8_t AckBit;
    MyI2C_W_SDA(1);
    MyI2C_W_SCL(1);
    AckBit=MyI2C_R_SDA();
    MyI2C_W_SCL(0);
    return AckBit;
}
