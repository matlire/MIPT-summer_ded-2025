#include "../test_core.h"
#include "../../quadratic/quadratic.h"

TEST(test_two_roots_1) {
    eq_t eq = {.a = 1, .b = 5, .c = 6};
    quadratic_calc_roots(&eq);
    ASSERT_EQ_INT(TWO_ROOTS, eq.root_num);
    ASSERT_EQ_DOUBLE(-2.0, eq.r1, EPSILON);
    ASSERT_EQ_DOUBLE(-3.0, eq.r2, EPSILON);
}

TEST(test_two_roots_2) {
    eq_t eq = {.a = 1, .b = 3, .c = 2};
    quadratic_calc_roots(&eq);
    ASSERT_EQ_INT(TWO_ROOTS, eq.root_num);
    ASSERT_EQ_DOUBLE(-1.0, eq.r1, EPSILON);
    ASSERT_EQ_DOUBLE(-2.0, eq.r2, EPSILON);
}

TEST(test_two_roots_3) {
    eq_t eq = {.a = 1, .b = -7, .c = 12};
    quadratic_calc_roots(&eq);
    ASSERT_EQ_INT(TWO_ROOTS, eq.root_num);
    ASSERT_EQ_DOUBLE(4.0, eq.r1, EPSILON);
    ASSERT_EQ_DOUBLE(3.0, eq.r2, EPSILON);
}

TEST(test_one_root_1) {
    eq_t eq = {.a = 0, .b = 2, .c = 3};
    quadratic_calc_roots(&eq);
    ASSERT_EQ_INT(ONE_ROOT, eq.root_num);
    ASSERT_EQ_DOUBLE(-1.5, eq.r1, EPSILON);
}

TEST(test_one_root_2) {
    eq_t eq = {.a = 0, .b = 5, .c = 6};
    quadratic_calc_roots(&eq);
    ASSERT_EQ_INT(ONE_ROOT, eq.root_num);
    ASSERT_EQ_DOUBLE(-1.2, eq.r1, EPSILON);
}

TEST(test_no_roots) {
    eq_t eq = {.a = 0, .b = 0, .c = 6};
    quadratic_calc_roots(&eq);
    ASSERT_EQ_INT(NO_ROOTS, eq.root_num);
}

TEST(test_inf_roots) {
    eq_t eq = {.a = 0, .b = 0, .c = 0};
    quadratic_calc_roots(&eq);
    ASSERT_EQ_INT(INFINITE_ROOTS, eq.root_num);
}
