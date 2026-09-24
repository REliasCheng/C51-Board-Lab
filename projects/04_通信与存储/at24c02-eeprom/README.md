# AT24C02 EEPROM

## 功能介绍

软件 I²C 驱动读写 AT24C02 地址 0。示例读取计数值、更新并写回，再通过 LCD1602 显示。

## 硬件结构

```text
P2.1 ──> SCL
P2.0 <─> SDA
        AT24C02
```

![AT24C02 接口](../../../assets/images/at24c02-interface.png)

## 软件结构

`i2c.c` 同时实现总线时序和 AT24C02 字节接口；`main.c` 负责读取、修改、写入和显示。

## 关键代码

- `I2C_Start()` / `I2C_Stop()` 控制总线条件；
- `I2C_SendByte()` / `I2C_ReceiveByte()` 按位传输；
- `I2C_SendAck()` 产生 ACK/NACK；
- `AT24C02_WriteByte()` / `ReadByte()` 组合器件地址和存储地址。

## 调试记录

课程流程没有系统向上层报告 ACK 失败，写完成主要依靠固定等待。后续个人驱动应增加 ACK polling、超时和错误返回。P2.0/P2.1 与 LED 端口复用，通信时 LED 会受到总线波形影响。

## 技术总结

该工程把 GPIO 时序封装为存储器接口，连接了位级通信、器件协议和非易失数据三个层次。

## Source

`course/software-i2c/` 为课程原始工程。
