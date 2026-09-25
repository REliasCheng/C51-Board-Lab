# UART

两个工程分别实现查询式发送和中断接收。Timer1 生成波特率，P3.0/P3.1 经过 CH340C 与 USB 串口连接。

## Hardware Overview

| 信号 | MCU 引脚 | 板载路径 |
| --- | --- | --- |
| RXD | P3.0 | CH340C TXD → MCU |
| TXD | P3.1 | MCU → CH340C RXD |
| Baud clock | Timer1 | 模式 2 自动重装 |

P3.0/P3.1 同时连接两个独立按键。串口运行时应把它们视为通信引脚，而不是普通按键输入。

## Signal Flow

```text
PC / USB
   ↕ CH340C
P3.0 RXD / P3.1 TXD
   ↕ SBUF
8051 UART
   ↕ RI / TI flags
main loop or interrupt 4
```

## Software Structure

```text
main.c
  ├─ Uart1_Init()
  ├─ UART_SendByte()
  └─ Uart1_Isr() interrupt 4
```

`Uart1_Init()` 配置 SCON、PCON、Timer1 和串口中断。发送函数写 SBUF 后轮询 TI；接收 ISR 检查 RI、读取 SBUF 并清除标志。

## Key Implementation

串口参数按 11.0592 MHz、9600 baud 生成。Timer1 使用模式 2 自动重装，避免每个串口位都由软件重新装载计数器。

接收工程在 ISR 内调用 `UART_SendByte(SBUF)` 完成回显。该函数等待 TI，意味着 ISR 会一直占用 CPU 直到发送完成。可将接收字节写入环形缓冲区，再由主循环或发送状态机处理回显。

## 设计说明

UART 工程覆盖了时钟源、帧收发寄存器、中断标志和 USB-UART 桥接的完整路径。ISR 阻塞问题也提供了从演示代码过渡到缓冲式通信框架的明确改进点。

## Debug Record

- 无串口输出：检查晶振、Timer1 重装值、波特率和 CH340C 端口；
- 重复进入中断：确认软件清除 RI/TI；
- 接收期间其他任务延迟：检查 ISR 中是否执行阻塞发送；
- 按键行为异常：确认 P3.0/P3.1 当前由 UART 占用。

## Source

工程入口：`course/01_transmit/`、`course/02_receive-echo/`。
