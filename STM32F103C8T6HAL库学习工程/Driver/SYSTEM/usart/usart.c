/******************************************************************************
* 文件名：usart.c
* 作　者：Xiaopang
* 创建日期：2025-07-18
* 最后修改：2025-07-18
* 版　本：v1.0
* 功能描述：
*   STM32F103C8T6 USART通信驱动实现文件
*   主要功能包括：
*     1. 实现USART外设初始化及配置
*     2. 管理GPIO引脚复用和时钟使能
*     3. 实现中断接收机制
*     4. 提供接收完成回调函数
* 
*   特性说明：
*     - 支持异步全双工通信模式
*     - 默认使用USART1外设（PA9-TX/PA10-RX）
*     - 可配置波特率
*     - 采用单字节中断接收机制
* 
* 修改记录：
*  版本   日期       修改人     修改内容
*  v1.0  2025-07-18  Xiaopang   初始版本创建
*
* 注意事项：
*   1. 使用前需包含sys.h和usart.h头文件
*   2. 主程序中应定期检查g_usart_rx_flag标志位，并在处理完成后及时清零
*   3. 接收缓冲区g_rx_buffer为单字节，如需存储历史数据应自行扩展缓冲区
*   4. 如更改串口外设，需在usart.h中同步修改相关宏定义
*   5. 如需支持其他串口(USART2/USART3)，需在HAL_UART_MspInit中添加相应配置
* 
* 使用示例：
*   // 初始化串口(115200波特率)
*   USART_Init(115200);
*   
*   // 主循环中检测接收
*   while(1) {
*       if(g_usart_rx_flag) {
*           g_usart_rx_flag = 0;
*           printf("Received: %c\n", g_rx_buffer[0]);
*       }
*   }
*
******************************************************************************/

#include "sys.h"
#include "usart.h"

/******************************************************************************************/
/* 加入以下代码, 支持printf函数, 而不需要选择use MicroLIB */

#if 1

#if (__ARMCC_VERSION >= 6010050)            /* 使用AC6编译器时 */
__asm(".global __use_no_semihosting\n\t");  /* 声明不使用半主机模式 */
__asm(".global __ARM_use_no_argv \n\t");    /* AC6下需要声明main函数为无参数格式，否则部分例程可能出现半主机模式 */

#else
/* 使用AC5编译器时, 要在这里定义__FILE 和 不使用半主机模式 */
#pragma import(__use_no_semihosting)

struct __FILE
{
    int handle;
    /* Whatever you require here. If the only file you are using is */
    /* standard output using printf() for debugging, no file handling */
    /* is required. */
};

#endif

/* 不使用半主机模式，至少需要重定义_ttywrch\_sys_exit\_sys_command_string函数,以同时兼容AC6和AC5模式 */
int _ttywrch(int ch)
{
    ch = ch;
    return ch;
}

/* 定义_sys_exit()以避免使用半主机模式 */
void _sys_exit(int x)
{
    x = x;
}

char *_sys_command_string(char *cmd, int len)
{
    return NULL;
}


/* FILE 在 stdio.h里面定义. */
FILE __stdout;

/* MDK下需要重定义fputc函数, printf函数最终会通过调用fputc输出字符串到串口 */
int fputc(int ch, FILE *f)
{
    while ((USART_UX->SR & 0X40) == 0);     /* 等待上一个字符发送完成 */

    USART_UX->DR = (uint8_t)ch;             /* 将要发送的字符 ch 写入到DR寄存器 */
    return ch;
}
#endif
/******************************************************************************************/

/* 全局变量定义 -----------------------------------------------------------*/
UART_HandleTypeDef g_uart_handle;          // UART配置结构体实例
uint8_t g_rx_buffer[BUFFER_SIZE] = {0};              // 接收数据缓冲区
uint8_t g_usart_rx_flag = 0;               // 接收标志（原子操作标志）


/******************************************************************************
* 函数名：USART_Init
* 功能：初始化USART外设
* 参数：baudrate - 通信波特率（如115200, 9600等）
* 说明：配置USART通信参数并启用中断接收
******************************************************************************/
void USART_Init(uint32_t baudrate)
{
    /* 配置UART基础参数 */
    g_uart_handle.Instance = USART_UX;              		// 选择USART外设
    g_uart_handle.Init.BaudRate = baudrate;        		 	// 设置波特率
    g_uart_handle.Init.WordLength = UART_WORDLENGTH_8B;  	// 8位数据位
    g_uart_handle.Init.StopBits = UART_STOPBITS_1;  		// 1位停止位
    g_uart_handle.Init.Parity = UART_PARITY_NONE;   		// 无校验位
    g_uart_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE; 	// 无硬件流控
    g_uart_handle.Init.Mode = UART_MODE_TX_RX;      		// 使能发送和接收模式
    
    /* 应用配置 */
    HAL_UART_Init(&g_uart_handle);
    
    /* 启动中断接收机制：设置接收缓冲区，收到数据时触发中断 */
    HAL_UART_Receive_IT(&g_uart_handle, g_rx_buffer, BUFFER_SIZE);
}

/******************************************************************************
* 函数名：HAL_UART_MspInit
* 功能：硬件层初始化（MSP：MCU支持包初始化）
* 参数：huart - UART句柄指针
* 说明：由HAL库自动调用，初始化时钟、GPIO和中断
*       此函数会被HAL_UART_Init()函数调用
*       仅处理USART_UX定义的外设，其他串口需扩展此函数
******************************************************************************/
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef gpio_init_struct;
    
    /* 仅当是目标USART外设时执行初始化 */
    if(huart->Instance == USART_UX)
    {
        /* 1. 使能外设时钟 */
        USART_UX_CLK_ENABLE();                     // USART外设时钟
        USART_TX_GPIO_CLK_ENABLE();                 // TX引脚端口时钟
        USART_RX_GPIO_CLK_ENABLE();                 // RX引脚端口时钟
        
        /* 2. 配置TX引脚为复用推挽输出 */
        gpio_init_struct.Pin = USART_TX_GPIO_PIN;   // TX引脚编号
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;    // 复用推挽输出模式
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH; // 高速模式
        HAL_GPIO_Init(USART_TX_GPIO_PORT, &gpio_init_struct); // 应用配置
        
        /* 3. 配置RX引脚为上拉输入 */
        gpio_init_struct.Pin = USART_RX_GPIO_PIN;   // RX引脚编号
        gpio_init_struct.Mode = GPIO_MODE_AF_INPUT; // 复用输入模式
        gpio_init_struct.Pull = GPIO_PULLUP;         // 内部上拉
        HAL_GPIO_Init(USART_RX_GPIO_PORT, &gpio_init_struct); // 应用配置
        
        /* 4. 配置USART中断 */
        HAL_NVIC_SetPriority(USART_UX_IRQn, 2, 2);  // 设置中断优先级（主2，子2）
        HAL_NVIC_EnableIRQ(USART_UX_IRQn);          // 使能USART中断
    }
}

/******************************************************************************
* 函数名：USART1_IRQHandler
* 功能：USART1全局中断服务函数
* 说明：处理所有USART1中断事件，包括发送完成、接收完成等
*       通过HAL_UART_IRQHandler函数进行具体中断处理
*       处理完成后重新启动单字节接收中断
******************************************************************************/
void USART1_IRQHandler(void)
{
    /* 基础中断处理（HAL库会自动识别具体中断类型） */
    HAL_UART_IRQHandler(&g_uart_handle);
    
    /* 重新启动接收中断 */
    HAL_UART_Receive_IT(&g_uart_handle, g_rx_buffer, BUFFER_SIZE);
}

/******************************************************************************
* 函数名：HAL_UART_RxCpltCallback
* 功能：接收完成回调函数
* 参数：huart - UART句柄指针
* 说明：当接收完指定字节后由HAL库调用，设置接收标志位
*       此函数在中断上下文中执行，应保持简洁
*       注意：此处仅设置标志位，实际数据处理应在主循环中完成
******************************************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    /* 设置接收完成标志（主程序可通过此标志处理数据） */
    g_usart_rx_flag = 1;
}


/******************************************************************************
* 测试代码示例
******************************************************************************/
#if 0
#include "stm32f1xx.h"
#include "stdio.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
int main()
{
    HAL_Init();
    sys_stm32_clock_init(RCC_PLL_MUL9);		/* 初始化系统时钟 */  
	USART_Init(115200);
    while(1)
    {
		if(g_usart_rx_flag == 1)
		{
			HAL_UART_Transmit(&g_uart_handle,(uint8_t*)g_rx_buffer,BUFFER_SIZE,1000);
			while(__HAL_UART_GET_FLAG(&g_uart_handle,UART_FLAG_TC) != 1);
			printf("\r\n");
			g_usart_rx_flag = 0;
		}
		else
		{
			Delay_ms(100);
		}
	}
}
#endif
/******************************************************************************/

