#include "colors.h"

void print_colored(const AnsiColor fore, const AnsiColor back, const char *const text)
{
   printf("\033[%d;%d;%dm%s", 0, back, fore, text); 
}

void print_clear_formatting()
{
    printf("\033[0m");
}

