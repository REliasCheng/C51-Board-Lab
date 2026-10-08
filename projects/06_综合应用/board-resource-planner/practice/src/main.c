#include "board_resources.h"

#include <inttypes.h>
#include <stdio.h>

static void add_module(BoardPlan *plan, BoardModule module)
{
    BoardConflict conflict;
    BoardPlanResult result = board_plan_add(plan, module, &conflict);

    if (result == BOARD_PLAN_OK) {
        printf("add %-18s : OK\n", board_module_name(module));
    } else if (result == BOARD_PLAN_RESOURCE_CONFLICT) {
        printf("add %-18s : CONFLICT with %s (mask=0x%08" PRIx32 ")\n",
               board_module_name(module), board_module_name(conflict.existing),
               conflict.shared_resources);
    } else {
        printf("add %-18s : %s\n", board_module_name(module),
               board_plan_result_name(result));
    }
}

int main(void)
{
    BoardPlan plan;
    board_plan_init(&plan);

    add_module(&plan, BOARD_MODULE_SEVEN_SEGMENT);
    add_module(&plan, BOARD_MODULE_UART);
    add_module(&plan, BOARD_MODULE_INDEPENDENT_KEYS);
    add_module(&plan, BOARD_MODULE_AT24C02);
    add_module(&plan, BOARD_MODULE_LED);
    return 0;
}
