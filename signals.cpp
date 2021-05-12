// signals.cpp
// contains signal handler functions
// contains the function/s that set the signal handlers


#include "signals.h"
#include "commands.h"
#include <cerrno>

extern pid_t lastFgPid;
extern bool isFgProcess;
extern map<unsigned int, pJob>* jobs;
extern string lastFgJobName;
extern unsigned int totalJobCount;

/*******************************************/
/* Name: handler_cntlz
   Synopsis: handle the Control-Z */
void handler_cntlz(int sig_num)
{
    if(isFgProcess)
    {
        int didKillSucceed = kill(lastFgPid, SIGTSTP);
        if(didKillSucceed == -1)
        {
            // If sys call failed, not because process doesn't exist
            if(errno == EINVAL || errno == EPERM)
            {
                fprintf(stderr, "smash error: > %s\n", strerror(errno));
            }
            //  If sys call failed because process doesn't exist, ignore
            return;
        }

        //If signal was successfully sent
        else
        {
            //  Print that signal was sent
            //TODO check for answer re use of strsignal()
            printf("smash > signal SIGTSTP was sent to pid %d\n", lastFgPid);
            updateJobs(jobs);

            //  Check if process was previously in jobs
            auto it = jobs->begin();
            while(it != jobs->end() && !jobs->empty())
            {
                pid_t currentJobPid = it->second->jobPid;

                // If job was already in jobs, update its status
                if(currentJobPid == lastFgPid)
                {
                    it->second->jobStatus = "(Stopped)";
                    return;
                }
                ++it;
            }

            // If job was not in jobs, add it
            pJob pNewJob = new Job;
            pNewJob->jobName = lastFgJobName;
            pNewJob->jobPid = lastFgPid;
            pNewJob->jobStartTime = time(NULL);
            pNewJob->jobStatus = "(Stopped)";

            totalJobCount++;
            unsigned int jobNumber = totalJobCount;
            jobs->insert({jobNumber, pNewJob});
        }
    }
}

/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */
void handler_cntlc(int sig_num)
{
    if(isFgProcess)
    {
        int didKillSucceed = kill(lastFgPid, SIGINT);
        if(didKillSucceed == -1)
        {
            // If sys call failed, not because process doesn't exist
            if(errno == EINVAL || errno == EPERM)
            {
                fprintf(stderr, "smash error: > %s\n", strerror(errno));
            }
        }

        //If signal was successfully sent
        else
        {
            //TODO check for answer re use of strsignal()
            printf("smash > signal SIGINT was sent to pid %d\n", lastFgPid);
            updateJobs(jobs);
        }
    }
}
