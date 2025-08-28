#include "quadratic.h"

bool is_zero (const double value)
{
    log_printf(DEBUG, "Is zero for %.5lf", value);
    return (0 <= fabs(value) && fabs(value) <= EPSILON);
}

static uint8_t linear_solve (eq_t *const eq)
{
    log_printf(INFO, "Linear solve for eq_b=%.5lf eq_c=%.5lf", eq->b, eq->c);
    eq->r1 = (-eq->c) / eq->b;
    return ONE_ROOT;
}

static uint8_t quadratic_solve (eq_t *const eq)
{
    eq->d = eq->b * eq->b - 4 * eq->a * eq->c;
    log_printf(INFO, "Quadratic solving for eq_a=%.5lf eq_b=%.5lf eq_c=%.5lf eq_d=%.5lf", \
                            eq->a, eq->b, eq->c, eq->d);
    // d < 0
    if (eq->d < 0)
    {
        log_printf(INFO, "No roots for eq_a=%.5lf eq_b=%.5lf eq_c=%.5lf eq_d=%.5lf", \
                            eq->a, eq->b, eq->c, eq->d);
        return NO_ROOTS;
    }

    eq->d = sqrt(eq->d);
    // d = 0
    if (eq->d == 0)
    {
        eq->r1 = (-eq->b + eq->d) / (2 * eq->a);
        log_printf(INFO, "One root=%.2lf for eq_a=%.5lf eq_b=%.5lf eq_c=%.5lf eq_d=%.5lf", \
                            eq->r1, eq->a, eq->b, eq->c, eq->d);
        return ONE_ROOT;
    // d > 0
    } else {
        eq->r1 = (-eq->b + eq->d) / (2 * eq->a);
        eq->r2 = (-eq->b - eq->d) / (2 * eq->a);
        log_printf(INFO, "Two roots r1=%.5lf r2=%.5lf for eq_a=%.5lf eq_b=%.5lf eq_c=%.5lf eq_d=%.5lf", \
                            eq->r1, eq->r2, eq->a, eq->b, eq->c, eq->d);
        return TWO_ROOTS;
    }
    return eq->root_num;
}

uint8_t quadratic_calc_roots(eq_t *const eq)
{
    log_printf(DEBUG, "Calcing roots for eq_a=%.5lf eq_b=%.5lf eq_c=%.5lf eq_d=%.5lf", \
                            eq->a, eq->b, eq->c, eq->d);

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
    log_printf(DEBUG, "Clearing debug buffer...");
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

uint8_t quadratic_parse_input(eq_t *const eq)
{
    print_clear_formatting();

    printf("Enter equation in generally accepted format: ");
    print_clear_formatting();
    char str[MAX_CHARS];
    fgets(str, MAX_CHARS, stdin);
    uint8_t result = parse_eq_input(str, eq);
    log_printf(INFO, "Got equation from user: %s", str);

    switch (result)
    {
        case ERROR_INCORRECT_FORMATTING:
            log_printf(ERROR, "Incorrect equation formatting!");
            print_clear_formatting();
            print_colored(COLOR_FORE_WHITE, COLOR_BACK_RED, "Incorrect equation formatting!\n");
            print_clear_formatting();
            break;
        case ERROR_SCARYY:
            log_printf(ERROR, "Spooky scary skeleton error!;)))");
            print_clear_formatting();
            print_colored(COLOR_FORE_WHITE, COLOR_BACK_YELLOW, "Spooky scary skeleton error!;)))\n");
            print_clear_formatting();
            break;
        case ERROR_INCORRECT_INPUT:
            log_printf(ERROR, "Incorrect input formatting!");
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
            log_printf(INFO, "Solved equation with no roots!");
            fprintf(stream, "No roots!\n");
            break;
        case ONE_ROOT:
            log_printf(INFO, "Solved equation with one root %c=%.5lf", eq->to_find, eq->r1);
            fprintf(stream, "%c = %.5lf\n", eq->to_find, eq->r1);
            break;
        case TWO_ROOTS:
            log_printf(INFO, "Solved equation with two roots %c1=%.5lf %c2=%.5lf", \
                                eq->to_find, eq->r1, eq->to_find, eq->r2);
            fprintf(stream, "%c1 = %.5lf %c2 = %.5lf\n", eq->to_find, eq->r1, eq->to_find, eq->r2);
            break;
        case INFINITE_ROOTS:
            log_printf(INFO, "Solved equation with no roots!");
            fprintf(stream, "Infinite roots!\n");
            break;
        default:
            fprintf(stream, "Some error\n");
    }
}

