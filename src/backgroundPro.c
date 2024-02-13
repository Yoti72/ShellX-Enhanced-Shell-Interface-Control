#include "backgroundPro.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_BACKGROUND_JOBS 10
BackgroundProcess bgJobs[MAX_BACKGROUND_JOBS];
int jobCounter = 1;

void initializeBackgroundProcessing()
{
    for (int i = 0; i < MAX_BACKGROUND_JOBS; i++)
    {
        bgJobs[i].pid = 0; // 0 PID indicates unused slot
        bgJobs[i].jobNumber = -1;
        bgJobs[i].commandLine = NULL;
        bgJobs[i].status = 0;
    }
}

void addBackgroundJob(pid_t pid, char *commandLine)
{
    for (int i = 0; i < MAX_BACKGROUND_JOBS; i++)
    {
        if (bgJobs[i].pid == 0)
        { // Find an empty slot
            bgJobs[i].jobNumber = jobCounter++;
            bgJobs[i].pid = pid;
            bgJobs[i].commandLine = strdup(commandLine);
            bgJobs[i].status = 0;                          // Mark as running
            printf("[%d] %d\n", bgJobs[i].jobNumber, pid); // Print job info
            return;
        }
    }
    fprintf(stderr, "Reached max background jobs limit.\n");
}

void updateBackgroundJobs()
{
    for (int i = 0; i < MAX_BACKGROUND_JOBS; i++)
    {
        if (bgJobs[i].pid != 0 && bgJobs[i].status == 0)
        { // Job is running
            int status;
            pid_t result = waitpid(bgJobs[i].pid, &status, WNOHANG);
            if (result == bgJobs[i].pid)
            {
                bgJobs[i].status = 1;                                                  // Mark as done
                printf("[%d]+ done %s\n", bgJobs[i].jobNumber, bgJobs[i].commandLine); // Print completion message
                free(bgJobs[i].commandLine);                                           // Free allocated command line
                bgJobs[i].commandLine = NULL;
                bgJobs[i].pid = 0; // Reset slot
            }
        }
    }
}

void listBackgroundJobs()
{
    for (int i = 0; i < MAX_BACKGROUND_JOBS; i++)
    {
        if (bgJobs[i].pid != 0)
        {
            printf("[%d] + %d %s %s\n", bgJobs[i].jobNumber, bgJobs[i].pid, bgJobs[i].status ? "done" : "running", bgJobs[i].commandLine);
        }
    }
}
