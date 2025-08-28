#include "io.h"

FILE *load_file (const char *name, const char *mode)
{
    FILE* f = fopen(name, mode);
    if (!f)
    {
        print_clear_formatting();
        print_colored(COLOR_FORE_WHITE, COLOR_BACK_RED, "");
        perror("Error openning file!");
    }

    return f;
}

