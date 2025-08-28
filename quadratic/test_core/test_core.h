#ifndef TEST_CORE_H
#define TEST_CORE_H

#include <stdio.h>
#include <stdlib.h>

#include "logging.h"
#include "colors.h"

/*
    Template for test funciton
*/
typedef void (*test_func_t) (void);

/*
    Structire for test case
*/
typedef struct {
    const char *name;
    test_func_t func;
} test_case_t;

/*
    Assert double with epsilon
*/
#define ASSERT_EQ_DOUBLE(expected, actual, eps)                           \
    {                                                                     \
        if (fabs((expected) - (actual)) > (eps)) {                        \
            log_printf(FATAL, "[TEST FAIL] %s:%d: expected %lf, got %lf", \
            __FILE__, __LINE__, (double)(expected), (double)(actual));    \
            print_colored(COLOR_FORE_WHITE, COLOR_BACK_RED, "");          \
            printf("[TEST FAIL] %s:%d: expected %lf, got %lf",            \
            __FILE__, __LINE__, (double)(expected), (double)(actual));    \
            exit(1);                                                      \
        }                                                                 \
    }

/*
    Assert ints
*/
#define ASSERT_EQ_INT(expected, actual)                                 \
    {                                                                   \
        if ((expected) != (actual)) {                                   \
            log_printf(FATAL, "[TEST FAIL] %s:%d: expected %d, got %d", \
            __FILE__, __LINE__, (int)(expected), (int)(actual));        \
            print_colored(COLOR_FORE_WHITE, COLOR_BACK_RED, "");        \
            printf("[TEST FAIL] %s:%d: expected %d, got %d\n",          \
            __FILE__, __LINE__, (int)(expected), (int)(actual));        \
            exit(1);                                                    \
        }                                                               \
    }

/*
    Test registration function
    Parameters:
        name - test case name
*/
#define TEST(name)                                                                             \
    void name(void);                                                                           \
    static test_case_t _##name##_test __attribute__((used, section("tests"))) = {#name, name}; \
    void name(void)

/*
    Run all created tests
*/
int run_all_tests(void);

#endif
