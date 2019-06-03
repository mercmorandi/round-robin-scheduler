//
// Created by merkm on 28/05/2019.
//

#ifndef ROUND_ROBIN_SCHEDULER_JOB_H
#define ROUND_ROBIN_SCHEDULER_JOB_H

#include <stdbool.h>
#include <chrono>
#include <iostream>
#include <thread>


class Job {
public:

    Job doJob2(Job job) const;

    long int id;
    //std::chrono::milliseconds startTime;
    long int start_time;
    long int duration;
    long int execution_time;
    long int wait_time;
    long int completion_time;



    Job(long int id, long int start_time, long int duration);

    Job(const Job &job);

  /*  Job(Job &&job) : id(job.id),
                     start_time(job.start_time),
                     duration((job.duration)),
                     execution_time((job.execution_time)),
                     wait_time((job.wait_time)),
                     completion_time((job.completion_time)) {};*/


Job() = default;

Job &operator()();

Job &operator=(const Job &job);

// bool operator<(const Job& lhs, const Job& rhs);
bool operator<(const Job &job) const;
//void executeJob(std::promise<long int> & promise);
~

Job() = default;

};


#endif //ROUND_ROBIN_SCHEDULER_JOB_H
