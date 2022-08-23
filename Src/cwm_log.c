/*
 * cwm_log.c
 *
 *  Created on: 2022年3月30日
 *      Author: user
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#include <cwm_log.h>

#define log_file
#define log_max_lenght 65536

#ifdef log_file
static const char *log_folder = "log";
static const char *log_subName = ".log";
static FILE *file;
#endif

int CWM_OS_dbgOutput(const char * format) {
    // printf("%s", format);
    
#ifdef log_file
    fprintf(file, "%s", format);
#endif

    return 0;
}

int CWM_OS_dbgOutput_checkLog(const char * format)
{
    static int s_sn = 0;

    char InputBuff[log_max_lenght];

    sprintf(InputBuff, "#_#|%d|%s", s_sn, format);
    s_sn ++;
    CWM_OS_dbgOutput(InputBuff);
    return 0;
}

int CWM_OS_dbgPrintf(const char * format,...)
{
    char InputBuff[log_max_lenght];

    va_list    args;
    va_start(args, format);
    vsprintf(InputBuff, format, args);
    va_end(args);

    CWM_OS_dbgOutput(InputBuff);
    return 0;
}

void cwm_log_init(void) {
    struct stat info;

    time_t t;
    struct tm tm;

    char date_time[32];
    char filename[64];    

#ifdef log_file

    if (stat(log_folder, &info) != 0 || !(info.st_mode & S_IFDIR)) {
        mkdir(log_folder);
    }

    t = time(NULL);
    tm = *localtime(&t);

    memset(date_time, 0, sizeof(date_time));
    sprintf(date_time, "%d-%02d-%02d-%02d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

    memset(filename, 0, sizeof(filename));
    sprintf(filename, "%s\\%s%s", log_folder, date_time, log_subName);
    
    file = fopen(filename, "a");    
#endif
}

void cwm_log_close(void) {
    fclose(file);
}