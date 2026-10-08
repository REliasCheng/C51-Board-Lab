# Board Resource Planner

这个个人实践把开发板原理图中的端口复用关系转换为可执行的资源表。选择多个模块时，程序检查它们是否占用同一组 GPIO、定时器或显示跳线，避免组合工程建立后才发现硬件冲突。

## 资源模型

| 资源组 | 典型模块 |
| --- | --- |
| P0 数据总线 | 数码管、LED 点阵、LCD1602 |
| P2.0/P2.1 | LED、AT24C02 |
| P2.2~P2.4 | LED、74HC138 数码管位选 |
| P2.5~P2.7 | LED、LCD 控制、蜂鸣器 |
| P3.0/P3.1 | UART、独立按键 |
| P3.2 / INT0 | 独立按键、红外输入 |
| P3.4~P3.6 | 74HC595、DS1302、XPT2046 |
| P3.7 | XPT2046、DS18B20 |
| J24 显示模式 | 数码管、LED 点阵 |

映射来自[MCU 资源分配](../../../docs/MCU资源分配.md)和[外设连接关系](../../../docs/外设连接关系.md)。资源表采用保守策略：共享同一端口组即报告冲突，是否能通过分时复用解决由应用设计进一步判断。

## 代码结构

```text
BoardModule
    ↓ lookup
BoardResourceMask
    ↓ compare with selected modules
BoardPlanResult
    ↓
accepted plan / conflict details / explicit error
```

- `practice/include/board_resources.h`：模块、资源位图和冲突结果接口。
- `practice/src/board_resources.c`：板级资源表和组合检查。
- `practice/src/main.c`：数码管、UART、按键、EEPROM 和 LED 的组合示例。
- `tests/test_board_resources.c`：验证兼容组合、资源冲突、错误优先级、输出合同与失败状态不变性。

## 结果与冲突合同

`board_plan_add()` 返回 `BoardPlanResult`：

| 结果 | 语义 |
| --- | --- |
| `BOARD_PLAN_OK` | 模块已提交到规划表 |
| `BOARD_PLAN_INVALID_ARGUMENT` | `BoardPlan` 指针无效 |
| `BOARD_PLAN_INVALID_MODULE` | 模块枚举不在当前资源表中 |
| `BOARD_PLAN_DUPLICATE_MODULE` | 模块已经存在 |
| `BOARD_PLAN_CAPACITY_EXCEEDED` | 固定容量规划表已满或状态超界 |
| `BOARD_PLAN_RESOURCE_CONFLICT` | 与第一个冲突的既有模块共享资源 |

`BoardConflict *` 是可选输出。每次调用都会先清空它；只有 `BOARD_PLAN_RESOURCE_CONFLICT` 返回时可以读取冲突模块与资源位图。所有失败均保持现有模块数量和顺序不变。容量已满时，错误优先级为 duplicate → capacity → resource conflict。

## 构建

从仓库根目录执行：

```powershell
python scripts/run_host_tests.py --compiler gcc
```

该入口以 C11 严格警告选项构建并执行测试和示例；Linux 或已安装 Clang 的主机可把编译器参数改为 `clang`。程序只检查静态资源占用，不代替 Keil C51 构建、跳线检查、电气兼容性分析或板端测试。
