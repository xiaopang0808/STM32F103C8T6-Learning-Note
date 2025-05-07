# STM32F103C8T6 Keil5 学习工程

![STM32F103C8T6](https://img.shields.io/badge/STM32F103C8T6-Blue%20Pill-blue)
![Keil MDK](https://img.shields.io/badge/IDE-Keil%20MDK5-green)

本仓库包含基于 ​**STM32F103C8T6**​（最小系统板）的Keil MDK5工程示例代码，用于学习STM32基础外设开发。

## 📋 目录
- [硬件要求](#-硬件要求)
- [开发环境](#-开发环境)
- [工程结构](#-工程结构)
- [使用说明](#-使用说明)
- [示例功能](#-示例功能)
- [注意事项](#-注意事项)
- [许可证](#-许可证)

## 🛠️ 硬件要求
### 开发板与调试工具
- ✅ ​**STM32F103C8T6 核心板**​（最小系统板）
- ✅ ​**ST-Link V2 下载器**​（或兼容的J-Link、DAP-Link）
- ✅ ​**CH340 USB转串口模块**​（UART通信）

### 传感器模块
- ✅ ​**光敏电阻传感器**​（环境光强度检测）
- ✅ ​**热敏电阻传感器**​（温度检测）
- ✅ ​**反射式红外传感器**​（障碍物检测）
- ✅ ​**对射式红外传感器**​（物体通过检测）
- ✅ ​**MPU6050 模块**​（陀螺仪 & 加速度计）

### 执行器模块
- ✅ ​**有源蜂鸣器模块**​（报警提示音）
- ✅ ​**SG90 舵机**​（角度控制）
- ✅ ​**TB6612 电机驱动模块**​（直流电机控制）

### 显示与存储
- ✅ ​**0.96寸OLED显示屏**​（4针脚，I2C接口，用于数据可视化）
- ✅ ​**W25QXX Flash模块**​（W25Q64/W25Q128，外部存储数据）

### 输入设备
- ✅ ​**旋转编码器**​（旋钮位置检测）
- ✅ ​**按键模块**​（用户输入）
- ✅ ​**LED模块**​（状态指示）

### 其他外设
- ✅ ​**STM32核心板板载资源**​：
  - 用户LED（PC13）
  - 复位按键
  - Boot模式选择跳线

## 💻 开发环境
- Keil uVision5 MDK (v5.37+)
- STM32F1xx_DFP 支持包（建议v2.4.0）
- ST-Link Utility / STM32CubeProgrammer
- 串口调试工具（如Putty、XShell）

## 📂 工程结构
```bash
Project/
├── Hardware/                  # 用户自定义外设驱动
│   ├── AD.c/h                # ADC数据采集
│   ├── Buzzer.c/h            # 蜂鸣器控制
│   ├── CountSensor.c/h       # 计数器传感器
│   ├── DMA.c                 # DMA配置（用户层）
│   ├── Encoder.c/h           # 编码器驱动
│   ├── Encoder_interface.c/h # 编码器接口
│   ├── IC.c/h                # 输入捕获
│   ├── KEY.c/h               # 按键检测
│   ├── LED.c/h               # LED控制
│   ├── LightSensor.c/h       # 光传感器
│   ├── Motor.c/h             # 电机驱动
│   ├── MPU6050.c/h          # 陀螺仪驱动（含MPU6050_reg.h寄存器定义）
│   ├── MyI2C.c/h            # GPIO模拟I2C协议
│   ├── MySPI.c/h            # GPIO模拟SPI协议
│   ├── OLED.c/h             # OLED显示屏驱动（含OLED_Font.h字库）
│   ├── PWM.c/h              # PWM信号生成
│   ├── Serial.c/h           # 串口通信
│   ├── Servo.c/h            # 舵机控制
│   └── W25Q64.c/h           # SPI Flash驱动（含W25Q64_Ins.h指令集）
│
├── Library/                  # STM32标准外设库（CMSIS + SPL）
│   ├── CMSIS/                # Cortex-M3核心支持
│   │   ├── stm32f10x.h       # 芯片寄存器映射
│   │   ├── core_cm3.c/h      # Cortex-M3内核接口
│   │   └── system_stm32f10x.c/h # 系统时钟初始化
│   │
│   ├── SPL/                  # 标准外设库（完整列表）
│   │   ├── stm32f10x_adc.c/h        # ADC驱动
│   │   ├── stm32f10x_bkp.c/h        # 备份寄存器
│   │   ├── stm32f10x_can.c/h        # CAN总线
│   │   ├── stm32f10x_cec.c/h        # HDMI-CEC
│   │   ├── stm32f10x_crc.c/h        # CRC校验
│   │   ├── stm32f10x_dac.c/h        # DAC驱动
│   │   ├── stm32f10x_dbgmcu.c/h     # 调试MCU
│   │   ├── stm32f10x_dma.c/h        # DMA控制器
│   │   ├── stm32f10x_exti.c/h       # 外部中断
│   │   ├── stm32f10x_flash.c/h      # Flash编程
│   │   ├── stm32f10x_fsmc.c/h       # 静态存储器控制器
│   │   ├── stm32f10x_gpio.c/h       # GPIO控制
│   │   ├── stm32f10x_i2c.c/h        # I2C总线
│   │   ├── stm32f10x_iwdg.c/h       # 独立看门狗
│   │   ├── stm32f10x_pwr.c/h        # 电源控制
│   │   ├── stm32f10x_rcc.c/h        # 复位与时钟控制
│   │   ├── stm32f10x_rtc.c/h        # 实时时钟
│   │   ├── stm32f10x_sdio.c/h       # SD卡接口
│   │   ├── stm32f10x_spi.c/h        # SPI总线
│   │   ├── stm32f10x_tim.c/h        # 定时器
│   │   ├── stm32f10x_usart.c/h      # 串口通信
│   │   ├── stm32f10x_wwdg.c/h       # 窗口看门狗
│   │   └── misc.c/h                 # 中断优先级管理
│
├── Start/                     # 启动文件（根据芯片型号选择）
│   ├── startup_stm32f10x_cl.s      # 互联型（Connectivity Line）
│   ├── startup_stm32f10x_hd.s       # 大容量（High Density，Flash≥256KB）
│   ├── startup_stm32f10x_hd_vl.s    # 大容量+超值型（Value Line）
│   ├── startup_stm32f10x_ld.s       # 小容量（Low Density，Flash≤32KB）
│   ├── startup_stm32f10x_ld_vl.s    # 小容量+超值型
│   ├── startup_stm32f10x_md.s       # 中容量（Medium Density，64KB≤Flash≤128KB）
│   ├── startup_stm32f10x_md_vl.s    # 中容量+超值型
│   └── startup_stm32f10x_xl.s      # 超大容量（XL Density，Flash≥512KB）
│
├── System/                    # 系统级功能
│   ├── DMA.c/h                # DMA通道管理
│   ├── Timer.c/h              # 定时器基础配置
│   └── Delay.c/h              # SysTick实现的精确延时
│
├── User/                      # 用户应用代码
│   ├── main.c                 # 主程序入口
│   ├── stm32f10x_it.c/h       # 中断服务函数
│   └── stm32f10x_conf.h       # 库功能开关（如启用ADC/USART等）
│
├── MDK-ARM/                   # Keil工程文件
│   ├── Project.uvprojx        # 工程主文件
│   └── Project.uvoptx         # 调试配置（如断点、变量监视）
│
└── Utilities/                 # 实用工具
    └── keilkill.bat           # 清理编译残留文件（*.o, *.axf等）
```

## 🚀 使用说明
1. 克隆仓库：
   ```bash
   git clone https://github.com/xiaopang0808/STM32F103C8T6-Learning-Note.git
   ```
2. 使用Keil MDK5打开工程文件：  
   `MDK-ARM/YourProjectName.uvprojx`
3. 编译工程：`Project -> Build Target` (F7)
4. 连接ST-Link调试器：
   - 选择调试工具：`Project -> Options for Target -> Debug -> ST-Link Debugger`
   - 确保开发板供电正常
5. 下载程序：`Flash -> Download` (F8)
6. 复位开发板观察现象

## 🌟 示例功能
当前工程已实现以下功能：
- ✅ ​**LED闪烁**​（GPIO控制，基于 `Hardware/LED.c`）
- ✅ ​**按键中断检测**​（支持长按/短按，基于 `Hardware/KEY.c`）
- ✅ ​**USART1 串口通信**​（115200bps，支持printf重定向，基于 `Hardware/Serial.c`）
- ✅ ​**PWM输出控制**​（调节LED亮度/舵机角度，基于 `Hardware/PWM.c`）
- ✅ ​**ADC采集**​（光敏/热敏电阻信号，基于 `Hardware/AD.c`）
- ✅ ​**蜂鸣器报警**​（支持频率调节，基于 `Hardware/Buzzer.c`）
- ✅ ​**旋转编码器计数**​（EC11类型，基于 `Hardware/Encoder.c`）
- ✅ ​**电机调速控制**​（TB6612驱动，基于 `Hardware/Motor.c`）
- ✅ ​**OLED显示**​（实时数据可视化，基于 `Hardware/OLED.c`）
- ✅ ​**MPU6050姿态检测**​（读取加速度/陀螺仪数据，基于 `Hardware/MPU6050.c`）
- ✅ ​**外部Flash存储**​（W25Q64数据读写，基于 `Hardware/W25Q64.c`）
- ✅ ​**红外传感器检测**​：
  - 反射式（障碍物距离检测，基于 `Hardware/LightSensor.c`）
  - 对射式（物体通过计数，基于 `Hardware/CountSensor.c`）
- ✅ ​**DMA传输优化**​（ADC/USART数据高效传输，基于 `System/DMA.c`）
- ✅ ​**定时器输入捕获**​（测量脉冲宽度，基于 `Hardware/IC.c`）
- ✅ ​**SysTick精确延时**​（毫秒/微秒级，基于 `System/Delay.c`）
- ✅ ​**GPIO模拟I2C/SPI协议**​（兼容无硬件外设场景，基于 `Hardware/MyI2C.c` 和 `Hardware/MySPI.c`）

📍 ​**待实现功能**​：
- 多机CAN总线通信
- FATFS文件系统移植
- FreeRTOS任务调度

## ⚠️ 注意事项
1. ​**开发环境配置**​  
   - 首次使用需通过Keil Pack Installer安装 ​**STM32F1xx_DFP**​ 支持包（v2.4.0+）  
   - 检查标准库完整性：确保 `Library` 目录包含完整STM32F10x标准外设库文件（如 `stm32f10x_gpio.c`、`stm32f10x_rcc.c` 等）

## ⚠️ 注意事项
2. ​**硬件适配修改**​  
   - ​**时钟配置**​：在 `System/system_stm32f10x.c` 中修改 `HSE_VALUE`（**默认为8MHz**，需与外部晶振实际频率一致，本工程通过PLL倍频至72MHz系统时钟）  
   - ​**引脚定义**​：根据核心板原理图修改以下文件中的引脚：  
     ```c
     Hardware/LED.h       // LED引脚
     Hardware/KEY.h       // 按键引脚
     Hardware/OLED.h      // OLED的I2C/SPI接口引脚
     ```
   - ​**通信参数**​：在 `Hardware/Serial.h` 中修改串口波特率（默认115200）

3. ​**外设模块依赖**​  
   - ​**I2C/SPI冲突**​：若同时使用MPU6050（I2C）和W25Q64（SPI），需检查总线引脚是否冲突  
   - ​**传感器电压**​：光敏/热敏电阻模块需接3.3V，避免5V烧毁STM32 ADC引脚  
   - ​**电机供电隔离**​：TB6612模块的电机电源（VM）与STM32逻辑电源需隔离

4. ​**工程配置建议**​  
   - ​**启动文件**​：确认 `Start/` 目录下选择正确的启动文件（STM32F103C8T6为中等容量，应选 `startup_stm32f10x_md.s`）  
   - ​**优化等级**​：在Keil中设置 `Optimization Level -O0` 避免某些外设驱动异常（如MyI2C的延时函数）  
   - ​**下载器配置**​：ST-Link需连接 `SWDIO` 和 `SWCLK` 引脚，并在Keil中启用 `Reset and Run`

5. ​**关键文件说明**​  
   - `stm32f10x_conf.h`：启用/禁用外设编译（如未使用CAN总线可注释 `#define _CAN`）  
   - `stm32f10x_it.c`：添加自定义中断服务函数（如编码器中断）  
   - `System/Delay.c`：若需高精度延时，需校准 `SysTick` 时钟源  

6. ​**调试提示**​  
   - 若OLED无显示：检查是否在 `OLED_Init()` 中正确配置I2C地址（默认0x78）  
   - 若MPU6050数据异常：检查 `MyI2C.c` 中的时序延时是否匹配模块要求  
   - 若Flash写入失败：确认 `W25Q64.c` 中已解除写保护（发送 `0x01, 0x00` 指令）  

7. ​**资源占用**​  
   - 编译后 `Program Size` 应小于64KB（STM32F103C8T6 Flash容量）  
   - 若内存不足，可在 `Target Options -> C/C++` 中勾选 `One ELF Section per Function` 优化  

## 📜 许可证
本项目采用 [MIT License](LICENSE)，欢迎学习与二次开发。

---
🙋 如有问题请联系：  
[![GitHub Issues](https://img.shields.io/github/issues/xiaopang0808/STM32F103C8T6-Learning-Note)](https://github.com/xiaopang0808/STM32F103C8T6-Learning-Note/issues)  
📚 仓库地址：  
https://github.com/xiaopang0808/STM32F103C8T6-Learning-Note.git
