#ifndef IO_H
#define IO_H

#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#include "colors.h"

#define FILE_READ_BUFFER 1024

/*
    Load file tries to open file 
    Parameters:
        name  - name of file
    Output:
        FILE  - pointer to FILE
*/
FILE *load_file  (const char *name, const char *mode);

#endif
