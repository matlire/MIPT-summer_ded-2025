#include "quadratic.h"

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

uint8_t quadratic_parse_input(eq_t *const eq)
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
    print_clear_formatting();
    char str[MAX_CHARS];
    fgets(str, MAX_CHARS, stdin);
    uint8_t result = parse_eq_input(str, eq);

    switch (result)
    {
        case ERROR_INCORRECT_FORMATTING:
            print_clear_formatting();
            print_colored(COLOR_FORE_WHITE, COLOR_BACK_RED, "Incorrect equation!\n");
            print_clear_formatting();
            break;
        case ERROR_SCARYY:
            print_clear_formatting();
            print_colored(COLOR_FORE_WHITE, COLOR_BACK_YELLOW, "Spooky scary skeleton error!\n");
            print_clear_formatting();
            break;
        case ERROR_INCORRECT_INPUT:
            print_clear_formatting();
            print_colored(COLOR_FORE_WHITE, COLOR_BACK_RED, "Incorrect input formattinq!\n");
            print_clear_formatting();
            break;
        default: break;
    }

    return result;
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
            fprintf(stream, "%c = %.5lf\n", eq->to_find, eq->r1);
            break;
        case TWO_ROOTS:
            fprintf(stream, "%c1 = %.5lf %c2 = %.5lf\n", eq->to_find, eq->r1, eq->to_find, eq->r2);
            break;
        case INFINITE_ROOTS:
            fprintf(stream, "Infinite roots!\n");
            break;
        default:
            fprintf(stream, "Some error\n");
    }
}

