#include "parameter_parser.h"

uint8_t parameters_parse (int argc, char *argv[])
{
    if (argc < 2)
    {
        return OK_CONT;
    }

    for (int i = 1; i < argc; i++)
    {
        if (strstr(argv[i], "--input_file"))
        {
            if (i == (argc - 1)) return 0;
            
            const char *name = argv[i+1];
            
            FILE* f = load_file(name, "r");

            char new_name[MAX_FILE_NAME_SIZE];
            
            // Check how long filename
            if (strlen(name) + strlen(FILE_OUT_PREFIX) + 1 > MAX_FILE_NAME_SIZE)
            {
                print_clear_formatting();
                print_colored(COLOR_FORE_WHITE, COLOR_BACK_RED, "Error! Too long output filename\n\n");
                return PERROR;
            }

            strncat(new_name, FILE_OUT_PREFIX, strlen(FILE_OUT_PREFIX));
            strncat(new_name, name,            strlen(name));

            if (parse_file(f, new_name)) return OK_CONT;

            return PERROR;
        }
        if (strstr(argv[i], "--help"))
        {
            printf("Super quadratic equation solver!\n\n");
            printf("Usage:\n");
            printf("    --help                  -> get help (wow)\n");
            printf("    --input_file <filename> -> solve equations from csv file of format a,b,c\n\n");
            printf("P.s. in program you can find super scary error, good luck!\n\n");

            return OK_EXIT;
        }
        return PERROR;
    }
}

uint8_t parse_file (FILE *file, const char *output_filename)
{
    FILE *output = load_file(output_filename, "w");

    char data[FILE_READ_BUFFER] = { '\0' };

    // Create new file and write input nums and results into it
    while (fgets(data, sizeof(data), file))
    {
        eq_t eq;
        init_eq(&eq);
        
        char *ptr = data;
        // If correct formatting (e.g.: 1,12.6,-0.2412)
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

