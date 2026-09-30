#ifndef BOARD_RESOURCES_H
#define BOARD_RESOURCES_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define BOARD_PLAN_CAPACITY 16U

typedef uint32_t BoardResourceMask;

enum {
    BOARD_RES_P0_BUS = UINT32_C(1) << 0,
    BOARD_RES_P1_BUS = UINT32_C(1) << 1,
    BOARD_RES_P2_0_1 = UINT32_C(1) << 2,
    BOARD_RES_P2_2_4 = UINT32_C(1) << 3,
    BOARD_RES_P2_5_7 = UINT32_C(1) << 4,
    BOARD_RES_P3_0_1 = UINT32_C(1) << 5,
    BOARD_RES_P3_2 = UINT32_C(1) << 6,
    BOARD_RES_P3_4_6 = UINT32_C(1) << 7,
    BOARD_RES_P3_7 = UINT32_C(1) << 8,
    BOARD_RES_TIMER0 = UINT32_C(1) << 9,
    BOARD_RES_TIMER1 = UINT32_C(1) << 10,
    BOARD_RES_INT0 = UINT32_C(1) << 11,
    BOARD_RES_DISPLAY_SWITCH = UINT32_C(1) << 12
};

typedef enum {
    BOARD_MODULE_LED,
    BOARD_MODULE_SEVEN_SEGMENT,
    BOARD_MODULE_LED_MATRIX,
    BOARD_MODULE_LCD1602,
    BOARD_MODULE_INDEPENDENT_KEYS,
    BOARD_MODULE_MATRIX_KEYPAD,
    BOARD_MODULE_INFRARED,
    BOARD_MODULE_UART,
    BOARD_MODULE_AT24C02,
    BOARD_MODULE_DS1302,
    BOARD_MODULE_XPT2046,
    BOARD_MODULE_DS18B20,
    BOARD_MODULE_BUZZER,
    BOARD_MODULE_TIMER0_TICK,
    BOARD_MODULE_COUNT
} BoardModule;

typedef struct {
    BoardModule modules[BOARD_PLAN_CAPACITY];
    size_t count;
} BoardPlan;

typedef struct {
    BoardModule incoming;
    BoardModule existing;
    BoardResourceMask shared_resources;
} BoardConflict;

void board_plan_init(BoardPlan *plan);
bool board_plan_add(BoardPlan *plan, BoardModule module, BoardConflict *conflict);
const char *board_module_name(BoardModule module);
BoardResourceMask board_module_resources(BoardModule module);

#endif
