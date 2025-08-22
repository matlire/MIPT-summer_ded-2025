#include "quadratic.h"

void init_eq (eq_t *const eq)
{
eq->a = 0.0;
    eq->b = 0.0;
    eq->c = 0.0;

    eq->root_num = 0;
    eq->d        = 0.0;
    eq->r1       = 0.0;
    eq->r2       = 0.0;
}

bool is_zero (const double value)
{
    return (0 <= abs(value) && abs(value) <= EPSILON);
}

uint8_t linear_solve (eq_t *const eq)
{
    eq->r1 = (-eq->c) / eq->b;
    return ONE_ROOT;
}

uint8_t quadratic_solve (eq_t *const eq)
{
    eq->d  = eq->b * eq->b - 4 * eq->a * eq->c;
    // d < 0
    if (eq->d < 0)
    {
        return NO_ROOTS;
    }

    eq->d = sqrt(eq->d);
    // d = 0
    if (eq->d == 0)
    {
        eq->r1 = (-eq->b + eq->d) / (2 * eq->a);
        return ONE_ROOT;
    // d > 0
    } else {
        eq->r1 = (-eq->b + eq->d) / (2 * eq->a);
        eq->r2 = (-eq->b - eq->d) / (2 * eq->a);
        return TWO_ROOTS;
    }
    return eq->root_num;
}

uint8_t calc_roots(eq_t *const eq)
{
    // a = 0, b != 0 => 1 root
    if (is_zero(eq->a) && !is_zero(eq->b))
    {
        eq->root_num = linear_solve(eq);
    // a = 0, b = 0
    } else if (is_zero(eq->a) && is_zero(eq->b))
    {
        eq->root_num = NO_ROOTS;
    // a != 0, b != 0
    } else { 
        eq->root_num = quadratic_solve(eq);
    }
    return eq->root_num;
}

bool io_parse_input(eq_t *const eq)
{
    printf("Enter a, b, c\n");
    int result = scanf("%lf %lf %lf", &eq->a, &eq->b, &eq->c); 
    if (!result)
    {
        printf("Invalid input!");
        return 0;
    }
    return 1;
}

void io_print_output(const eq_t *const eq)
{
    switch (eq->root_num)
    {
        case NO_ROOTS:
            printf("No roots!");
            break;
        case ONE_ROOT:
            printf("x=%.5lf", eq->r1);
            break;
        case TWO_ROOTS:
            printf("x1=%.5lf x2=%.5lf", eq->r1, eq->r2);
            break;
        case INFINITE_ROOTS:
            printf("Infinite roots amount!");
            break;
        default:
            printf("Some error");
    }
}

