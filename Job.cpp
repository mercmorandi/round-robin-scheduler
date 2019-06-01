//
// Created by merkm on 28/05/2019.
//

#include "Job.h"
#include <unistd.h>
#include <chrono>
#include <thread>


Job::Job(long int id, long int start_time, long int duration):
        id(id),start_time(start_time), duration(duration){
  this->execution_time = 0;
  this->wait_time = 0;
  this->completion_time = 0;
}
Job::Job(const Job& job) {
  this->id=job.id;
  this->start_time=job.start_time;
  this->execution_time=job.execution_time;
  this->duration=job.duration;
  this->wait_time=job.wait_time;
  this->completion_time=job.completion_time;
}
Job& Job::operator=(const Job &job) {
  this->id=job.id;
  this->start_time=job.start_time;
  this->execution_time=job.execution_time;
  this->duration=job.duration;
  this->wait_time=job.wait_time;
  this->completion_time=job.completion_time;
  return *this;
}

/*bool Job::operator<(const Job &lhs, const Job &rhs) {
  return lhs.start_time < rhs.start_time;
}*/
bool Job::operator<(const Job &job) const {
  return this->start_time > job.start_time;
}

