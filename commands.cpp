//		commands.c
//********************************************
#include "commands.h"

using namespace std;
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

    //static queue<char*> historyQueue; //TODO should history include illegal
    // commands

    
/*************************************************/
// Built in Commands PLEASE NOTE NOT ALL REQUIRED
// ARE IN THIS CHAIN OF IF COMMANDS. PLEASE ADD
// MORE IF STATEMENTS AS REQUIRED
/*************************************************/
    if (!strcmp(cmd, "cd") )
        // TODO take care of whitespaces (ignore)
        // TODO what to do in case of error (smash error?)
    {
        static char prevPath[MAX_LINE_SIZE];    // Hold prev working directory
        static bool prevPathValid = false;
        char *path = args[1];

        // Case 1: path equals "-"
        if (!strcmp(path, "-"))
        {
            if(prevPathValid)
            {
                char nextPath[MAX_LINE_SIZE];
                strcpy(nextPath, prevPath);
                getcwd(prevPath, MAX_LINE_SIZE);    //TODO check if need to
                // take care of syscall error
                int changeDirSuccess = chdir(nextPath);
                if(changeDirSuccess == -1)   // changeDir failed
                {
                    perror("cd failed");
                }
                else if(changeDirSuccess == 0) //changeDir succeeded
                {
                    prevPathValid = true;
                }
            }
            else
            {
                // TODO ask what to do if user entered "-" but no prev path exists
            }
        }
        // Case 2: path is not "-"
        else
        {
            getcwd(prevPath, MAX_LINE_SIZE);    //TODO maybe need to check if
            // succeeded, like pwd
            int changeDirSuccess = chdir(path);
            if(changeDirSuccess == -1)   // changeDir failed
            {
                //perror("cd failed");
            }
            else if(changeDirSuccess == 0) //changeDir succeeded
            {
                prevPathValid = true;
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
            printf("%s\n", pwd);
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
        // getpid is always successful, no need to check
        pid_t myPID = getpid();
        cout << "smash pid is " << myPID << endl;
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
    else if (!strcmp(cmd, "history"))
    {

    }
        /*************************************************/
    else if (!strcmp(cmd, "diff"))
    {
        //TODO We checked this function! To check again, put folder of test
        // files in c drive (only works with short path)

        //TODO Ask what to do if try to open same file twice

        // Paths for two files to diff
        const char* pathName1 = args[1];
        const char* pathName2 = args[2];

        int fd1 = open(pathName1, O_RDONLY);

        // Exit if opening file 1 fails
        if(fd1 == -1)
        {
            perror("Error opening file 1");
        }
        else
        {
            int fd2 = open(pathName2, O_RDONLY);

            // Check if opening file 2 fails
            if(fd2 == -1)
            {
                perror("Error opening file 2");
                int isCloseSuccessful = close(fd1);
                if(isCloseSuccessful == -1)  // If close is not successful
                {
                    perror("Error closing file 1");
                }
            }

            // Files 1 and 2 are open
            char fileBuff1[BUFF_SIZE], fileBuff2[BUFF_SIZE];
            ssize_t bytesRead1, bytesRead2;
            bool areFilesSame = true;
            do
            {
                bytesRead1 = read(fd1, fileBuff1, BUFF_SIZE - 1);
                fileBuff1[bytesRead1] = '\0';
                bytesRead2 = read(fd2, fileBuff2, BUFF_SIZE - 1);
                fileBuff2[bytesRead2] = '\0';

                // files 1 and 2 contain some different characters
                if(strcmp(fileBuff1, fileBuff2) != 0)
                {
                    areFilesSame = false;
                    break;
                }
            }while(bytesRead1 > 0 && bytesRead2 > 0);

            if(areFilesSame == true)
            {
                cout << "0" << endl;
            }
            else
            {
                cout << "1" << endl;
            }

            //  Close the files
            if(close(fd1))
            {
                perror("Error closing file 1");
            }
            if(close(fd2))
            {
                perror("Error closing file 2");
            }
        }
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

