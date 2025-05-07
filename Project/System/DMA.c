#include "stm32f10x.h"                  // Device header

uint16_t Mysize;

void MyDMA_Init(uint32_t AddrA,uint32_t AddrB,uint16_t Size){
    Mysize=Size;
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
    
    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_BufferSize = Size;                                    /* 数据传输量*/
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                          /* 数据传输方向*/
    DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;                                 /* 内存到内存模式 ,是否软件触发*/
    DMA_InitStructure.DMA_MemoryBaseAddr = AddrB;                               /* 内存基地址*/
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;             /* 内存数据宽度 */
    DMA_InitStructure.DMA_MemoryInc =  DMA_MemoryInc_Enable;                    /* 内存地址递增使能 */
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                               /* DMA传输模式*/
    DMA_InitStructure.DMA_PeripheralBaseAddr = AddrA;                           /* 外设数据寄存器地址*/
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;     /* 外设数据宽度*/
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;             /* 外设地址递增使能*/
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                       /* DMA通道优先级（多个DMA请求时生效）*/
    DMA_Init(DMA1_Channel1,&DMA_InitStructure);
    
    DMA_Cmd(DMA1_Channel1,DISABLE);
}

void MyDMA_Transfer(void){
    DMA_Cmd(DMA1_Channel1,DISABLE);
    DMA_SetCurrDataCounter(DMA1_Channel1,Mysize);
    DMA_Cmd(DMA1_Channel1,ENABLE);
    
    while(DMA_GetFlagStatus(DMA1_FLAG_TC1)==RESET);
    DMA_ClearFlag(DMA1_FLAG_TC1);
}



