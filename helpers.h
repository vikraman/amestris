#ifndef _HELPERS_H_
#define _HELPERS_H_

/* concat src1 and src2 into dest */
char * concat(char * dest, const char * src1, const char * src2);

/* execute command and copy the stdout into output */
char * pipexec(char * command, char * output);

#endif
