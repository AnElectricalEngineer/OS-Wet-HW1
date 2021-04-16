//		commands.c
//********************************************
#include "commands.h"
//********************************************
// function name: ExeCmd
// Description: interprets and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************
int ExeCmd(void* jobs, char* lineSize, char* cmdString)
{
    char* cmd;
    char* args[MAX_ARG];
    char pwd[MAX_LINE_SIZE];
    char* delimiters = " \t\n";
    int i = 0, num_arg = 0;
    bool illegal_cmd = false; // illegal command   //TODO changed from FALSE
    cmd = strtok(lineSize, delimiters);
    if (cmd == NULL)
        return 0;
    args[0] = cmd;
    for (i=1; i<MAX_ARG; i++)
    {
        args[i] = strtok(NULL, delimiters);
        if (args[i] != NULL)
            num_arg++;
    }

    static char prevPath[MAX_LINE_SIZE];    // Hold prev working directory
    static bool prevPathValid = false;
    
/*************************************************/
// Built in Commands PLEASE NOTE NOT ALL REQUIRED
// ARE IN THIS CHAIN OF IF COMMANDS. PLEASE ADD
// MORE IF STATEMENTS AS REQUIRED
/*************************************************/
    if (!strcmp(cmd, "cd") )    // TODO take care of whitespaces (ignore)
    {
        char *path = args[1];

        // Case 1: path equals "-"
        if (!strcmp(path, "-"))
        {
            if(prevPathValid)
            {

            }
        }
        // Case 2: path is not "-"
        else
        {
            int chdirSuccess = chdir(args[1]);
            switch(chdirSuccess)
            {
                case -1: perror("cd failed");
                case 0:
            }
        }
    }

        /*************************************************/
    else if (!strcmp(cmd, "pwd"))
    {
        // TODO check if works because limit on line size is very small
        // TODO check that limit is correct

        // getcwd() returns NULL if buffer size is too small
        if(!getcwd(pwd, MAX_LINE_SIZE))
        {
            perror("Path limit exceeded");
        }
        else
        {
            printf("%s", pwd);
        }
    }

        /*************************************************/
    else if (!strcmp(cmd, "mkdir"))
    {

    }
        /*************************************************/

    else if (!strcmp(cmd, "jobs"))
    {

    }
        /*************************************************/
    else if (!strcmp(cmd, "showpid"))
    {

    }
        /*************************************************/
    else if (!strcmp(cmd, "fg"))
    {

    }
        /*************************************************/
    else if (!strcmp(cmd, "bg"))
    {

    }
        /*************************************************/
    else if (!strcmp(cmd, "quit"))
    {

    }
        /*************************************************/
    else // external command
    {
        ExeExternal(args, cmdString);
        return 0;
    }
    if (illegal_cmd == true)      //TODO changed from TRUE
    {
        printf("smash error: > \"%s\"\n", cmdString);
        return 1;
    }
    return 0;
}
//**************************************************************************************
// function name: ExeExternal
// Description: executes external command
// Parameters: external command arguments, external command string
// Returns: void
//**************************************************************************************
void ExeExternal(char *args[MAX_ARG], char* cmdString)
{
    // TODO Need to test if external commands work. Tested failures - work.
    int pID;
    switch(pID = fork())
    {
        case -1:
            // Add your code here (error)

            perror("Fork failed");

        case 0 :
            // Child Process
            setpgrp();

            // Add your code here (execute an external command)
            execv(cmdString, args);
            perror("Execute command failed");

        default:
            // Add your code here
            wait(NULL);
            // TODO this is simple example - add background support later
            /*
            your code
            */
    }
}
//**************************************************************************************
// function name: ExeComp
// Description: executes complicated command
// Parameters: command string
// Returns: 0- if complicated -1- if not
//**************************************************************************************
int ExeComp(char* lineSize)
{
    char ExtCmd[MAX_LINE_SIZE+2];
    char *args[MAX_ARG];
    if ((strstr(lineSize, "|")) || (strstr(lineSize, "<")) || (strstr(lineSize, ">")) || (strstr(lineSize, "*")) || (strstr(lineSize, "?")) || (strstr(lineSize, ">>")) || (strstr(lineSize, "|&")))
    {
        // Add your code here (execute a complicated command)

        /*
        your code
        */
    }
    return -1;
}
//**************************************************************************************
// function name: BgCmd
// Description: if command is in background, insert the command to jobs
// Parameters: command string, pointer to jobs
// Returns: 0- BG command -1- if not
//**************************************************************************************
int BgCmd(char* lineSize, void* jobs)
{

    char* Command;
    char* delimiters = " \t\n";
    char *args[MAX_ARG];
    if (lineSize[strlen(lineSize)-2] == '&')
    {
        lineSize[strlen(lineSize)-2] = '\0';
        // Add your code here (execute a in the background)

        /*
        your code
        */

    }
    return -1;
}

