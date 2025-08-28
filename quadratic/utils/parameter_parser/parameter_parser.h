#ifndef PARAMETER_PARSER_H
#define PARAMETER_PARSER_H

#include <stdint.h>

#include "io.h"
#include "quadratic.h"

// Output file prefix
// Maximum filename size
#define FILE_OUT_PREFIX    "result."
#define MAX_FILE_NAME_SIZE 256

typedef enum
{
    OK_EXIT = 0,
    PERROR  = 1,
    OK_CONT = 2,
} parser_codes;

/*
    Parses all parameters given to program and executes associated code
    Parameters:
        argc - argument count
        argv - arguments
*/
uint8_t parameters_parse (int argc, char *argv[]);

/*
    Parses filee line by line and calculates something
    Parameters:
        file - file to parse
*/
uint8_t parse_file (FILE *file, const char *output_filename);

#endif
