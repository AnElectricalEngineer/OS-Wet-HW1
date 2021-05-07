//		commands.c
//******************************************************************************
#include "commands.h"

#define MAX_HISTORY_SIZE 50

using namespace std;

// Counter to keep track of highest job number
unsigned int totalJobCount = 0;
bool isFgProcess = false;
pid_t lastFgPid;
string lastFgJobName;


//******************************************************************************
// function name: ExeCmd
// Description: interprets and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//******************************************************************************
int ExeCmd(map<unsigned int, pJob>* jobs, char* lineSize, char* cmdString)
{

    // TODO ask how does signal handler work, who gets CTRL-Z/C ...?
    //TODO (not question) take care of printing whenever signals are sent
    char* cmd;
    char* args[MAX_ARG];
    char pwd[MAX_LINE_SIZE];
    char* delimiters = " \t\n";
    int i = 0, num_arg = 0;
    bool illegal_cmd = false; // illegal command
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

    // Enqueue command if command is not history. Holds all prev commands
    // (except 'history', including gibberish.
    if(strcmp(cmd, "history") != 0)
    {
        enqueueNewCmd(&historyQueue, cmdString);
    }

/******************************************************************************/
// Built in Commands PLEASE NOTE NOT ALL REQUIRED ARE IN THIS CHAIN OF IF
// COMMANDS. PLEASE ADD MORE IF STATEMENTS AS REQUIRED
/******************************************************************************/
    if (!strcmp(cmd, "cd") )
    {
        //  Check that correct number of arguments were received (1)
        if(num_arg != 1)
        {
            cerr << "smash error: > " << "\"" << cmdString << "\"" << endl;
            return 1;
        }

        //  Here, number of arguments is correct
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
        //  Check that number of arguments is correct
        if(num_arg != 0)
        {
            cerr << "smash error: > " << "\"" << cmdString << "\"" << endl;
            return 1;
        }
        // getcwd() returns NULL if buffer size is too small
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

    else if (!strcmp(cmd, "jobs"))  //TODO test with actual bg process
    {
        //  Check that number of parameters is correct (0)
        if(num_arg != 0)
        {
            cerr << "smash error: > " << "\"" << cmdString << "\"" << endl;
            return 1;
        }

        // Update jobs
        illegal_cmd = updateJobs(jobs);

        // If there were no sys call errors in updateJobs
        if(!illegal_cmd)
        {
            // Print jobs
            auto it = jobs->begin();
            while(it != jobs->end() && !jobs->empty())
            {
                time_t currentTime = time(NULL);

                cout << "[" << it->first << "] " << it->second->jobName << " : "
                     << it->second->jobPid << " " << currentTime -
                     it->second->jobStartTime << " secs " <<
                     it->second->jobStatus << endl;

                ++it;
            }
        }
    }

    /*************************************************/
    //TODO (not question) test this, including errors
    else if (!strcmp(cmd, "kill"))
    {
        //Check if number of parameters is correct (2)
        if(num_arg != 2)
        {
            cerr << "smash error: > " << "\"" << cmdString << "\"" << endl;
            return 1;
        }

        //Check if first argument (signum) begins with "-"
        if(args[1][0] != '-')
        {
            cerr << "smash error: > " << "\"" << cmdString << "\"" << endl;
            return 1;
        }

        // Convert string arguments to numbers

        //TODO ask if we can assume 0 is not a valid signal because strtol
        // returns 0 if no conversion can be printed.

        //TODO (not question) maybe use std::stoi?
        int sig = strtol(args[1] + 1, NULL, 10); // signum
        int jobNum = strtol(args[2], NULL, 10); // jobID

        // strtol returns 0 if it fails to convert string - that is: if not
        // value passed to it for conversion is not a number. If either jobNum
        // or sig are not numbers or if sig is not a valid signal number, error.

        //TODO (not question) change to if(jobNum == 0 || sig == 0) because
        // Lior said don't need to check that sig is valid signal number,
        // only that it is a number.
        if(jobNum == 0 || sig <= 0 || sig > 31)
        {
            cerr << "smash error: > " << "\"" << cmdString << "\"" << endl;
            return 1;
        }

        auto jobIt = jobs->find(jobNum);

        // If job does not exist in jobs
        if(jobIt == jobs->end())
        {
            cerr << "smash error: > kill " << jobNum << " - job does not "
                                                        "exist" << endl;
            return 1;
        }
        else
        {
            pid_t jobPid = jobIt->second->jobPid;
            int didKillSucceed = kill(jobPid, sig);

            // If sending signal sending failed
            if(didKillSucceed == -1)
            {
                cerr << "smash error: > kill " << jobNum << " - cannot send "
                                                            "signal" << endl;
                return 1;
            }
        }
    }

    /*************************************************/

    else if (!strcmp(cmd, "showpid"))
    {
        //  Check that number of parameters is correct (0)
        if(num_arg != 0)
        {
            cerr << "smash error: > " << "\"" << cmdString << "\"" << endl;
            return 1;
        }

        // getpid is always successful, no need to check
        pid_t myPID = getpid();
        cout << "smash pid is " << myPID << endl;
    }

    /*************************************************/

    else if (!strcmp(cmd, "fg"))
    {
        //  Check that number of parameters is correct (0 or 1), and that jobs
        //  contains at least one job
        if(num_arg > 1 || jobs->empty())
        {
            cerr << "smash error: > " << "\"" << cmdString << "\"" << endl;
            return 1;
        }
        //TODO add check that when fg is called and jobs is empty, error is
        // printed

        // update jobs list
        updateJobs(jobs);

        if (num_arg == 0)
        {
            pid_t jobPid = (prev(jobs->end()))->second->jobPid;
            string jobName = (prev(jobs->end()))->second->jobName;
            cout << jobName << endl;

            isFgProcess = true;
            lastFgPid = jobPid;
            lastFgJobName = jobName;
            if (waitpid(jobPid, NULL, NULL) == -1)
            {
                illegal_cmd = true; //TODO continue from here, check if wait
                // is correct...
            }
            isFgProcess = false;
        }

        //TODO add here isFgProcess updates etc...from line 278
        else if (num_arg == 1)
        {
            int job_id;
            try
            {
                job_id = std::stoi(args[1]);
            }
            catch(std::invalid_argument&)
            {
                cerr << "smash error: > " << "\"" << cmdString << "\"" << endl;
                return 1;
            }

            if ((jobs->find(job_id) != jobs->end()))
            {
                pid_t jobPid = jobs->find(job_id)->second->jobPid;
                cout << jobs->find(jobPid)->second->jobName << endl;
                if (waitpid(jobPid, NULL, NULL) == -1)
                {
                    illegal_cmd = true;
                }
            }

            // job_id is a number, but no such job exists in jobs
            else
            {
                cerr << "smash error: > " << "\"" << cmdString << "\"" << endl;
                return 1;
            }
        }
    }

    /*************************************************/

    else if (!strcmp(cmd, "bg"))
    {

    }

    /*************************************************/

    //TODO work on this
    else if (!strcmp(cmd, "quit"))
    {
        //  Check that number of parameters is correct (0 or 1)
        if(num_arg > 1)
        {
            cerr << "smash error: > " << "\"" << cmdString << "\"" << endl;
            return 1;
        }

        //TODO ask how should quit work? send SIGKILL to each bg process? or exit?
        if (num_arg == 0)
        {
            exit(0);
        }
        else if (num_arg == 1)
        {
            if(!strcmp(args[1], "kill"))    // If first argument is "kill"
            {
                //TODO (not question) do this
            }
        }
        else //error occurred
        {
            cerr << "smash error: > " << "\"" << cmdString << "\"" << endl;
            return 1;
        }
    }

    /*************************************************/

    else if (!strcmp(cmd, "history"))
    {
        //  Check that number of parameters is correct (0)
        if(num_arg != 0)
        {
            cerr << "smash error: > " << "\"" << cmdString << "\"" << endl;
            return 1;
        }

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

        //TODO ask what should happen if file being written to is
        // longer than file writing from. Should we delete all contents of
        // file being written to? Currently, content at end is saved.

    else if (!strcmp(cmd, "cp"))
    {
        //  Check that number of parameters is correct (2)
        if(num_arg != 2)
        {
            cerr << "smash error: > " << "\"" << cmdString << "\"" << endl;
            return 1;
        }

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
        //  Check that number of parameters is correct (2)
        if(num_arg != 2)
        {
            cerr << "smash error: > " << "\"" << cmdString << "\"" << endl;
            return 1;
        }

        // We checked this function! To check again, put folder of test
        // files in c drive (only works with short path)

        //TODO (not question) Check if opening same file twice works

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

    if (illegal_cmd == true)
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
    // TODO (not question) Need to test if external commands work. Tested
    //  failures - work.
    // TODO  (not question) Need to add return 1 to perrors here
    int pID;
    switch(pID = fork())
    {
        case -1:
            // Add your code here (error)
            fprintf(stderr, "smash error: > %s\n", strerror(errno));

        case 0 :
            // Child Process
            setpgrp();

            // Add your code here (execute an external command)
            execv(args[0], args);

            //If execv returns - error
            fprintf(stderr, "smash error: > %s\n", strerror(errno));
            exit(1); // Lior said correct way to exit process - just dont do
            // exit in main

        default:
            // If command should be run in background

            //TODO CURRENTLY there is an error here. If you run an illegal
            // external command (eg 'ls &' instead of '/bin/ls &'), it is
            // still added to jobs.

            if (cmdString[strlen(cmdString)-1] == '&')
            {
                cmdString[strlen(cmdString)-1] = '\0';

                // Create new jobs entry
                pJob pNewJob = new Job;
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
                //wait(NULL);
                isFgProcess = true;
                lastFgPid = pID;
                lastFgJobName = args[0];
                // TODO check this
                // waitPid with WUNTRACED option will return if child (fg
                // process) is terminated OR stopped
                waitpid(pID, NULL, WUNTRACED);
                isFgProcess = false;
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
// Description: iterates over jobs and updates process statuses
// Parameters: pointer to jobs
// Returns: false if succeeded, true if sys call fail
//******************************************************************************
bool updateJobs(map<unsigned int, pJob>* jobs)
{
    // Check for processes that have finished and remove them
    auto it = jobs->begin();
    while(it != jobs->end() && !jobs->empty())
    {
        pid_t currentJobPid = it->second->jobPid;
        int processStatus;
        pid_t waitPid = waitpid(currentJobPid, &processStatus,
                                WNOHANG | WUNTRACED | WCONTINUED);

        //Sys call error
        if(waitPid == -1)
        {
            fprintf(stderr, "smash error: > %s\n", strerror(errno));
            return true;
        }

        // if job HAS changed status
        else if(waitPid == currentJobPid)
        {
            // If process was terminated normally or by signal
            if(WIFEXITED(processStatus) || WIFSIGNALED(processStatus))
            {
                delete it->second;
                it = jobs->erase(it);   // Automatically increments iterator
            }

            // If process was stopped
            else if(WIFSTOPPED(processStatus))
            {
                it->second->jobStatus = "(Stopped)";
                ++it;
            }

            // TODO check if necessary
            // If processes was continued by means of SIGCONT
            else if(WIFCONTINUED(processStatus))
            {
                it->second->jobStatus = "";
                ++it;
            }
        }

        // if job did NOT change status, do not modify its entry in jobs
        else
        {
            ++it;
        }
    }
    return false;
}
