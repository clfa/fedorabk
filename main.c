#include <getopt.h>
#include <stdlib.h>
#include "fedorabk.h"
#include <string.h>

#define PRINT(S) printf("%s\n", S);

/*#define FOR_TEST*/

char cFrom[MAX_PATH] = "/home/example/Work";
char cTo[MAX_PATH] = "/mnt/data/Backup/fedora/Work";

const char* ProgName;
const char* const ArgShortOpt = "h:q:l:f:t:v";

const struct option ArgLongOpt[] = {
        {"help",            0,  NULL,   'h' },
        {"quiet",           1,  NULL,   'q' },
        {"list",            1,  NULL,   'l' },
        {"from",            1,  NULL,   'f' },
        {"to",              1,  NULL,   't' },
        {"version",         1,  NULL,   'v' },
        {NULL,              0,  NULL,   0   }
};

void PrintHelp(FILE* stream, int exit_code) {
        fprintf(stream, "Usage: %s [options] [source-path dst-path]\n", ProgName);
        fprintf(stream,
                "   -h --help       Display this usage information.\n"
                "   -q --quiet      Quiet execute program.\n"
                "   -l --list       List backup or copy files.\n"
                "   -f --from       Source Path.\n"
                "   -t --to         Backup Path.\n"
                "   -v --version    Program version information.\n"
                );
        exit(exit_code);
}

int main(int argc, char* argv[])
{
        if(argc == 1) {
#ifndef FOR_TEST
                MakeBackup(cFrom, cTo, 1);
#endif
        }
        else {
                int iList = 1;
                int iNexOpt;
                ProgName = argv[0];
                do {
                        iNexOpt = getopt_long(argc, argv, ArgShortOpt, ArgLongOpt, NULL);
                        switch(iNexOpt) {
                        case 'h':
                                PrintHelp(stdout, 0);
                        case 'q':
                                iList = 0;
                                break;
                        case 'l':
                                iList = 1;
                                break;
                        case 'f':
                                strcpy(cFrom, optarg);
                                break;
                        case 't':
                                strcpy(cTo, optarg);
                                break;
                        case 'v':
                                printf("fedorabk v1.0\n");
                                break;
                        default:
                                break;
                        }
                }
                while(iNexOpt != -1);
#ifndef FOR_TEST
                MakeBackup(cFrom, cTo, iList);
#endif
        }
        return 0;
}














