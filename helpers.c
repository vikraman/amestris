#include "helpers.h"

#include <stdio.h>
#include <string.h>

char * concat(char * dest, const char * src1, const char * src2)
{
    strcpy(dest, src1);
    return strcat(dest, src2);
}

char * pipexec(char * command, char * output)
{
    FILE * fp = popen(command, "r");
    if (output)
        fscanf(fp, "%s", output);
    return output;
}
