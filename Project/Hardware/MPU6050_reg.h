#ifndef __MPU6050_REG_H
#define __MPU6050_REG_H

/*----------------------------------
  采样率控制寄存器
  ----------------------------------*/
#define MPU6050_SMPLRT_DIV     0x19  // 采样率分频寄存器
                                     // 采样率 = 陀螺仪输出率 / (1 + SMPLRT_DIV)
                                     
/*----------------------------------
  设备配置寄存器组
  ----------------------------------*/
#define MPU6050_CONFIG         0x1A  // 配置寄存器
                                     // 配置数字低通滤波器(DLPF)参数
                                     
#define MPU6050_GYRO_CONFIG    0x1B  // 陀螺仪配置寄存器
                                     // 设置陀螺仪量程(±250/±500/±1000/±2000°/s)
                                     // 和自检使能
                                     
#define MPU6050_ACCEL_CONFIG   0x1C  // 加速度计配置寄存器
                                     // 设置加速度计量程(±2g/±4g/±8g/±16g)
                                     // 和自检使能

/*----------------------------------
  传感器数据输出寄存器组
  所有数据均为16位有符号整型（高位在前）
  ----------------------------------*/
#define MPU6050_ACCEL_XOUT_H   0x3B  // 加速度计X轴高字节
#define MPU6050_ACCEL_XOUT_L   0x3C  // 加速度计X轴低字节
#define MPU6050_ACCEL_YOUT_H   0x3D  // 加速度计Y轴高字节
#define MPU6050_ACCEL_YOUT_L   0x3E  // 加速度计Y轴低字节
#define MPU6050_ACCEL_ZOUT_H   0x3F  // 加速度计Z轴高字节
#define MPU6050_ACCEL_ZOUT_L   0x40  // 加速度计Z轴低字节

#define MPU6050_TEMP_OUT_H     0x41  // 温度传感器高字节
#define MPU6050_TEMP_OUT_L     0x42  // 温度传感器低字节
                                     // 温度计算公式：TEMP_degC = (TEMP_OUT/340.0) + 36.53

#define MPU6050_GYRO_XOUT_H    0x43  // 陀螺仪X轴高字节
#define MPU6050_GYRO_XOUT_L    0x44  // 陀螺仪X轴低字节
#define MPU6050_GYRO_YOUT_H    0x45  // 陀螺仪Y轴高字节
#define MPU6050_GYRO_YOUT_L    0x46  // 陀螺仪Y轴低字节
#define MPU6050_GYRO_ZOUT_H    0x47  // 陀螺仪Z轴高字节
#define MPU6050_GYRO_ZOUT_L    0x48  // 陀螺仪Z轴低字节

/*----------------------------------
  电源管理寄存器组
  ----------------------------------*/
#define MPU6050_PWR_MGMT_1     0x6B  // 电源管理寄存器1
                                     // 控制设备电源模式、温度传感器使能
                                     // 和时钟源选择（默认值0x00：内部8MHz时钟）

#define MPU6050_PWR_MGMT_2     0x6C  // 电源管理寄存器2
                                     // 控制加速度计和陀螺仪各轴的待机模式

/*----------------------------------
  设备识别寄存器
  ----------------------------------*/
#define MPU6050_WHO_AM_I       0x75  // 设备ID寄存器（默认值0x68）
                                     // 读取该寄存器应返回0x68（MPU6050）或0x69（MPU6051）

#endif