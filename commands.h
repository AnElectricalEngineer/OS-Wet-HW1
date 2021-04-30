#ifndef _COMMANDS_H
#define _COMMANDS_H
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

// User added*******************************************************************
#include <new>
#include <queue>
#include <string>
#include <map>
#include <iostream>
#include <fcntl.h> //TODO for open - check if allowed
//******************************************************************************

#define MAX_LINE_SIZE 800 //TODO CHANGE BACK TO 80!
#define MAX_ARG 20
#define BUFF_SIZE 100

using namespace std;

typedef struct job
{
    string jobName;
    pid_t jobPid;
    time_t jobStartTime;
    string jobStatus;
} Job, *pJob;

//typedef enum { FALSE , TRUE } bool_; //TODO check that not needed
int BgCmd(char* lineSize, void* jobs);
int ExeCmd(map<unsigned int, pJob>* jobs, char* lineSize, char* cmdString);
void ExeExternal(char *args[MAX_ARG], char* cmdString, map<unsigned int, pJob>*
jobs);

//Helper functions
void enqueueNewCmd(queue<string>* historyPtr, char* cmdString);
void removeTermProcesses(map<unsigned int, pJob>* jobs);
#endif

