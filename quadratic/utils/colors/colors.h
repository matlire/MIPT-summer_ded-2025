#ifndef COLORS_H
#define COLORS_H

#include <stdio.h>

typedef enum
{
    COLOR_RESET        = 0,
    TEXT_BOLD          = 1,
    TEXT_UNDERLINE     = 4,
    TEXT_INVERSE       = 7,
    TEXT_BOLD_OFF      = 21,
    TEXT_UNDERLINE_OFF = 24,
    TEXT_INVERSE_OFF   = 27,

    COLOR_FORE_BLACK   = 30,
    COLOR_FORE_RED     = 31,
    COLOR_FORE_GREEN   = 32,
    COLOR_FORE_YELLOW  = 33,
    COLOR_FORE_BLUE    = 34,
    COLOR_FORE_MAGENTA = 35,
    COLOR_FORE_CYAN    = 36,
    COLOR_FORE_WHITE   = 37,

    COLOR_BACK_BLACK   = 40,
    COLOR_BACK_RED     = 41,
    COLOR_BACK_GREEN   = 42,
    COLOR_BACK_YELLOW  = 43,
    COLOR_BACK_BLUE    = 44,
    COLOR_BACK_MAGENTA = 45,
    COLOR_BACK_CYAN    = 46,
    COLOR_BACK_WHITE   = 47,
} AnsiColor;

void print_colored          (const AnsiColor fore, const AnsiColor back, const char *const text);
void print_clear_formatting ();

#endif
