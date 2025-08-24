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
            if (parse_file(f, "result.csv")) return 1;

            return 0;
        }
    }
}
