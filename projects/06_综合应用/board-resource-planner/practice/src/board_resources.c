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

void board_plan_init(BoardPlan *plan)
{
    if (plan != NULL) {
        plan->count = 0U;
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

bool board_plan_add(BoardPlan *plan, BoardModule module, BoardConflict *conflict)
{
    size_t i;
    BoardResourceMask incoming;

    if (plan == NULL || !valid_module(module) || plan->count >= BOARD_PLAN_CAPACITY) {
        return false;
    }
    incoming = board_module_resources(module);
    for (i = 0U; i < plan->count; ++i) {
        BoardModule existing = plan->modules[i];
        BoardResourceMask shared;
        if (existing == module) {
            return true;
        }
        shared = incoming & board_module_resources(existing);
        if (shared != UINT32_C(0)) {
            if (conflict != NULL) {
                conflict->incoming = module;
                conflict->existing = existing;
                conflict->shared_resources = shared;
            }
            return false;
        }
    }
    plan->modules[plan->count++] = module;
    return true;
}
