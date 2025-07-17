#ifndef __W25Q64_INS_H
#define __W25Q64_INS_H

/*----------------------------------
  基础操作指令组
  ----------------------------------*/
#define W25Q64_WRITE_ENABLE              0x06  // 写使能指令（所有写/擦除操作前必须发送）
#define W25Q64_WRITE_DISABLE             0x04  // 写禁止指令（保护Flash数据）
#define W25Q64_READ_STATUS_REGISTER_1    0x05  // 读状态寄存器1（包含忙标志位、写使能标志等）
#define W25Q64_READ_STATUS_REGISTER_2    0x35  // 读状态寄存器2（包含QE位等高级配置）
#define W25Q64_WRITE_STATUS_REGISTER     0x01  // 写状态寄存器（同时配置SR1和SR2）

/*----------------------------------
  存储器操作指令组
  ----------------------------------*/
#define W25Q64_PAGE_PROGRAM              0x02  // 页编程指令（最大写入256字节，需页对齐）
#define W25Q64_QUAD_PAGE_PROGRAM         0x32  // 四线页编程指令（QSPI模式）
#define W25Q64_BLOCK_ERASE_64KB          0xD8  // 64KB块擦除（地址需64KB对齐）
#define W25Q64_BLOCK_ERASE_32KB          0x52  // 32KB块擦除（地址需32KB对齐）
#define W25Q64_SECTOR_ERASE_4KB          0x20  // 4KB扇区擦除（最小擦除单位）
#define W25Q64_CHIP_ERASE                0xC7  // 全片擦除指令（耗时约30-150秒）
#define W25Q64_ERASE_SUSPEND             0x75  // 擦除暂停（允许执行其他操作）
#define W25Q64_ERASE_RESUME              0x7A  // 擦除恢复（继续被暂停的擦除操作）

/*----------------------------------
  电源管理模式指令
  ----------------------------------*/
#define W25Q64_POWER_DOWN                0xB9  // 进入低功耗模式（电流降至5μA）
#define W25Q64_HIGH_PERFORMANCE_MODE     0xA3  // 启用高性能模式（提升访问速度）
#define W25Q64_CONTINUOUS_READ_MODE_RESET 0xFF // 退出连续读取模式

/*----------------------------------
  设备识别指令组
  ----------------------------------*/
#define W25Q64_RELEASE_POWER_DOWN_HPM_DEVICE_ID 0xAB  // 唤醒设备并读取ID（低功耗模式唤醒用）
#define W25Q64_MANUFACTURER_DEVICE_ID    0x90  // 读取制造商ID（返回Winbond 0xEF + 设备ID）
#define W25Q64_READ_UNIQUE_ID            0x4B  // 读取64位唯一标识符（出厂烧写不可修改）
#define W25Q64_JEDEC_ID                  0x9F  // 标准JEDEC ID（返回制造商ID+存储器类型+容量）

/*----------------------------------
  数据读取模式指令组
  ----------------------------------*/
#define W25Q64_READ_DATA                 0x03  // 标准SPI读取（单线模式，默认时钟频率）
#define W25Q64_FAST_READ                 0x0B  // 快速读取（带8 dummy clocks）
#define W25Q64_FAST_READ_DUAL_OUTPUT     0x3B  // 双线输出读取（CLK速率翻倍）
#define W25Q64_FAST_READ_DUAL_IO         0xBB  // 双线IO读取（地址和数据共用双线）
#define W25Q64_FAST_READ_QUAD_OUTPUT     0x6B  // 四线输出读取（CLK速率4倍）
#define W25Q64_FAST_READ_QUAD_IO         0xEB  // 四线IO读取（地址和数据共用四线）
#define W25Q64_OCTAL_WORD_READ_QUAD_IO   0xE3  // 八字节四线IO读取（最高性能模式）

/*----------------------------------
  通用配置参数
  ----------------------------------*/
#define W25Q64_DUMMY_BYTE                0xFF  // 无用数据

#endif
