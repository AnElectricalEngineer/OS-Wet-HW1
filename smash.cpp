/*	smash.c
main file. This file contains the main function of smash
*******************************************************************/
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "commands.h"
#include "signals.h"
#include <map>
#define MAX_LINE_SIZE 800 //TODO change back to 80
#define MAXARGS 20

using namespace std;

char* L_Fg_Cmd;

auto jobs = new map<unsigned int, pJob>;    // Data structure to hold jobs

//void* jobs = NULL; //This represents the list of jobs. Please change to a
// preferred type (e.g array of char*)
char lineSize[MAX_LINE_SIZE];
//**************************************************************************************
// function name: main
// Description: main function of smash. get command from user and calls command functions
//**************************************************************************************
int main(int argc, char *argv[])
{
    char cmdString[MAX_LINE_SIZE];


    //signal declarations
    //NOTE: the signal handlers and the function/s that sets the handler should be found in signals.c
    /* add your code here */

    /************************************/
    //NOTE: the signal handlers and the function/s that sets the handler should be found in signals.c
    //set your signal handlers here
    /* add your code here */

    /************************************/

    /************************************/
    // Init globals



    L_Fg_Cmd =(char*)malloc(sizeof(char)*(MAX_LINE_SIZE+1));
    if (L_Fg_Cmd == NULL)
        exit (-1);
    L_Fg_Cmd[0] = '\0';

    while (1)
    {
        printf("smash > ");
        fgets(lineSize, MAX_LINE_SIZE, stdin);
        strcpy(cmdString, lineSize);
        cmdString[strlen(lineSize)-1]='\0';
        // perform a complicated Command
        // TODO Exe comp is not relevant for us! Delete at end!
        if(!ExeComp(lineSize)) continue;
        // background command
        // TODO probably delete BgCmd also
        if(!BgCmd(lineSize, jobs)) continue;
        // built in commands
        ExeCmd(jobs, lineSize, cmdString);

        /* initialize for next line read*/
        lineSize[0]='\0';
        cmdString[0]='\0';
    }
    return 0;
}

