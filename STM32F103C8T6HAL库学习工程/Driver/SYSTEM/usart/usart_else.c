/******************************************************************************
* 文件名：usart_else.c
* 作　者：Xiaopang
* 创建日期：2025-07-27
* 最后修改：2025-07-27
* 版　本：v1.0
* 功能描述：STM32F103C8T6串口(USART)驱动实现
*			由标准库模板移植而来，格式上会有所不同
*           支持9600波特率，8位数据位，1位停止位，无校验位
*           使用中断方式接收数据包，数据包格式为"@消息内容\r\n"
*
* 修改记录：
*  版本   日期       修改人     修改内容
*  v1.0  2025-07-27  Xiaopang   初始版本创建
*
* 注意事项：
* 1. 数据包格式要求以'@'开头，以"\r\n"结尾
* 2. RX状态机使用三级状态处理协议帧
******************************************************************************/

#if 0
#include "usart_else.h"

char USART_RxPacket[100];        // 接收数据包缓冲区
uint8_t USART_RxFlag = 0;        // 接收完成标志
UART_HandleTypeDef huart1;       // HAL库UART句柄

/**
  * @brief  串口初始化函数
  * @note   配置USART1为9600波特率，8位数据位，1位停止位，无校验位，收发模式
  *         使能接收中断，初始化对应GPIO
  */
void USART_Init_Func(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    // 1. 使能时钟
    USART_UX_CLK_ENABLE();
    USART_TX_GPIO_CLK_ENABLE();
    USART_TX_GPIO_CLK_ENABLE();
    
    // 2. 配置GPIO
    // TX引脚配置(复用推挽输出)
    GPIO_InitStruct.Pin = USART_TX_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(USART_TX_GPIO_PORT, &GPIO_InitStruct);
    
    // RX引脚配置(上拉输入)
    GPIO_InitStruct.Pin = USART_RX_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(USART_RX_GPIO_PORT, &GPIO_InitStruct);
    
    // 3. 配置UART参数
    huart1.Instance = USART_UX;
    huart1.Init.BaudRate = 9600;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    
    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        // 初始化错误处理
        while(1);
    }
    
    // 4. 使能接收中断
    HAL_UART_Receive_IT(&huart1, (uint8_t *)&huart1.pRxBuffPtr, 1);
    
    // 5. 配置中断优先级
    HAL_NVIC_SetPriority(USART_UX_IRQn, 1, 1);
    HAL_NVIC_EnableIRQ(USART_UX_IRQn);
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
void USARTx_IRQHandler(void)
{
    HAL_UART_IRQHandler(&huart1);
}

/**
  * @brief  HAL库UART接收完成回调函数
  * @param  huart UART句柄
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    static uint8_t RxState = 0;     // 状态机当前状态
    static uint8_t pRxPacket = 0;   // 数据包缓冲区索引
    
    if (huart->Instance == USART_UX)
    {
        uint8_t RxData = *huart->pRxBuffPtr; // 获取接收到的数据
        
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
        
        // 重新使能接收中断
        HAL_UART_Receive_IT(huart, huart->pRxBuffPtr, 1);
    }
}

#endif