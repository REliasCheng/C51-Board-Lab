# C51-Board-Lab

面向 8051 兼容教学开发板的板级资源分析与组合工程规划仓库。核心实践 `Board Resource Planner` 把原理图中的端口复用、定时器占用和显示跳线转换为可执行的资源冲突检查，使综合应用在编码前就能发现硬件资源重叠。

## 平台与技术栈

- **处理器范围**：40 引脚 8051 兼容 MCU，代码语境以 `REGX52` / AT89C52 类目标为主
- **典型时钟**：11.0592 MHz，实际使用以板载晶振为准
- **开发方式**：Embedded C / Keil C51；资源规划核心使用 C11 + GCC
- **板级模块**：数码管、LED 点阵、LCD、独立键/矩阵键、UART、AT24C02、DS1302、DS18B20、XPT2046

## 架构说明

```text
原理图与引脚关系
        ↓
BoardModule → BoardResourceMask
        ↓
已选模块集合 → 冲突比较 → 可接受方案 / 冲突报告
```

资源模型采用保守策略：只要两个模块共享同一端口组、定时器或显示模式，就先报告冲突；是否能通过分时复用解决，由具体应用进一步评估。

## 核心功能

- 建立 P0 数据总线、P2 控制线、P3 串行外设与 UART 的复用表。
- 识别数码管、点阵和 LCD 之间的显示资源冲突。
- 检查独立按键与 UART、AT24C02 与 LED、DS1302 与 XPT2046 等组合。
- 通过位图表达模块占用，输出冲突资源和相关模块。
- 用主机端测试覆盖六组已知冲突和一组兼容组合。

## 工程结构

```text
projects/06_综合应用/board-resource-planner/
  practice/include/board_resources.h  资源与结果接口
  practice/src/board_resources.c      板级资源表和冲突检查
  practice/src/main.c                 组合规划示例
  tests/test_board_resources.c        主机端验证
docs/                                 板卡、外设、资源与调试说明
```

## 文档导航

- [开发板架构分析](docs/开发板架构分析.md)
- [外设连接关系](docs/外设连接关系.md)
- [MCU 资源分配](docs/MCU资源分配.md)
- [个人实践路线](docs/个人实践路线.md)
- [Board Resource Planner](projects/06_综合应用/board-resource-planner/README.md)

## 验证范围

资源规划器及其测试已使用 GCC 16.1.0、C11 与 `-Wall -Wextra -Werror -pedantic` 构建并运行通过。

验证对象是静态资源模型，不替代 Keil 目标构建、跳线检查、电气兼容性分析或真实开发板测试；板级结论仍需结合具体板卡版本和原理图复核。

## License Boundary

根目录 [MIT License](LICENSE) 适用于仓库维护者编写的代码与文档。芯片手册、板卡图纸、器件资料以及未随仓库分发的外部实现不因技术引用而纳入该许可。
