#include "test_core.h"

extern test_case_t __start_tests[];
extern test_case_t __stop_tests[];

int run_all_tests(void)
{
    int failed = 0;
    test_case_t *t;
    print_colored(COLOR_FORE_BLACK, COLOR_BACK_YELLOW, "RUNNING TESTS...\n");
    for (t = __start_tests; t < __stop_tests; t++) {
        t->func();
    }
    print_colored(COLOR_FORE_WHITE, COLOR_BACK_GREEN, "OK\n\n");
    return failed;
}
