#include "board_resources.h"

#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

static void assert_conflict_cleared(const BoardConflict *conflict)
{
    assert(conflict->incoming == BOARD_MODULE_INVALID);
    assert(conflict->existing == BOARD_MODULE_INVALID);
    assert(conflict->shared_resources == UINT32_C(0));
}

static void assert_plan_equal(const BoardPlan *actual, const BoardPlan *expected)
{
    size_t i;

    assert(actual->count == expected->count);
    for (i = 0U; i < BOARD_PLAN_CAPACITY; ++i) {
        assert(actual->modules[i] == expected->modules[i]);
    }
}

static void expect_conflict(BoardModule first, BoardModule second,
                            BoardResourceMask expected_resources)
{
    BoardPlan plan;
    BoardPlan before;
    BoardConflict conflict;

    board_plan_init(&plan);
    assert(board_plan_add(&plan, first, &conflict) == BOARD_PLAN_OK);
    before = plan;
    assert(board_plan_add(&plan, second, &conflict) == BOARD_PLAN_RESOURCE_CONFLICT);
    assert(conflict.existing == first);
    assert(conflict.incoming == second);
    assert(conflict.shared_resources == expected_resources);
    assert_plan_equal(&plan, &before);
}

static void test_initialization(void)
{
    struct {
        uint32_t guard_before;
        BoardPlan plan;
        uint32_t guard_after;
    } guarded;
    BoardConflict conflict;
    size_t i;

    memset(&guarded, 0xA5, sizeof(guarded));
    guarded.guard_before = UINT32_C(0x12345678);
    guarded.guard_after = UINT32_C(0x87654321);
    board_plan_init(NULL);
    board_plan_init(&guarded.plan);

    assert(guarded.plan.count == 0U);
    for (i = 0U; i < BOARD_PLAN_CAPACITY; ++i) {
        assert(guarded.plan.modules[i] == BOARD_MODULE_INVALID);
    }
    assert(guarded.guard_before == UINT32_C(0x12345678));
    assert(guarded.guard_after == UINT32_C(0x87654321));

    assert(board_plan_add(&guarded.plan, BOARD_MODULE_LED, &conflict) == BOARD_PLAN_OK);
    board_plan_init(&guarded.plan);
    assert(guarded.plan.count == 0U);
    for (i = 0U; i < BOARD_PLAN_CAPACITY; ++i) {
        assert(guarded.plan.modules[i] == BOARD_MODULE_INVALID);
    }
}

static void test_single_and_compatible_modules(void)
{
    BoardPlan plan;
    BoardConflict conflict = {
        BOARD_MODULE_UART,
        BOARD_MODULE_LED,
        UINT32_MAX,
    };

    board_plan_init(&plan);
    assert(board_plan_add(&plan, BOARD_MODULE_SEVEN_SEGMENT, &conflict) == BOARD_PLAN_OK);
    assert_conflict_cleared(&conflict);
    assert(board_plan_add(&plan, BOARD_MODULE_UART, NULL) == BOARD_PLAN_OK);
    assert(board_plan_add(&plan, BOARD_MODULE_TIMER0_TICK, &conflict) == BOARD_PLAN_OK);
    assert_conflict_cleared(&conflict);
    assert(plan.count == 3U);
    assert(plan.modules[0] == BOARD_MODULE_SEVEN_SEGMENT);
    assert(plan.modules[1] == BOARD_MODULE_UART);
    assert(plan.modules[2] == BOARD_MODULE_TIMER0_TICK);
}

static void test_known_resource_conflicts(void)
{
    expect_conflict(BOARD_MODULE_UART, BOARD_MODULE_INDEPENDENT_KEYS,
                    BOARD_RES_P3_0_1);
    expect_conflict(BOARD_MODULE_LED, BOARD_MODULE_AT24C02,
                    BOARD_RES_P2_0_1);
    expect_conflict(BOARD_MODULE_LED_MATRIX, BOARD_MODULE_DS1302,
                    BOARD_RES_P3_4_6);
    expect_conflict(BOARD_MODULE_XPT2046, BOARD_MODULE_DS18B20,
                    BOARD_RES_P3_7);
    expect_conflict(BOARD_MODULE_LCD1602, BOARD_MODULE_BUZZER,
                    BOARD_RES_P2_5_7);
    expect_conflict(BOARD_MODULE_SEVEN_SEGMENT, BOARD_MODULE_LED_MATRIX,
                    BOARD_RES_P0_BUS | BOARD_RES_DISPLAY_SWITCH);
}

static void test_first_conflict_is_deterministic(void)
{
    BoardPlan plan;
    BoardConflict conflict;

    board_plan_init(&plan);
    assert(board_plan_add(&plan, BOARD_MODULE_AT24C02, &conflict) == BOARD_PLAN_OK);
    assert(board_plan_add(&plan, BOARD_MODULE_BUZZER, &conflict) == BOARD_PLAN_OK);
    assert(board_plan_add(&plan, BOARD_MODULE_LED, &conflict) ==
           BOARD_PLAN_RESOURCE_CONFLICT);
    assert(conflict.existing == BOARD_MODULE_AT24C02);
    assert(conflict.shared_resources == BOARD_RES_P2_0_1);

    board_plan_init(&plan);
    assert(board_plan_add(&plan, BOARD_MODULE_BUZZER, &conflict) == BOARD_PLAN_OK);
    assert(board_plan_add(&plan, BOARD_MODULE_AT24C02, &conflict) == BOARD_PLAN_OK);
    assert(board_plan_add(&plan, BOARD_MODULE_LED, &conflict) ==
           BOARD_PLAN_RESOURCE_CONFLICT);
    assert(conflict.existing == BOARD_MODULE_BUZZER);
    assert(conflict.shared_resources == BOARD_RES_P2_5_7);
}

static void test_invalid_arguments_and_modules(void)
{
    BoardPlan plan;
    BoardPlan before;
    BoardConflict conflict = {
        BOARD_MODULE_UART,
        BOARD_MODULE_LED,
        UINT32_MAX,
    };

    assert(board_plan_add(NULL, BOARD_MODULE_LED, &conflict) ==
           BOARD_PLAN_INVALID_ARGUMENT);
    assert_conflict_cleared(&conflict);

    board_plan_init(&plan);
    before = plan;
    assert(board_plan_add(&plan, BOARD_MODULE_INVALID, &conflict) ==
           BOARD_PLAN_INVALID_MODULE);
    assert_conflict_cleared(&conflict);
    assert_plan_equal(&plan, &before);
    assert(board_plan_add(&plan, (BoardModule)BOARD_MODULE_COUNT, &conflict) ==
           BOARD_PLAN_INVALID_MODULE);
    assert_plan_equal(&plan, &before);
    assert(board_plan_add(&plan, (BoardModule)INT_MAX, &conflict) ==
           BOARD_PLAN_INVALID_MODULE);
    assert_plan_equal(&plan, &before);

    assert(strcmp(board_module_name(BOARD_MODULE_INVALID), "invalid") == 0);
    assert(board_module_resources(BOARD_MODULE_INVALID) == UINT32_C(0));
    assert(strcmp(board_module_name((BoardModule)INT_MAX), "invalid") == 0);
    assert(board_module_resources((BoardModule)INT_MAX) == UINT32_C(0));
}

static void test_duplicate_module(void)
{
    BoardPlan plan;
    BoardPlan before;
    BoardConflict conflict = {
        BOARD_MODULE_UART,
        BOARD_MODULE_LED,
        UINT32_MAX,
    };

    board_plan_init(&plan);
    assert(board_plan_add(&plan, BOARD_MODULE_UART, &conflict) == BOARD_PLAN_OK);
    before = plan;
    assert(board_plan_add(&plan, BOARD_MODULE_UART, &conflict) ==
           BOARD_PLAN_DUPLICATE_MODULE);
    assert_conflict_cleared(&conflict);
    assert_plan_equal(&plan, &before);
    assert(board_plan_add(&plan, BOARD_MODULE_UART, NULL) ==
           BOARD_PLAN_DUPLICATE_MODULE);
    assert_plan_equal(&plan, &before);
}

static void test_capacity_and_error_priority(void)
{
    struct {
        uint32_t guard_before;
        BoardPlan plan;
        uint32_t guard_after;
    } guarded;
    BoardPlan before;
    BoardConflict conflict;
    size_t i;

    guarded.guard_before = UINT32_C(0x11111111);
    guarded.guard_after = UINT32_C(0x22222222);
    board_plan_init(&guarded.plan);
    for (i = 0U; i < BOARD_PLAN_CAPACITY; ++i) {
        guarded.plan.modules[i] = BOARD_MODULE_LED;
    }
    guarded.plan.count = BOARD_PLAN_CAPACITY;
    before = guarded.plan;

    assert(board_plan_add(&guarded.plan, BOARD_MODULE_LED, &conflict) ==
           BOARD_PLAN_DUPLICATE_MODULE);
    assert_plan_equal(&guarded.plan, &before);
    assert(board_plan_add(&guarded.plan, BOARD_MODULE_AT24C02, &conflict) ==
           BOARD_PLAN_CAPACITY_EXCEEDED);
    assert_conflict_cleared(&conflict);
    assert_plan_equal(&guarded.plan, &before);
    assert(guarded.guard_before == UINT32_C(0x11111111));
    assert(guarded.guard_after == UINT32_C(0x22222222));

    guarded.plan.count = BOARD_PLAN_CAPACITY + 1U;
    assert(board_plan_add(&guarded.plan, BOARD_MODULE_UART, &conflict) ==
           BOARD_PLAN_CAPACITY_EXCEEDED);
    assert(guarded.guard_before == UINT32_C(0x11111111));
    assert(guarded.guard_after == UINT32_C(0x22222222));
}

static void test_failure_state_and_retry(void)
{
    BoardPlan plan;
    BoardPlan before;
    BoardConflict conflict;

    board_plan_init(&plan);
    assert(board_plan_add(&plan, BOARD_MODULE_LED, &conflict) == BOARD_PLAN_OK);
    before = plan;
    assert(board_plan_add(&plan, BOARD_MODULE_AT24C02, &conflict) ==
           BOARD_PLAN_RESOURCE_CONFLICT);
    assert_plan_equal(&plan, &before);

    assert(board_plan_add(&plan, BOARD_MODULE_MATRIX_KEYPAD, &conflict) == BOARD_PLAN_OK);
    assert(plan.count == before.count + 1U);
    assert(plan.modules[1] == BOARD_MODULE_MATRIX_KEYPAD);
}

static void test_conflict_output_reuse_and_optional_output(void)
{
    BoardPlan plan;
    BoardPlan before;
    BoardConflict conflict;

    board_plan_init(&plan);
    assert(board_plan_add(&plan, BOARD_MODULE_SEVEN_SEGMENT, &conflict) == BOARD_PLAN_OK);
    before = plan;

    assert(board_plan_add(&plan, BOARD_MODULE_LED_MATRIX, &conflict) ==
           BOARD_PLAN_RESOURCE_CONFLICT);
    assert(conflict.incoming == BOARD_MODULE_LED_MATRIX);
    assert(conflict.existing == BOARD_MODULE_SEVEN_SEGMENT);
    assert(conflict.shared_resources ==
           (BOARD_RES_P0_BUS | BOARD_RES_DISPLAY_SWITCH));
    assert_plan_equal(&plan, &before);

    assert(board_plan_add(&plan, BOARD_MODULE_SEVEN_SEGMENT, &conflict) ==
           BOARD_PLAN_DUPLICATE_MODULE);
    assert_conflict_cleared(&conflict);
    assert_plan_equal(&plan, &before);

    assert(board_plan_add(&plan, BOARD_MODULE_LED_MATRIX, NULL) ==
           BOARD_PLAN_RESOURCE_CONFLICT);
    assert_plan_equal(&plan, &before);
}

static void test_repeated_planning_is_deterministic(void)
{
    BoardPlan first;
    BoardPlan second;
    BoardConflict conflict;

    board_plan_init(&first);
    board_plan_init(&second);
    assert(board_plan_add(&first, BOARD_MODULE_SEVEN_SEGMENT, &conflict) == BOARD_PLAN_OK);
    assert(board_plan_add(&first, BOARD_MODULE_UART, &conflict) == BOARD_PLAN_OK);
    assert(board_plan_add(&first, BOARD_MODULE_TIMER0_TICK, &conflict) == BOARD_PLAN_OK);
    assert(board_plan_add(&second, BOARD_MODULE_SEVEN_SEGMENT, &conflict) == BOARD_PLAN_OK);
    assert(board_plan_add(&second, BOARD_MODULE_UART, &conflict) == BOARD_PLAN_OK);
    assert(board_plan_add(&second, BOARD_MODULE_TIMER0_TICK, &conflict) == BOARD_PLAN_OK);
    assert_plan_equal(&first, &second);
}

static void test_result_names(void)
{
    assert(strcmp(board_plan_result_name(BOARD_PLAN_OK), "OK") == 0);
    assert(strcmp(board_plan_result_name(BOARD_PLAN_INVALID_ARGUMENT),
                  "INVALID_ARGUMENT") == 0);
    assert(strcmp(board_plan_result_name(BOARD_PLAN_INVALID_MODULE),
                  "INVALID_MODULE") == 0);
    assert(strcmp(board_plan_result_name(BOARD_PLAN_DUPLICATE_MODULE),
                  "DUPLICATE_MODULE") == 0);
    assert(strcmp(board_plan_result_name(BOARD_PLAN_CAPACITY_EXCEEDED),
                  "CAPACITY_EXCEEDED") == 0);
    assert(strcmp(board_plan_result_name(BOARD_PLAN_RESOURCE_CONFLICT),
                  "RESOURCE_CONFLICT") == 0);
    assert(strcmp(board_plan_result_name((BoardPlanResult)INT_MAX),
                  "UNKNOWN_RESULT") == 0);
}

int main(void)
{
    test_initialization();
    test_single_and_compatible_modules();
    test_known_resource_conflicts();
    test_first_conflict_is_deterministic();
    test_invalid_arguments_and_modules();
    test_duplicate_module();
    test_capacity_and_error_priority();
    test_failure_state_and_retry();
    test_conflict_output_reuse_and_optional_output();
    test_repeated_planning_is_deterministic();
    test_result_names();

    puts("board resource planner tests passed (11 cases)");
    return 0;
}
