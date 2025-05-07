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
- STM32F103C8T6 最小系统板
- ST-Link V2 下载器（或其他兼容调试器）
- USB转串口模块（如CH340/CP2102，用于UART通信）
- LED、按键等外设

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
当前工程包含以下功能（根据实际情况修改）：
- ✅ LED闪烁（GPIO控制）
- ✅ 按键中断检测
- ✅ USART1 串口通信（收发数据）
- ✅ PWM输出（如控制LED亮度）
- ✅ ADC采集（需连接传感器）
- 📍 更多功能持续更新中...

## ⚠️ 注意事项
1. 首次使用时需安装 ​**STM32F1xx_DFP**​ 支持包（通过Keil Pack Installer）
2. 若使用标准外设库，请检查 `Library` 目录下的文件是否完整
3. 根据实际硬件修改以下配置：
   - 系统时钟源（HSE_VALUE，默认为8MHz）
   - GPIO引脚定义（LED/按键等引脚号）
   - 串口波特率（默认115200）
4. 推荐使用 STM32CubeMX 生成初始化代码

## 📜 许可证
本项目采用 [MIT License](LICENSE)，欢迎学习与二次开发。

---
🙋 如有问题请联系：  
[![GitHub Issues](https://img.shields.io/github/issues/xiaopang0808/STM32F103C8T6-Learning-Note)](https://github.com/xiaopang0808/STM32F103C8T6-Learning-Note/issues)  
📚 仓库地址：  
https://github.com/xiaopang0808/STM32F103C8T6-Learning-Note.git
