//
// Created by merkm on 03/06/2019.
//

#ifndef ROUND_ROBIN_SCHEDULER_JOBSCHEDULER_H
#define ROUND_ROBIN_SCHEDULER_JOBSCHEDULER_H

#include "ThreadPool.h"
#include <queue>
#include "Job.h"

static const int num_threads = 2;



class JobScheduler {

    std::priority_queue<Job, std::vector<Job>> waitingJobs;
    std::queue<Job> runningJob;
    SynchronizedQueue<std::function<void(Job)>> runningFuncJobs;
    ThreadPool threadPool;




public:
    JobScheduler();
    void submit(Job j);
    void start();

    virtual ~JobScheduler();


};


#endif //ROUND_ROBIN_SCHEDULER_JOBSCHEDULER_H
