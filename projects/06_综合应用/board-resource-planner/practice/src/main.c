#include "board_resources.h"

#include <inttypes.h>
#include <stdio.h>
#include <string.h>

static BoardPlanResult add_module(BoardPlan *plan, BoardModule module)
{
    BoardConflict conflict;
    BoardPlanResult result = board_plan_add(plan, module, &conflict);

    if (result == BOARD_PLAN_OK) {
        printf("add %-18s : OK\n", board_module_name(module));
    } else if (result == BOARD_PLAN_RESOURCE_CONFLICT) {
        BoardResourceMask resource;
        int first = 1;

        printf("add %-18s : CONFLICT with %s (mask=0x%08" PRIx32 ", resources=",
               board_module_name(module), board_module_name(conflict.existing),
               conflict.shared_resources);
        for (resource = UINT32_C(1); resource <= BOARD_RES_DISPLAY_SWITCH;
             resource <<= 1) {
            if ((conflict.shared_resources & resource) != UINT32_C(0)) {
                printf("%s%s", first ? "" : ", ", board_resource_name(resource));
                first = 0;
            }
        }
        puts(")");
    } else {
        printf("add %-18s : %s\n", board_module_name(module),
               board_plan_result_name(result));
    }
    return result;
}

static BoardModule find_module(const char *name)
{
    BoardModule module;

    for (module = BOARD_MODULE_LED; module < BOARD_MODULE_COUNT; ++module) {
        if (strcmp(name, board_module_name(module)) == 0) {
            return module;
        }
    }
    return BOARD_MODULE_INVALID;
}

int main(int argc, char **argv)
{
    BoardPlan plan;
    int index;
    int failed = 0;

    if (argc == 2 && strcmp(argv[1], "--list") == 0) {
        BoardModule module;
        for (module = BOARD_MODULE_LED; module < BOARD_MODULE_COUNT; ++module) {
            puts(board_module_name(module));
        }
        return 0;
    }

    /* Validate the complete request before printing any planning result. */
    for (index = 1; index < argc; ++index) {
        if (find_module(argv[index]) == BOARD_MODULE_INVALID) {
            fprintf(stderr, "unknown module: %s (see --list)\n", argv[index]);
            return 2;
        }
    }

    board_plan_init(&plan);

    if (argc > 1) {
        for (index = 1; index < argc; ++index) {
            if (add_module(&plan, find_module(argv[index])) != BOARD_PLAN_OK) {
                failed = 1;
            }
        }
        printf("accepted %zu/%d modules\n", plan.count, argc - 1);
        return failed;
    }

    add_module(&plan, BOARD_MODULE_SEVEN_SEGMENT);
    add_module(&plan, BOARD_MODULE_UART);
    add_module(&plan, BOARD_MODULE_INDEPENDENT_KEYS);
    add_module(&plan, BOARD_MODULE_AT24C02);
    add_module(&plan, BOARD_MODULE_LED);
    return 0;
}
