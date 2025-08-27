#include "logging.h"

static logging_t logging; 

void init_logging (FILE *file, logging_level level)
{
    logging.file  = file;
    logging.level = level;
}

void log_printf (logging_level level, const char *fmt, ...)
{
    char res_str[MAX_LOG_STR_SIZE] = {  };
        
    va_list args = {  };
    
    va_start(args, fmt);
    vsprintf(res_str, fmt, args);
    va_end(args);

    format_log(level, fmt, res_str);
    fprintf(logging.file, "%s", res_str);
    
    fflush(logging.file);
}

static void get_timestamp (char *timestamp)
{
    time_t    current_time;
    struct tm *local_time_info;
    
    current_time = time(NULL);
    local_time_info = localtime(&current_time);

    strftime(timestamp, STR_TIMESTAMP_SIZE, "%d-%m-%Y %H:%M:%S", local_time_info);
}

static void format_log (logging_level level, char *str, char *res_str)
{
    char timestamp[STR_TIMESTAMP_SIZE];
    get_timestamp(timestamp);
    sprintf(res_str, "[%s %s] %s\n", timestamp, logging_levels[level], str);
}

void close_log_file ()
{
    fclose(logging.file);
}
