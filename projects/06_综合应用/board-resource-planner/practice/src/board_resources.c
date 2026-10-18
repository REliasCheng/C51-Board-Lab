#include "board_resources.h"

typedef struct {
    const char *name;
    BoardResourceMask resources;
} BoardModuleInfo;

static const BoardModuleInfo MODULES[BOARD_MODULE_COUNT] = {
    {"LED", BOARD_RES_P2_0_1 | BOARD_RES_P2_2_4 | BOARD_RES_P2_5_7},
    {"seven-segment", BOARD_RES_P0_BUS | BOARD_RES_P2_2_4 | BOARD_RES_DISPLAY_SWITCH},
    {"LED matrix", BOARD_RES_P0_BUS | BOARD_RES_P3_4_6 | BOARD_RES_DISPLAY_SWITCH},
    {"LCD1602", BOARD_RES_P0_BUS | BOARD_RES_P2_5_7},
    {"independent keys", BOARD_RES_P3_0_1 | BOARD_RES_P3_2},
    {"matrix keypad", BOARD_RES_P1_BUS},
    {"infrared", BOARD_RES_P3_2 | BOARD_RES_INT0},
    {"UART", BOARD_RES_P3_0_1 | BOARD_RES_TIMER1},
    {"AT24C02", BOARD_RES_P2_0_1},
    {"DS1302", BOARD_RES_P3_4_6},
    {"XPT2046", BOARD_RES_P3_4_6 | BOARD_RES_P3_7},
    {"DS18B20", BOARD_RES_P3_7},
    {"buzzer", BOARD_RES_P2_5_7},
    {"Timer0 tick", BOARD_RES_TIMER0}
};

static bool valid_module(BoardModule module)
{
    return module >= BOARD_MODULE_LED && module < BOARD_MODULE_COUNT;
}

static void clear_conflict(BoardConflict *conflict)
{
    if (conflict != NULL) {
        conflict->incoming = BOARD_MODULE_INVALID;
        conflict->existing = BOARD_MODULE_INVALID;
        conflict->shared_resources = UINT32_C(0);
    }
}

void board_plan_init(BoardPlan *plan)
{
    if (plan != NULL) {
        size_t i;

        plan->count = 0U;
        for (i = 0U; i < BOARD_PLAN_CAPACITY; ++i) {
            plan->modules[i] = BOARD_MODULE_INVALID;
        }
    }
}

const char *board_module_name(BoardModule module)
{
    return valid_module(module) ? MODULES[module].name : "invalid";
}

BoardResourceMask board_module_resources(BoardModule module)
{
    return valid_module(module) ? MODULES[module].resources : UINT32_C(0);
}

const char *board_resource_name(BoardResourceMask resource)
{
    switch (resource) {
    case BOARD_RES_P0_BUS:
        return "P0 data bus";
    case BOARD_RES_P1_BUS:
        return "P1 data bus";
    case BOARD_RES_P2_0_1:
        return "P2.0/P2.1";
    case BOARD_RES_P2_2_4:
        return "P2.2-P2.4";
    case BOARD_RES_P2_5_7:
        return "P2.5-P2.7";
    case BOARD_RES_P3_0_1:
        return "P3.0/P3.1";
    case BOARD_RES_P3_2:
        return "P3.2";
    case BOARD_RES_P3_4_6:
        return "P3.4-P3.6";
    case BOARD_RES_P3_7:
        return "P3.7";
    case BOARD_RES_TIMER0:
        return "Timer0";
    case BOARD_RES_TIMER1:
        return "Timer1";
    case BOARD_RES_INT0:
        return "INT0";
    case BOARD_RES_DISPLAY_SWITCH:
        return "J24 display mode";
    default:
        return NULL;
    }
}

const char *board_plan_result_name(BoardPlanResult result)
{
    switch (result) {
    case BOARD_PLAN_OK:
        return "OK";
    case BOARD_PLAN_INVALID_ARGUMENT:
        return "INVALID_ARGUMENT";
    case BOARD_PLAN_INVALID_MODULE:
        return "INVALID_MODULE";
    case BOARD_PLAN_DUPLICATE_MODULE:
        return "DUPLICATE_MODULE";
    case BOARD_PLAN_CAPACITY_EXCEEDED:
        return "CAPACITY_EXCEEDED";
    case BOARD_PLAN_RESOURCE_CONFLICT:
        return "RESOURCE_CONFLICT";
    default:
        return "UNKNOWN_RESULT";
    }
}

BoardPlanResult board_plan_add(BoardPlan *plan, BoardModule module,
                               BoardConflict *conflict)
{
    size_t i;
    BoardResourceMask incoming;

    clear_conflict(conflict);
    if (plan == NULL) {
        return BOARD_PLAN_INVALID_ARGUMENT;
    }
    if (!valid_module(module)) {
        return BOARD_PLAN_INVALID_MODULE;
    }
    if (plan->count > BOARD_PLAN_CAPACITY) {
        return BOARD_PLAN_CAPACITY_EXCEEDED;
    }

    for (i = 0U; i < plan->count; ++i) {
        if (!valid_module(plan->modules[i])) {
            return BOARD_PLAN_INVALID_ARGUMENT;
        }
    }
    for (i = 0U; i < plan->count; ++i) {
        if (plan->modules[i] == module) {
            return BOARD_PLAN_DUPLICATE_MODULE;
        }
    }
    if (plan->count == BOARD_PLAN_CAPACITY) {
        return BOARD_PLAN_CAPACITY_EXCEEDED;
    }

    incoming = board_module_resources(module);
    for (i = 0U; i < plan->count; ++i) {
        BoardModule existing = plan->modules[i];
        BoardResourceMask shared = incoming & board_module_resources(existing);

        if (shared != UINT32_C(0)) {
            if (conflict != NULL) {
                conflict->incoming = module;
                conflict->existing = existing;
                conflict->shared_resources = shared;
            }
            return BOARD_PLAN_RESOURCE_CONFLICT;
        }
    }
    plan->modules[plan->count++] = module;
    return BOARD_PLAN_OK;
}
