#include "stm32f10x.h"                  // Device header

uint16_t AD_Value[4];

void AD_Init(void) {
    /* 时钟配置 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);    // 启用ADC1时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   // 启用GPIOA时钟（PA0用于ADC）
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);                       // 设置ADC时钟预分频= PCLK2/6（假设PCLK2=72MHz → 12MHz）
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
    
    /* GPIO配置 */
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;           // 模拟输入模式（禁用施密特触发器）
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;               // 配置PA0引脚（对应ADC1通道0）
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       // 速度参数对模拟输入无影响，保持默认
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* ADC通道配置   可在AD_GetValue函数中调用，实现多通道*/
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);	//规则组序列1的位置，配置为通道0
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);	//规则组序列2的位置，配置为通道1
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_55Cycles5);	//规则组序列3的位置，配置为通道2
	  
    // 参数说明：
    // ADC1: 目标ADC
    // ADC_Channel_0: 选择通道0（PA0）
    // 1: 转换序列中的优先级（扫描模式下生效）
    // ADC_SampleTime_55Cycles5: 55.5时钟周期的采样时间（12MHz时≈4.63μs）

    /* ADC工作模式配置 */
    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // 软件触发转换（无硬件触发）
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;  // 数据右对齐（12位有效数据在低12位）
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;      // 独立模式（非双ADC协作模式）
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;           // 禁用扫描模式（单通道转换）
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;     // 单次转换模式（需要手动触发每次转换）
    ADC_InitStructure.ADC_NbrOfChannel = 3;                 // 转换通道数（扫描模式时有效）
    ADC_Init(ADC1, &ADC_InitStructure);
    
    
    DMA_InitTypeDef DMA_InitStructure;											//定义结构体变量
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;				//外设基地址，给定形参AddrA
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//外设数据宽度，选择半字，对应16为的ADC数据寄存器
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			//外设地址自增，选择失能，始终以ADC数据寄存器为源
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)AD_Value;					//存储器基地址，给定存放AD转换结果的全局数组AD_Value
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;			//存储器数据宽度，选择半字，与源数据宽度对应
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;						//存储器地址自增，选择使能，每次转运后，数组移到下一个位置
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;							//数据传输方向，选择由外设到存储器，ADC数据寄存器转到数组
	DMA_InitStructure.DMA_BufferSize = 4;										//转运的数据大小（转运次数），与ADC通道数一致
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;								//模式，选择循环模式，与ADC的连续转换一致
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;								//存储器到存储器，选择失能，数据由ADC外设触发转运到存储器
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;						//优先级，选择中等
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);								//将结构体变量交给DMA_Init，配置DMA1的通道1
	
    
    DMA_Cmd(DMA1_Channel1,ENABLE);
    ADC_DMACmd(ADC1,ENABLE);
    
    /* 启用ADC并校准 */
    ADC_Cmd(ADC1, ENABLE);                                  // 激活ADC模块
    
    // 校准流程（必须执行以确保精度）
    ADC_ResetCalibration(ADC1);                             // 复位校准寄存器
    while (ADC_GetResetCalibrationStatus(ADC1) == SET);     // 等待复位完成
    ADC_StartCalibration(ADC1);                             // 开始校准
    while (ADC_GetCalibrationStatus(ADC1) == SET);          // 等待校准完成（约需1ms）
    
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void AD_GetValue(void) {
    DMA_Cmd(DMA1_Channel1,DISABLE);
    DMA_SetCurrDataCounter(DMA1_Channel1,3);
    DMA_Cmd(DMA1_Channel1,ENABLE);
    
    /* 启动单次转换（仅在单次/非连续模式下需要显式触发） */
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);  // 向ADC1发送软件转换启动命令    
     
    while(DMA_GetFlagStatus(DMA1_FLAG_TC1)==RESET);
    DMA_ClearFlag(DMA1_FLAG_TC1);
    
    
    

    
//    /* 阻塞等待转换完成 */
//    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET); 
//    // 注意：EOC标志在读取DR寄存器后自动清除，或需手动清除（根据库版本）
//    /* 读取并返回转换结果 */
//    return ADC_GetConversionValue(ADC1);     // 直接读取数据寄存器（右对齐12位值）
}

