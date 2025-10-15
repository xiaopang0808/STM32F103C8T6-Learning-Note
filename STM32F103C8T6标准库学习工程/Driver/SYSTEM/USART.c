/******************************************************************************
* 文件名：USART.c
* 作　者：Xiaopang
* 创建日期：2025-07-26
* 最后修改：2025-07-26
* 版　本：v1.1
* 功能描述：STM32F103C8T6串口(USART)驱动实现
*           支持9600波特率，8位数据位，1位停止位，无校验位
*           使用中断方式接收数据包，数据包格式为"@消息内容\r\n"
*
* 修改记录：
*  v1.1 2025-07-26  修正GPIO初始化错误，完善状态机注释
*
* 注意事项：
* 1. 数据包格式要求以'@'开头，以"\r\n"结尾
* 2. RX状态机使用三级状态处理协议帧
******************************************************************************/

#include "stm32f10x.h"
#include "USART.h"

char USART_RxPacket[100];        // 接收数据包缓冲区
uint8_t USART_RxFlag = 0;        // 接收完成标志

/**
  * @brief  串口初始化函数
  * @note   配置USART1为9600波特率，8位数据位，1位停止位，无校验位，收发模式
  *         使能接收中断，初始化对应GPIO
  */
void USART_Init_Func(void)
{
    // 1. 时钟使能
    USART_RX_GPIO_CLK_ENABLE();     // RX引脚时钟使能
    USART_TX_GPIO_CLK_ENABLE();     // TX引脚时钟使能
    USART_UX_CLK_ENABLE();          // USART时钟使能
    
    // 2. GPIO初始化
    GPIO_InitTypeDef GPIO_InitStruct;
    
    // TX引脚配置(复用推挽输出)
    GPIO_InitStruct.GPIO_Pin = USART_TX_GPIO_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;      // 复用推挽输出
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(USART_TX_GPIO_PORT, &GPIO_InitStruct);  // 注意使用宏定义端口
    
    // RX引脚配置(上拉输入)
    GPIO_InitStruct.GPIO_Pin = USART_RX_GPIO_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;         // 上拉输入
    GPIO_Init(USART_RX_GPIO_PORT, &GPIO_InitStruct);  // 注意使用宏定义端口
    
    // 3. USART参数配置
    USART_InitTypeDef USART_InitStruct;
    USART_InitStruct.USART_BaudRate = 9600;                        // 波特率
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;       // 8位数据位
    USART_InitStruct.USART_StopBits = USART_StopBits_1;            // 1位停止位
    USART_InitStruct.USART_Parity = USART_Parity_No;               // 无校验
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无硬件流控
    USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;   // 收发模式
    USART_Init(USART_UX, &USART_InitStruct);
    
    // 4. 中断配置
    USART_ITConfig(USART_UX, USART_IT_RXNE, ENABLE);  // 使能接收中断
    
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = USART_UX_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;  // 抢占优先级
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;         // 响应优先级
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
    
    // 5. 使能USART
    USART_Cmd(USART_UX, ENABLE);
}

/**
  * @brief  发送单字节数据
  * @param  Byte 要发送的字节
  */
void USART_SendByte(uint8_t Byte)
{
    USART_SendData(USART_UX, Byte);  // 写入数据寄存器
    // 等待发送完成(硬件自动清除标志)
    while (USART_GetFlagStatus(USART_UX, USART_FLAG_TXE) == RESET);
}

/**
  * @brief  发送字节数组
  * @param  Array 数组首地址
  * @param  Length 数组长度
  */
void USART_SendArray(uint8_t *Array, uint16_t Length)
{
    for(uint16_t i=0; i<Length; i++){
        USART_SendByte(Array[i]);  // 逐字节发送
    }
}

/**
  * @brief  发送字符串(以'\0'结尾)
  * @param  String 字符串首地址
  */
void USART_SendString(char *String)
{
    for(uint8_t i=0; String[i]!='\0'; i++){
        USART_SendByte(String[i]);  // 逐字符发送
    }
}

/* 内部工具函数：计算X的Y次方 */
static uint32_t USART_Pow(uint32_t X, uint32_t Y)
{
    uint32_t Result = 1;
    while(Y--) {
        Result *= X;
    }
    return Result;
}

/**
  * @brief  发送数字
  * @param  Number 要发送的数字(0-4294967295)
  * @param  Length 数字长度(如1234长度为4)
  */
void USART_SendNumber(uint32_t Number, uint8_t Length)
{
    for(uint8_t i=0; i<Length; i++){
        // 分解数字的每一位并转换为ASCII
        uint8_t digit = Number / USART_Pow(10, Length-i-1) % 10;
        USART_SendByte(digit + '0');
    }
}

/**
  * @brief  重定向的fputc函数(支持printf)
  * @param  ch 要发送的字符
  * @param  f 文件指针(未使用)
  * @return 发送的字符
  */
int fputc(int ch, FILE *f)
{
    USART_SendByte(ch);  // 通过串口发送
    return ch;
}

/**
  * @brief  自定义格式化打印函数
  * @param  format 格式化字符串
  * @param  ... 可变参数列表
  */
void USART_Printf(char *format, ...)
{
    char buffer[100];
    va_list args;
    
    va_start(args, format);         // 初始化可变参数列表
    vsprintf(buffer, format, args); // 格式化字符串
    va_end(args);                   // 清理可变参数列表
    
    USART_SendString(buffer);       // 发送格式化后的字符串
}

/**
  * @brief  USART1中断服务函数
  * @note   使用状态机解析数据包格式："@消息内容\r\n"
  *         状态0：等待包头'@'
  *         状态1：接收消息内容(直到遇到'\r')
  *         状态2：等待包尾'\n'
  */
void USART1_IRQHandler(void)
{
    static uint8_t RxState = 0;     // 状态机当前状态
    static uint8_t pRxPacket = 0;   // 数据包缓冲区索引
    
    // 检查是否接收中断
    if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
    {
        uint8_t RxData = USART_ReceiveData(USART1);  // 读取接收数据
        
        switch(RxState)
        {
            // 状态0：等待包头
            case 0:
                if(RxData == '@' && USART_RxFlag == 0) {
                    RxState = 1;        // 进入接收数据状态
                    pRxPacket = 0;      // 重置缓冲区索引
                }
                break;
                
            // 状态1：接收数据内容
            case 1:
                if(RxData == '\r') {
                    RxState = 2;  // 进入等待包尾状态
                } 
                else {
                    // 存储数据并防止缓冲区溢出
                    if(pRxPacket < sizeof(USART_RxPacket)-1) {
                        USART_RxPacket[pRxPacket++] = RxData;
                    }
                }
                break;
                
            // 状态2：检查包尾
            case 2:
                if(RxData == '\n') {
                    RxState = 0;  // 返回初始状态
                    USART_RxPacket[pRxPacket] = '\0';  // 字符串结束符
                    USART_RxFlag = 1;    // 设置接收完成标志
                }
                break;
        }
        
        // 清除中断标志
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}
