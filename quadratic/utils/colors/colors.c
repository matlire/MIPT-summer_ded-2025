#include "colors.h"

void print_colored(AnsiColor fore, AnsiColor back, const char *text)
{
   printf("\033[%d;%d;%dm%s", 0, back, fore, text); 
}

void print_clear_formatting()
{
    printf("\033[0m");
}

