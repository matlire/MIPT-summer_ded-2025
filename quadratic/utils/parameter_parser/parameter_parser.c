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
