// signals.c
// contains signal handler functions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlz
   Synopsis: handle the Control-Z */
#include "signals.h"

void handler_cntlz(int sig_num)
{
    //printf("got here\n");
    pid_t myPid = getpid();
    kill(myPid, SIGTSTP);
}

