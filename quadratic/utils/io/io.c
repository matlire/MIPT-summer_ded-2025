#include "io.h"

FILE* load_file (const char *name, const char *mode)
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

uint8_t parse_file (FILE *file, const char *output_filename)
{
    FILE *output = load_file(output_filename, "w");

    char data[FILE_READ_BUFFER] = { '\0' };

    while (fgets(data, sizeof(data), file))
    {
        eq_t eq;
        init_eq(&eq);
        
        char *ptr = data;
        if (sscanf(ptr, "%lf,%lf,%lf", &eq.a, &eq.b, &eq.c) == 3)
        {
            uint8_t root_number = quadratic_calc_roots(&eq);
            data[strcspn(data, "\n")] = '\0';
            fprintf(output, "%s,", data);
            quadratic_print_output(&eq, output);
        } else {
            print_clear_formatting();
            print_colored(COLOR_FORE_WHITE, COLOR_BACK_RED, "");
            perror("Incorrect file formatting, it must be csv!");
            fclose(output);
            fclose(file);
            return 0;
        }
    }

    print_clear_formatting();
    print_colored(COLOR_FORE_WHITE, COLOR_BACK_CYAN, "File parsed successfully!\n\n");
    
    fclose(output);
    fclose(file);
    return 1;
}

