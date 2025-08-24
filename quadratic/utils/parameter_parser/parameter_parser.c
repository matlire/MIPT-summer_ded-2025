#include "parameter_parser.h"

uint8_t parameters_parse (int argc, char *argv[])
{
    if (argc < 2)
    {
        return 1;
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
                return 0;
            }

            strncat(new_name, FILE_OUT_PREFIX, strlen(FILE_OUT_PREFIX));
            strncat(new_name, name,            strlen(name));

            if (parse_file(f, new_name)) return 1;

            return 0;
        }
    }
}
