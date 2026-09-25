#include "board_resources.h"

#include <assert.h>
#include <stdio.h>

static void expect_conflict(BoardModule first, BoardModule second,
                            BoardResourceMask expected_resource)
{
    BoardPlan plan;
    BoardConflict conflict;
    board_plan_init(&plan);
    assert(board_plan_add(&plan, first, &conflict));
    assert(!board_plan_add(&plan, second, &conflict));
    assert(conflict.existing == first);
    assert(conflict.incoming == second);
    assert((conflict.shared_resources & expected_resource) != 0U);
}

int main(void)
{
    BoardPlan compatible;
    BoardConflict conflict;

    board_plan_init(&compatible);
    assert(board_plan_add(&compatible, BOARD_MODULE_SEVEN_SEGMENT, &conflict));
    assert(board_plan_add(&compatible, BOARD_MODULE_UART, &conflict));
    assert(board_plan_add(&compatible, BOARD_MODULE_TIMER0_TICK, &conflict));
    assert(compatible.count == 3U);

    expect_conflict(BOARD_MODULE_UART, BOARD_MODULE_INDEPENDENT_KEYS, BOARD_RES_P3_0_1);
    expect_conflict(BOARD_MODULE_LED, BOARD_MODULE_AT24C02, BOARD_RES_P2_0_1);
    expect_conflict(BOARD_MODULE_LED_MATRIX, BOARD_MODULE_DS1302, BOARD_RES_P3_4_6);
    expect_conflict(BOARD_MODULE_XPT2046, BOARD_MODULE_DS18B20, BOARD_RES_P3_7);
    expect_conflict(BOARD_MODULE_LCD1602, BOARD_MODULE_BUZZER, BOARD_RES_P2_5_7);
    expect_conflict(BOARD_MODULE_SEVEN_SEGMENT, BOARD_MODULE_LED_MATRIX,
                    BOARD_RES_DISPLAY_SWITCH);

    puts("board resource planner tests passed");
    return 0;
}
