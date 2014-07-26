#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <time.h>
#include <errno.h>
#include <libgen.h>
#include <fcntl.h>
#include "fedorabk.h"

#define PRINT(C, S) printf("%s\t\t%s\n", C, S);

extern char cFrom[MAX_PATH];
extern char cTo[MAX_PATH];
int iCpCnt = 0;

int FileIsNew(time_t tTo, time_t tFrom)    {
        if(tTo < tFrom)
                return 1;
        else
                return 0;
}

void DoCopy(const char* ccFn, const char* ccFnto, int iListFile)	{
        char cTemp[256];
        strcpy(cTemp, ccFnto);
        char* cPathNsf = dirname(cTemp);
        char cCmd[256] = "";
        sprintf(cCmd, "mkdir -p \"%s\"", cPathNsf);
        system(cCmd);
        sprintf(cCmd, "cp \"%s\" \"%s\"", ccFn, cPathNsf);
        system(cCmd);
        if(iListFile)
                printf("%s==>%s\n", ccFn, ccFnto);
}

int ListDir( char* cfp, char* ctp, char* cfpcon, int iListFile ) {
        DIR* dirp;
        struct dirent* dp;
        char cPath[MAX_PATH];
        char cBkPath[MAX_PATH];
        struct stat pStat;
        struct stat pStatBk;
        int iRet = -1;
        extern int errno;
        if((dirp = opendir(cfp)) == NULL)   {
                if(iListFile)
                        printf("The dir %s is not exit!\n", cfp);
                return 1;
        }

        while ((dp = readdir( dirp )) != NULL )   {
                if ( (strcmp(dp->d_name,   ".") == 0) || (strcmp(dp->d_name, "..") == 0) )
                        continue;

                sprintf(cPath, "%s/%s", cfp, dp->d_name);

                if (stat(cPath, &pStat) == -1)      {
                        if(iListFile) {
                                perror("stat:");
                                printf("get stat %s err!\n", cPath);
                        }
                        closedir(dirp);
                        return 1;
                }

                if ( (pStat.st_mode & S_IFMT) == S_IFDIR )      {
                        if( ListDir(cPath, ctp, cfpcon, iListFile) == 1 )         {
                                closedir(dirp);
                                return 1;
                        }
                }
                else      {
                        sprintf(cBkPath, "%s%s", ctp, cPath+strlen(cfpcon));
                        iRet = stat(cBkPath, &pStatBk);
                        if(-1 == iRet && ENOENT == errno)  {
                                DoCopy(cPath, cBkPath, iListFile);
                                iCpCnt++;
                        }
                        else if(0 == iRet)    {
                                if(FileIsNew(pStatBk.st_mtime, pStat.st_mtime))   {
                                        DoCopy(cPath, cBkPath, iListFile);
                                        iCpCnt++;
                                }
                        }
                }
        }
        return 0;
}

void MakeBackup(char* cfp, char* ctp, int iListFile)   {
        char cFCon[MAX_PATH] = "";
        DIR* df;

        if(( df = opendir(cFrom)) == NULL )    {
                if(iListFile)
                        printf("Source directory  %s is not exist!\n", cFrom);
                return;
        }
        else
            closedir(df);
        if( (df = opendir(cTo)) == NULL )    {
                if(iListFile)
                        printf("Backup directory %s is not exist! Please mkdir it at first.\n", cTo);
                return;
        }
        else
            closedir(df);
        sprintf(cFCon, "%s", cFrom);
        ListDir(cFrom, cTo, cFCon, iListFile);

        if(iCpCnt)
                printf("Total backup %d files.\n", iCpCnt);
        else
                printf("No new files to backup.\n");
}
