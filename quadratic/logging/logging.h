#ifndef LOGGING_H
#define LOGGING_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>

#include "io.h"

#define MAX_F_ARGS 32

#define STR_TIMESTAMP_SIZE    32
#define MAX_ADDED_FORMAT_SIZE 128
#define MAX_LOG_STR_SIZE      2048

typedef enum
{
    DEBUG = 0,
    INFO  = 1,
    WARN  = 2,
    ERROR = 3,
    FATAL = 4,
} logging_level;

static const char *logging_levels[5] = {
    "DEBUG", "INFO", "WARN", "ERROR", "FATAL"
};

typedef struct
{
    FILE         *file;
    logging_level level;
} logging_t;

/*
    Init logging module
    Parameters:
        file  - file to write into
        level - logging level
*/
void init_logging (FILE *file, logging_level level);

/*
    Print to log
    Parameters:
        level - level of log output
        str   - string of log output
        args  - arguments for formatting
*/
void log_printf (logging_level level, const char *fmt, ...);

/*
    Close log file
*/
void close_log_file ();

/*
    Get timestamp
    Parameters:
        timestamp - string where to write timestamp converted to readable format
*/
static void get_timestamp (char *timestamp);

/*
    Format log string to something like [01-01-1970 12:00:00 INFO] timestamp 0 (wow)
    Parameters:
        level   - level of log output
        str     - string of log output
        res_str - string where to write formatted result
*/
static void format_log    (logging_level level, char *str, char *res_str); 

#endif
