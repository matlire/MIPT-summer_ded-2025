#include "test_core.h"

extern test_case_t __start_tests[];
extern test_case_t __stop_tests[];

int run_all_tests(void)
{
    int failed = 0;
    test_case_t *t; 
    for (t = __start_tests; t < __stop_tests; t++) {
        t->func();
    } 
    return failed;
}
