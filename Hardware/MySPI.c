#include "stm32f10x.h"                  // Device header

#define NSS GPIO_Pin_4
#define SCK GPIO_Pin_5
#define MISO GPIO_Pin_6
#define MOSI GPIO_Pin_7

//注释部分代码为软件模拟SPI


//void MySPI_W_SCK(uint8_t BitValue){
//    GPIO_WriteBit(GPIOA,SCK,(BitAction)BitValue);  
//}

//void MySPI_W_MOSI(uint8_t BitValue){
//    GPIO_WriteBit(GPIOA,MOSI,(BitAction)BitValue);  
//}

//uint8_t MySPI_R_MISO(void){
//    return GPIO_ReadInputDataBit(GPIOA,MISO);
//}

//软硬件都使用普通GPIO作为SS引脚
void MySPI_W_SS(uint8_t BitValue){
    GPIO_WriteBit(GPIOA,NSS,(BitAction)BitValue);   
}
void MySPI_Start(void){
    MySPI_W_SS(0);
}

void MySPI_Stop(void){
    MySPI_W_SS(1);
}



void MySPI_Init(void){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;        //从机选择，通用推挽输出
	GPIO_InitStructure.GPIO_Pin = NSS;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;        //复用推挽输出，由片上外设控制
	GPIO_InitStructure.GPIO_Pin = SCK | MOSI;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;        
	GPIO_InitStructure.GPIO_Pin = MISO;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
    
    SPI_InitTypeDef SPI_InitStructure;
    SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_128;      //波特率分频器
    SPI_InitStructure.SPI_CPHA=SPI_CPHA_1Edge;          //第一个边沿采样
    SPI_InitStructure.SPI_CPOL=SPI_CPOL_Low;            //时钟极性
    SPI_InitStructure.SPI_CRCPolynomial=7;
    SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;
    SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex;    //双线全双工模式
    SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;        //高位先行
    SPI_InitStructure.SPI_Mode=SPI_Mode_Master;     //主机模式
    SPI_InitStructure.SPI_NSS=SPI_NSS_Soft;
    SPI_Init(SPI1,&SPI_InitStructure);
    
    SPI_Cmd(SPI1,ENABLE);
    
    MySPI_W_SS(1);
    
    
    
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
//    
//  GPIO_InitTypeDef GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Pin = NSS | SCK | MOSI;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
//	GPIO_Init(GPIOA,&GPIO_InitStructure);
//    
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//	GPIO_InitStructure.GPIO_Pin = MISO;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
//	GPIO_Init(GPIOA,&GPIO_InitStructure);
//    
//  MySPI_W_SS(1);
//  MySPI_W_SCK(0);
}



uint8_t MySPI_SwapByte(uint8_t ByteSend){
    while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)!=SET);       //等待发送数据寄存器空
    
    SPI_I2S_SendData(SPI1,ByteSend);
    
    //等待收到一个字节，即等待接收数据寄存器非空   
    while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)!=SET); 

    return SPI_I2S_ReceiveData(SPI1);       //返回接收到的数据
    
    
    //软件模拟的第一种交换字节的实现方法
//    uint8_t ByteReceive=0x00;
//    for(int i=0;i<8;i++){
//        MySPI_W_MOSI(ByteSend & (0x80>>i));
//        MySPI_W_SCK(1);
//        if(MySPI_R_MISO()==1){
//            ByteReceive |=(0x80>>i);
//        }
//        MySPI_W_SCK(0);
//    }
//    return ByteReceive;   
    
//    另一种实现方法
    
//    for(int i=0;i<8;i++){
//        MySPI_W_MOSI(ByteSend & 0x80);
//        ByteSend<<=1;
//        MySPI_W_SCK(1);
//        if(MySPI_R_MISO()==1){
//            ByteReceive |= 0x80;
//        }
//        MySPI_W_SCK(0);
//    }
//    return ByteSend; 
}




