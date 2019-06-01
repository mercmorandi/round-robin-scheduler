//
// Created by merkm on 28/05/2019.
//

#ifndef ROUND_ROBIN_SCHEDULER_JOB_H
#define ROUND_ROBIN_SCHEDULER_JOB_H

#include <stdbool.h>
#include <chrono>


class Job {
public:

    long int id;
    //std::chrono::milliseconds startTime;
    long int start_time;
    long int duration;
    long int execution_time;
    long int wait_time;
    long int completion_time;

    Job(long int id,  long int start_time, long int duration);
    Job(const Job& job);
    Job() = default;
    Job& operator=(const Job& job);
   // bool operator<(const Job& lhs, const Job& rhs);
    bool operator<(const Job& job) const;
    //void executeJob(std::promise<long int> & promise);
    ~Job()= default;

};


#endif //ROUND_ROBIN_SCHEDULER_JOB_H