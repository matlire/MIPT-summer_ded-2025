#ifndef QUADRATIC_H
#define QUADRATIC_H

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <fcntl.h>

#include "colors.h"
#include "smart_parse.h"

#define MAX_CHARS 256
#define EPSILON 1e-9

typedef enum 
{
    NO_ROOTS       = 0,
    ONE_ROOT       = 1,
    TWO_ROOTS      = 2,
    INFINITE_ROOTS = 3,
} root_nums;

void init_eq (eq_t *const eq);

/*
    Function compares double with zero
    Parameters:
        value - value
    Return:
        0  - not zero
        1  - is zero
*/
bool is_zero (const double value);

/*
    Function to calculate roots
    Parameters:
        eq - quadratic equation
    Return:
        root number
*/
uint8_t quadratic_calc_roots   (eq_t *const eq);

static uint8_t quadratic_solve (eq_t *const eq);
static uint8_t linear_solve    (eq_t *const eq);

/*
    Funtions for beautiful io interactions with quadratic solving
    quadratic_parse_input  - parses given arguments (a, b, c) of equation
    quadratic_print_output - formats and prints equation result to console
*/
bool quadratic_parse_input  (eq_t *const eq);
void quadratic_print_output (const eq_t *const eq, FILE *stream);

void clear_buffer();

#endif
