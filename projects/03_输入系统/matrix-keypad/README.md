# 4×4 矩阵键盘

矩阵键盘使用 P1 的高、低半字节组成 4 行 × 4 列网络。软件逐行拉低输出并读取列输入，用 8 根引脚识别 16 个按键。

## Hardware Overview

| 信号 | MCU 引脚 | 方向 |
| --- | --- | --- |
| Row 1~4 | P1.3~P1.0 | 扫描时依次输出低电平 |
| Column 1~4 | P1.7~P1.4 | 读取被按键连接后的低电平 |

![矩阵键盘连接](../../../assets/images/diagram/matrix-keypad-wiring.png)

P1 还连接电机/扩展接口。矩阵扫描会反复改写整个端口，接入其他模块前需要核对复用关系。

## Signal Flow

```text
P1 row output
    ↓ pressed switch closes one row-column path
P1 column input
    ↓ row + column mapping
key value 1~16
    ↓
LCD1602 display
```

## Software Structure

```text
main.c
  ├─ LCD_Init()
  └─ MatrixKey_Scan()
       ├─ drive one row low
       ├─ read four columns
       ├─ debounce and wait for release
       └─ return mapped key value
```

`MatrixKey.c/.h` 处理扫描，`LCD1602.c/.h` 负责结果输出，`delay.c/.h` 提供消抖等待。

## Key Implementation

`MatrixKey_Scan()` 每轮先写 `P1=0xFF`，再依次拉低 P1.3、P1.2、P1.1、P1.0。对应列 P1.7~P1.4 出现低电平时，通过固定映射返回 1、5、9、13 等键值。

当前实现使用 20 ms 消抖，并在函数内部等待按键释放。这种方式会暂停显示刷新和其他任务；事件化版本可由周期任务采样端口，并根据前后状态产生 press/release 事件。

## Engineering Value

矩阵键盘把硬件连线、GPIO 方向切换、键值映射和消抖组合在一个输入模块中。后续系统可以在不改变应用逻辑的前提下，把阻塞扫描替换为非阻塞事件源。

## Debug Record

- 键值错位：核对原理图中的行列顺序与软件映射；
- 单行或单列失效：检查对应 P1 引脚和连接网络；
- 连续任务卡顿：检查等待释放循环是否长期占用 CPU；
- 电机接口异常动作：确认 P1 没有同时连接其他执行器。

## Source

工程入口：`course/matrix-scan/`。
