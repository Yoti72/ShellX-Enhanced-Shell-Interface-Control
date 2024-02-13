#ifndef BACKGROUNDPROCESSING_H
#define BACKGROUNDPROCESSING_H

#include <sys/types.h>

typedef struct
{
    int jobNumber;
    pid_t pid;
    char *commandLine;
    int status; // 0 for running, 1 for done
} BackgroundProcess;

void initializeBackgroundProcessing();
void addBackgroundJob(pid_t pid, char *commandLine);
void updateBackgroundJobs();
void listBackgroundJobs();

#endif // BACKGROUNDPROCESSING_H
