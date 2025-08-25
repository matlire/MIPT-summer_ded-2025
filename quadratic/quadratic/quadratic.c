#include "quadratic.h"

void init_eq (eq_t *const eq)
{
    // isnan()
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
    return (0 <= fabs(value) && fabs(value) <= EPSILON);
}

static uint8_t linear_solve (eq_t *const eq)
{
    eq->r1 = (-eq->c) / eq->b;
    return ONE_ROOT;
}

static uint8_t quadratic_solve (eq_t *const eq)
{
    eq->d = eq->b * eq->b - 4 * eq->a * eq->c;
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

uint8_t quadratic_calc_roots(eq_t *const eq)
{
    // a = 0, b != 0 => 1 root
    if (is_zero(eq->a) && !is_zero(eq->b))
    {
        eq->root_num = linear_solve(eq);
    // a = 0, b = 0, c = 0 -> inf roots
    } else if (is_zero(eq->a) && is_zero(eq->b) && is_zero(eq->c))
    {
        eq->root_num = INFINITE_ROOTS;
    // a = 0, b = 0, c != 0
    } else if (is_zero(eq->a) && is_zero(eq->b))
    {
        eq->root_num = NO_ROOTS;
    // a != 0, b != 0
    } else { 
        eq->root_num = quadratic_solve(eq);
    }
    return eq->root_num;
}

void clear_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

bool quadratic_parse_input(eq_t *const eq)
{
    print_clear_formatting();
    /* int stage = 0;
    while (stage != 3)
    {
        int result;
        switch (stage)
        {
            case 0:
                printf("Enter first  ('a') koefficient: ");
                result = scanf("%lf", &eq->a);
                break;
            case 1:
                printf("Enter second ('b') koefficient: ");
                result = scanf("%lf", &eq->b);
                break;
            case 2:
                printf("Enter third  ('c') koefficient: ");
                result = scanf("%lf", &eq->c);
                break;
            default: break;
        } 
        clear_buffer();
        if (!result)
        {
            printf("Invalid input, try again!\n");
        } else {
            stage++;
        }
    }
    */
    printf("Enter equation in generally accepted format: ");
    char str[MAX_CHARS];
    fgets(str, MAX_CHARS, stdin);
    parse_eq_input(str, eq);

    return 1;
}

void quadratic_print_output(const eq_t *const eq, FILE *stream)
{
    print_clear_formatting();
    print_colored(COLOR_FORE_WHITE, COLOR_BACK_GREEN, "");
    switch (eq->root_num)
    {
        case NO_ROOTS:
            fprintf(stream, "No roots!\n");
            break;
        case ONE_ROOT:
            fprintf(stream, "x=%.5lf\n", eq->r1);
            break;
        case TWO_ROOTS:
            fprintf(stream, "x1=%.5lf x2=%.5lf\n", eq->r1, eq->r2);
            break;
        case INFINITE_ROOTS:
            fprintf(stream, "Infinite roots!\n");
            break;
        default:
            fprintf(stream, "Some error\n");
    }
}

