//
// Created by merkm on 28/05/2019.
//

#ifndef ROUND_ROBIN_SCHEDULER_JOBSCHEDULER_H
#define ROUND_ROBIN_SCHEDULER_JOBSCHEDULER_H

#include <queue>
#include <thread>
#include <future>
#include <condition_variable>
#include <mutex>
#include "Job.h"
#include "Worker.h"


static std::condition_variable threadPoolCv;
static std::mutex runningJobsQueueMutex;
static std::mutex finishedMutex;
static std::mutex updateWorkerCounter;


static const int numThreads = 1;
static int usedThreads = 0;
//long int jobSchedulerClock = 0;

class JobScheduler {

    //std::vector<Job> waitingJobs;
    std::queue<Job> runningJobs;
    std::priority_queue<Job, std::vector<Job>> waitingJobs;
    std::queue<Job> finishedJobs;
    std::array<Worker, numThreads> threadPool;
    std::mutex threadPoolMutex;

    //std::chrono::milliseconds now;
    std::chrono::time_point<std::chrono::system_clock> now;

public:
    JobScheduler();
    void submit(const Job& j);
    void start();
    int threadPoolReady = numThreads;
    void enqueue(Job j);
    Job dequeue();

     std::queue<Job> &getRunningJobs();

     std::queue<Job> &getFinishedJobs();

protected:
    void initThreadPool();
    void makeRunnable(Job& j);
    Worker& launchThread(Job& j);
    Worker& getFreeWorker();


};


#endif //ROUND_ROBIN_SCHEDULER_JOBSCHEDULER_H
