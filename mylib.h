#ifndef MYLIB_H
#define MYLIB_H

/* DO NOT ADD CODE: This file is used in evaluation
 * Changing function signatures will result in a 25% deduction.
 * adding comments is ok.
 */

void clrbuf (int);
long decin (void);
void decout (long number);
void hexout (unsigned long);
void newline (void);
long writeline (const char *, FILE *);
int getopt(int argc, char * const argv[], const char *optstring);

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifdef NULL
#undef NULL
#endif
#define NULL 0

#endif

