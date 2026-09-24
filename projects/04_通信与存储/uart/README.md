# UART

## 功能介绍

发送工程上电发送 `0xAA`；接收工程通过串口中断接收字节，回显到上位机并把反码写入 P2。

## 硬件结构

```text
MCU P3.1 / TXD ──> CH340C ──> USB
MCU P3.0 / RXD <── CH340C <── USB
Timer1 ───────────> baud-rate generator
```

## 软件结构

`Uart1_Init()` 配置 SCON、PCON、Timer1 和中断；`UART_SendByte()` 轮询 TI；接收版由 `Uart1_Isr()` 处理 RI。

## 关键代码

Timer1 工作在模式 2，课程参数按 11.0592 MHz 生成 9600 baud。发送完成后软件清 TI，接收 ISR 清 RI 后读取 SBUF。

## 调试记录

接收 ISR 内调用阻塞发送函数进行回显，会延长中断时间。多外设版本应改成“ISR 收字节 → 缓冲/标志 → 主循环发送”。P3.0/P3.1 与独立按键复用，串口工作时不要同时按普通输入口使用。

## 技术总结

两个工程覆盖了 8051 串口寄存器、Timer1 波特率和中断接收的完整最小链路。

## Source

`course/01_transmit/` 和 `course/02_receive-echo/` 为课程原始工程。
