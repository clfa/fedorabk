#ifndef     _FEDORABK_H
#define     _FEDORABK_H     1

#include <stdio.h>
#define MAX_PATH 1024

void MakeBackup(char* cfp, char* ctp, int iListFile);

void PrintHelp(FILE* stream, int exit_code);

#endif  /*fedorabk.h*/
