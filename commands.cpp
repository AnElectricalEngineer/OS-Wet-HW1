//		commands.c
//******************************************************************************
#include "commands.h"

#define MAX_HISTORY_SIZE 50

using namespace std;

// Counter to keep track of highest job number
static unsigned int totalJobCount = 0;

//******************************************************************************
// function name: ExeCmd
// Description: interprets and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//******************************************************************************
int ExeCmd(map<unsigned int, pJob>* jobs, char* lineSize, char* cmdString)
{

    //TODO take care of printing whenever signals are sent
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


    //TODO check ALL built in commands for incorrect user input -> smash
    // error "line".
/******************************************************************************/
// Built in Commands PLEASE NOTE NOT ALL REQUIRED ARE IN THIS CHAIN OF IF
// COMMANDS. PLEASE ADD MORE IF STATEMENTS AS REQUIRED
/******************************************************************************/
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
                    illegal_cmd = true;
                }
                if(illegal_cmd == false)
                {
                    int changeDirSuccess = chdir(nextPath);
                    if (changeDirSuccess == -1)   // changeDir failed
                    {
                        illegal_cmd = true;
                    }
                    else if (changeDirSuccess == 0) //changeDir succeeded
                    {
                        prevPathValid = true;
                    }
                }
            }
            // If no previous path exists, do nothing
        }
        // Case 2: path is not "-"
        else
        {
            char *getCwdSuccess = getcwd(prevPath, MAX_LINE_SIZE);
            if (!getCwdSuccess)   // If getcwd() failed
            {
                illegal_cmd = true;
            }
            if(illegal_cmd == false)
            {
                int changeDirSuccess = chdir(path);
                if (changeDirSuccess == -1)   // changeDir failed
                {
                    illegal_cmd = true;
                }
                else if (changeDirSuccess == 0) //changeDir succeeded
                {
                    prevPathValid = true;
                }
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
            illegal_cmd = true;
        }
        else
        {
            printf("%s\n", pwd);
        }
    }

    /*************************************************/

    else if (!strcmp(cmd, "jobs"))
    {
        // Check for processes that have finished and remove them, and print
        // remaining processes
        auto it = jobs->begin();
        while(it != jobs->end() && !jobs->empty())
        {
            // Check for processes that have finished and remove them
            pid_t currentJobPid = it->second->jobPid;
            int didKillSucceed = kill(currentJobPid, 0); // TODO didnt check if
            // succeeded (smash
            // error)
            if(errno == ESRCH && didKillSucceed == -1)
            {
                delete it->second;
                it = jobs->erase(it);
                continue;
            }

            // Print each job that is still running
            time_t currentTime = time(NULL);

            //TODO check if colon should have space on both sides, like example
            cout << "[" << it->first << "] " << it->second->jobName << ": "
            << it->second->jobPid << " " << currentTime -
            it->second->jobStartTime << " secs " << it->second->jobStatus <<
            endl;

            ++it;
        }
    }

    /*************************************************/
    else if (!strcmp(cmd, "kill"))
    {
        int jobNum = strtol(args[2], NULL, 10);
        auto jobIt = jobs->find(jobNum);

        // If job does not exist in jobs
        if(jobIt == jobs->end())
        {
            cerr << "smash error: > kill " << jobNum << " - job does not "
                                                        "exist" << endl;
        }
        else
        {
            int sig = strtol(args[1] + 1, NULL, 10);
            pid_t jobPid = jobIt->second->jobPid;
            int didKillSucceed = kill(jobPid, sig);

            // If sending signal sending failed
            if(didKillSucceed == -1)
            {
                cerr << "smash error: > kill " << jobNum << " - cannot send "
                                                            "signal" << endl;
            }
        }
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
        //TODO add check what happens when jobs is empty
        //TODO check if should send SIGCONT signal instead of waitpid usage

        // Remove processes that have finished from jobs
        removeTermProcesses(jobs);
        if (num_arg == 0)
        {
            pid_t jobPid = (prev(jobs->end()))->second->jobPid;
            cout << (prev(jobs->end()))->second->jobName << endl;
            //pid_t jobPid = jobs->find(totalJobCount)->second->jobPid;
            //cout << jobs->find(totalJobCount)->second->jobName << endl;
            if (waitpid(jobPid, NULL, NULL) == -1)
            {
                illegal_cmd = true;
            }
        }
        else if (num_arg == 1)
        {
            int job_id = strtol(args[1],NULL,10);
            if ((job_id != 0) && (jobs->find(job_id) != jobs->end()))
            {
                pid_t jobPid = jobs->find(job_id)->second->jobPid;
                cout << jobs->find(jobPid)->second->jobName << endl;
                if (waitpid(jobPid, NULL, NULL) == -1)
                {
                    illegal_cmd = true;
                }
            }
            else
            {
                illegal_cmd = true;
            }
        }
        else
        {
            illegal_cmd = true;
        }
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
        //
        // tested - works
    else if (!strcmp(cmd, "cp"))
    {
        // Paths for two files to copy
        const char* pathName1 = args[1];
        const char* pathName2 = args[2];

        int fd1 = open(pathName1, O_RDONLY);

        // Exit if opening file 1 fails
        if(fd1 == -1)
        {
            illegal_cmd = true;
        }
        else
        {
            int fd2 = open(pathName2, O_WRONLY);

            // Check if opening file 2 fails
            if(fd2 == -1)
            {
                illegal_cmd = true;
                close(fd1);
            }
            if(illegal_cmd == false)
            {
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
                        illegal_cmd = true;
                        break;
                    }
                }while(bytesRead1 > 0);
                if(illegal_cmd == false)
                {
                    if (didWriteSucceed >= 0)
                    {
                        cout << pathName1 << " has been copied to "
                        << pathName2 << endl;
                    }
                    //  Close the files
                    int closeFd1Fail = close(fd1);
                    int closeFd2Fail = close(fd2);
                    if(closeFd1Fail || closeFd2Fail)
                    {
                        if(closeFd1Fail)
                        {
                            illegal_cmd = true;
                        }
                        if(closeFd2Fail)
                        {
                            illegal_cmd = true;
                        }
                    }
                }
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
            illegal_cmd = true;
        }
        else
        {
            int fd2 = open(pathName2, O_RDONLY);

            // Check if opening file 2 fails
            if(fd2 == -1)
            {
                illegal_cmd = true;
                close(fd1);
            }

            // Files 1 and 2 are open
            if(illegal_cmd == false)
            {
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
                        illegal_cmd = true;
                    }
                    if(closeFd2Fail)
                    {
                        illegal_cmd = true;
                    }
                }
            }
        }
    }

    /*************************************************/

    else // external command
    {
        ExeExternal(args, cmdString, jobs);
        return 0;
    }
    //TODO check forums + add smasherror
    if (illegal_cmd == true)      //TODO changed from TRUE
    {
        fprintf(stderr, "smash error: > %s\n", strerror(errno));
        return 1;
    }
    return 0;
}
//******************************************************************************
// function name: ExeExternal
// Description: executes external command
// Parameters: external command arguments, external command string
// Returns: void
//******************************************************************************
void ExeExternal(char *args[MAX_ARG], char* cmdString, map<unsigned int, pJob>*
jobs)
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
            execv(args[0], args);
            perror("Execute command failed");
            exit(1);

        default:
            // If command should be run in background

            //TODO check if background tasks should end on their own
            if (cmdString[strlen(cmdString)-1] == '&')
            {
                cmdString[strlen(cmdString)-1] = '\0';

                // Create new jobs entry
                pJob pNewJob = new Job; //  TODO free when job finishes!
                pNewJob->jobName = args[0];
                pNewJob->jobPid = pID;
                pNewJob->jobStartTime = time(NULL);
                pNewJob->jobStatus = "";

                totalJobCount++;
                unsigned int jobNumber = totalJobCount;
                jobs->insert({jobNumber, pNewJob});
            }
            // If command should be run in foreground
            else
            {
                wait(NULL);
                //waitpid(pID, NULL, NULL);
            }
    }
}

//******************************************************************************
// function name: BgCmd
// Description: if command is in background, insert the command to jobs
// Parameters: command string, pointer to jobs
// Returns: 0- BG command -1- if not
//******************************************************************************
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

//******************************************************************************
// function name: enqueueNewCmd
// Description: adds a new command to the history of commands
// Parameters: pointer to history queue, command string
// Returns: void
//******************************************************************************
void enqueueNewCmd(queue<string>* historyPtr, char* cmdString)
{
    unsigned long currentSize = historyPtr->size();
    if(currentSize == MAX_HISTORY_SIZE)
    {
        historyPtr->pop();
    }
    historyPtr->push(cmdString);
}
//******************************************************************************
// function name: removeTermProcesses
// Description: iterates over jobs and removes processes that have finished
// Parameters: pointer to jobs
// Returns: void
//******************************************************************************
void removeTermProcesses(map<unsigned int, pJob>* jobs)
{
    // Check for processes that have finished and remove them
    auto it = jobs->begin();
    while(it != jobs->end() && !jobs->empty())
    {
        // Check for processes that have finished and remove them
        pid_t currentJobPid = it->second->jobPid;
        int didKillFail = kill(currentJobPid, 0); // TODO didnt check if
        // succeeded (smash error)
        if (errno == ESRCH && didKillFail == -1)
        {
            delete it->second;
            it = jobs->erase(it);
        }
        ++it;
    }
}
