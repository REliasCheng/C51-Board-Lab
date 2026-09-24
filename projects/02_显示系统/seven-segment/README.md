# 七段数码管

## 功能介绍

项目包含段码输出、动态扫描和 `Nixie_Disp()` 模块化三个版本，用 8 位数码管显示数字。

## 硬件结构

```text
P0 ──> 74HC245 ──> a~g / dp
P2.2~P2.4 ──> 74HC138 ──> digit select 0~7
J24 ──> seven-segment / LED matrix selection
```

![段码总线](../../../assets/images/seven-segment-data-bus.png)

## 软件结构

```text
main
 └─ Nixie / Nixie_Disp
     ├─ position decode
     ├─ segment table lookup
     └─ blanking delay
```

## 关键代码

显示位置被转换成 P2.2~P2.4 的三位地址，段码表再把数字映射到 P0。动态版本轮询多位数码管，并在切换位选时清空 P0，减少重影。

## 调试记录

数码管无显示时先检查 J24 是否位于数码管侧。若只亮部分段，再分别检查 P0 段码、74HC245 和 P2.2~P2.4 位选。

## 技术总结

数码管实验把数据总线与片选控制分开，展示了有限 GPIO 下的时间复用显示。

## Source

`course/` 保存驱动、动态扫描和模块化三个课程版本。
