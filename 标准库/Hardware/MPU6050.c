#include "stm32f10x.h"                  // Device header
#include "MPU6050_reg.h"
#define MPU6050_ADDRESS 0xD0


//调用软件I2C

//#include "MyI2C.h"

//void MPU6050_WriteReg(uint8_t RegAddress,uint8_t Data){
//    MyI2C_Start();
//    MyI2C_SendByte(MPU6050_ADDRESS);
//    MyI2C_ReceiveAck();
//    MyI2C_SendByte(RegAddress);
//    MyI2C_ReceiveAck();
//    MyI2C_SendByte(Data);
//    MyI2C_ReceiveAck();
//    MyI2C_Stop();
//}

//uint8_t MPU6050_ReadReg(uint8_t RegAddress){
//    MyI2C_Start();
//    MyI2C_SendByte(MPU6050_ADDRESS);
//    MyI2C_ReceiveAck();
//    MyI2C_SendByte(RegAddress);
//    MyI2C_ReceiveAck();
//    
//    MyI2C_Start();
//    MyI2C_SendByte(MPU6050_ADDRESS | 0x01);
//    MyI2C_ReceiveAck();
//    uint8_t Data=MyI2C_ReceiveByte();
//    MyI2C_SendAck(1);       //不给从机应答    
//    MyI2C_Stop();
//    return Data;
//}


//调用硬件I2C

/**
  * 函    数：MPU6050等待事件
  * 参    数：同I2C_CheckEvent
  * 返 回 值：无
  */
void MPU6050_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
	uint32_t Timeout;
	Timeout = 10000;									//给定超时计数时间
	while (I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS)	//循环等待指定事件
	{
		Timeout --;										//等待时，计数值自减
		if (Timeout == 0)								//自减到0后，等待超时
		{
			/*超时的错误处理代码，可以添加到此处*/
			break;										//跳出等待，不等了
		}
	}
}

/**
  * 函    数：MPU6050写寄存器
  * 参    数：RegAddress 寄存器地址，范围：参考MPU6050手册的寄存器描述
  * 参    数：Data 要写入寄存器的数据，范围：0x00~0xFF
  * 返 回 值：无
  */
void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
	I2C_GenerateSTART(I2C2, ENABLE);										//硬件I2C生成起始条件
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);					//等待EV5
	
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter);	//硬件I2C发送从机地址，方向为发送
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);	//等待EV6
	
	I2C_SendData(I2C2, RegAddress);											//硬件I2C发送寄存器地址
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING);			//等待EV8
	
	I2C_SendData(I2C2, Data);												//硬件I2C发送数据
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);				//等待EV8_2
	
	I2C_GenerateSTOP(I2C2, ENABLE);											//硬件I2C生成终止条件
}

/**
  * 函    数：MPU6050读寄存器
  * 参    数：RegAddress 寄存器地址，范围：参考MPU6050手册的寄存器描述
  * 返 回 值：读取寄存器的数据，范围：0x00~0xFF
  */
uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
	uint8_t Data;
	
	I2C_GenerateSTART(I2C2, ENABLE);										//硬件I2C生成起始条件
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);					//等待EV5
	
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter);	//硬件I2C发送从机地址，方向为发送
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);	//等待EV6
	
	I2C_SendData(I2C2, RegAddress);											//硬件I2C发送寄存器地址
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);				//等待EV8_2
	
	I2C_GenerateSTART(I2C2, ENABLE);										//硬件I2C生成重复起始条件
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);					//等待EV5
	
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Receiver);		//硬件I2C发送从机地址，方向为接收
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);		//等待EV6
	
	I2C_AcknowledgeConfig(I2C2, DISABLE);									//在接收最后一个字节之前提前将应答失能
	I2C_GenerateSTOP(I2C2, ENABLE);											//在接收最后一个字节之前提前申请停止条件
	
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);				//等待EV7
	Data = I2C_ReceiveData(I2C2);											//接收数据寄存器
	
	I2C_AcknowledgeConfig(I2C2, ENABLE);									//将应答恢复为使能，为了不影响后续可能产生的读取多字节操作
	
	return Data;
}



void MPU6050_Init(void){
//    MyI2C_Init();
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
    I2C_InitTypeDef I2C_Initstructure;
    I2C_Initstructure.I2C_Ack=I2C_Ack_Enable;       //应答位
    I2C_Initstructure.I2C_AcknowledgedAddress=I2C_AcknowledgedAddress_7bit;     //stm32作为从机可以响应地址的长度
    I2C_Initstructure.I2C_ClockSpeed=50000;         //时钟速度
    I2C_Initstructure.I2C_DutyCycle=I2C_DutyCycle_2;        //占空比，仅100kHz以上有效
    I2C_Initstructure.I2C_Mode=I2C_Mode_I2C;        //模式选择
    I2C_Initstructure.I2C_OwnAddress1=0x00;         //作为从机时的地址
    I2C_Init(I2C2,&I2C_Initstructure);
    
    I2C_Cmd(I2C2,ENABLE);
    
    MPU6050_WriteReg(MPU6050_PWR_MGMT_1,0x01);    //解除睡眠模式，选择陀螺仪时钟
    MPU6050_WriteReg(MPU6050_PWR_MGMT_2,0x00);
    MPU6050_WriteReg(MPU6050_SMPLRT_DIV,0x09);
    MPU6050_WriteReg(MPU6050_CONFIG,0x06);
    MPU6050_WriteReg(MPU6050_GYRO_CONFIG,0x18); 
    MPU6050_WriteReg(MPU6050_ACCEL_CONFIG,0x18);
       
    MPU6050_WriteReg(0x19,0x66);
}




void MPU6050_GetData(int16_t *AccX,int16_t *AccY,int16_t *AccZ,int16_t *GyroX,int16_t *GyroY,int16_t *GyroZ){
    uint8_t DataH,DataL;
    //加速度数据
    DataH=MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);
    DataL=MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
    *AccX=(DataH<<8)| DataL;
    
    DataH=MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
    DataL=MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
    *AccY=(DataH<<8)| DataL; 
    
    DataH=MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
    DataL=MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
    *AccZ=(DataH<<8)| DataL; 
    
    //陀螺仪数据
    DataH=MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);
    DataL=MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
    *GyroX=(DataH<<8)| DataL;
    
    DataH=MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
    DataL=MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
    *GyroY=(DataH<<8)| DataL;
    
    DataH=MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
    DataL=MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
    *GyroZ=(DataH<<8)| DataL;
}

uint8_t MPU6050_GetID(void){
    return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}







