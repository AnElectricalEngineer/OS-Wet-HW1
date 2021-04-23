//		commands.c
//********************************************
#include "commands.h"

#define MAX_HISTORY_SIZE 50

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

    static queue<string> historyQueue;   // Holds history of commands

    // Enqueue command if command is not history
    //TODO check what happens if several legal built in commands are entered,
    // eg pwd, and then gibberish, and then history.
    if(strcmp(cmd, "history") != 0)
    {
        enqueueNewCmd(&historyQueue, cmdString);
    }

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
                char* getCwdSuccess = getcwd(prevPath, MAX_LINE_SIZE);
                if(!getCwdSuccess)   // If getcwd() failed
                {
                    perror("cwd failed");
                    return 1;
                }
                int changeDirSuccess = chdir(nextPath);
                if(changeDirSuccess == -1)   // changeDir failed
                {
                    perror("cd failed");
                    return 1;
                }
                else if(changeDirSuccess == 0) //changeDir succeeded
                {
                    prevPathValid = true;
                }
            }
            // If no previous path exists, do nothing
        }
        // Case 2: path is not "-"
        else
        {
            char* getCwdSuccess = getcwd(prevPath, MAX_LINE_SIZE);
            if(!getCwdSuccess)   // If getcwd() failed
            {
                perror("cwd failed");
                return 1;
            }
            int changeDirSuccess = chdir(path);
            if(changeDirSuccess == -1)   // changeDir failed
            {
                perror("cd failed");
                return 1;
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
        // TODO check that limit is correct

        // getcwd() returns NULL if buffer size is too small
        // TODO on forum, it's written to print smash error if pwd fails..
        if(!getcwd(pwd, MAX_LINE_SIZE))
        {
            perror("Path limit exceeded");
            return 1;
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
        queue<string> tempQueue;

        // Print history of commands
        while(!historyQueue.empty())
        {
            auto currentCommand = historyQueue.front();
            cout << currentCommand << endl;
            historyQueue.pop();
            tempQueue.push(currentCommand);
        }

        while(!tempQueue.empty())
        {
            historyQueue.push(tempQueue.front());
            tempQueue.pop();
        }
    }
        /*************************************************/

        //TODO ask on forum what should happen if file being written to is
        // longer than file writing from. Should we delete all contents of
        // file being written to? Currently, content at end is saved.

        //tested - works
    else if (!strcmp(cmd, "cp"))
    {
        // Paths for two files to copy
        const char* pathName1 = args[1];
        const char* pathName2 = args[2];

        int fd1 = open(pathName1, O_RDONLY);

        // Exit if opening file 1 fails
        if(fd1 == -1)
        {
            perror("Error opening file 1");
            return 1;
        }
        else
        {
            int fd2 = open(pathName2, O_WRONLY);

            // Check if opening file 2 fails
            if(fd2 == -1)
            {
                perror("Error opening file 2");
                int isCloseSuccessful = close(fd1);
                if(isCloseSuccessful == -1)  // If close is not successful
                {
                    perror("Error closing file 1");
                }
                return 1;
            }
            // Files 1 and 2 are open
            char fileBuff1[BUFF_SIZE];
            ssize_t bytesRead1;
            ssize_t didWriteSucceed;
            do
            {
                bytesRead1 = read(fd1, fileBuff1, BUFF_SIZE);
                didWriteSucceed = write(fd2, fileBuff1, bytesRead1);
                if (didWriteSucceed == -1)
                {
                    perror("Write failed");
                    break;
                }
            }while(bytesRead1 > 0);
            if (didWriteSucceed >= 0)
            {
                cout << pathName1 << " has been copied to " << pathName2 <<
                endl;
            }
            //  Close the files
            int closeFd1Fail = close(fd1);
            int closeFd2Fail = close(fd2);
            if(closeFd1Fail || closeFd2Fail)
            {
                if(closeFd1Fail)
                {
                    perror("Error closing file 1");
                }
                if(closeFd2Fail)
                {
                    perror("Error closing file 2");
                }
                return 1;
            }
        }
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
            return 1;
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
                return 1;
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
            int closeFd1Fail = close(fd1);
            int closeFd2Fail = close(fd2);
            if(closeFd1Fail || closeFd2Fail)
            {
                if(closeFd1Fail)
                {
                    perror("Error closing file 1");
                }
                if(closeFd2Fail)
                {
                    perror("Error closing file 2");
                }
                return 1;
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
    // TODO Need to add return 1 to perrors here
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
            exit(1);

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

//**************************************************************************************
// function name: enqueueNewCmd
// Description: adds a new command to the history of commands
// Parameters: pointer to history queue, command string
// Returns: void
//**************************************************************************************
void enqueueNewCmd(queue<string>* historyPtr, char* cmdString)
{
    unsigned long currentSize = historyPtr->size();
    if(currentSize == MAX_HISTORY_SIZE)
    {
        historyPtr->pop();
    }
    historyPtr->push(cmdString);
}