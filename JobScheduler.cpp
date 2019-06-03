//
// Created by merkm on 03/06/2019.
//

#include <iostream>
#include "JobScheduler.h"

int doJob(){
  std::this_thread::sleep_for(std::chrono::seconds(3));
  //job.execution_time += 3000;
  std::cout<<"sono dentro il thread "<<std::endl;
  //int remainingTime = job.duration - job.execution_time;
}

std::function<void()> doTask = [] () {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        std::cout<<"sono dentro il thread "<<std::endl;
};

void JobScheduler::submit(Job j) {
  this->waitingJobs.push(j);
}

void JobScheduler::start() {
  auto startTime = std::chrono::system_clock::now();
  this->threadPool.initThreadPool();

  while(true){
    if (waitingJobs.empty() && this->threadPool.getJobQueue().getQueue().empty()){
      this->threadPool.done = true;
      return;
    }
    if(!waitingJobs.empty()){
      Job job = waitingJobs.top();
      if(startTime + std::chrono::milliseconds(waitingJobs.top().start_time) > std::chrono::system_clock::now()){
        std::this_thread::sleep_for((startTime + std::chrono::milliseconds(waitingJobs.top().start_time)) - std::chrono::system_clock::now());
        this->waitingJobs.pop();
        //std::function<void()> f = std::move(doJob);
        std::lock_guard<std::mutex> lck(threadPool.mtx);
        //this->threadPool.pushTask(std::bind(job.doJob2(),job));
        //this->threadPool.pushTask(job());
        this->threadPool.pushJob(job);
        threadPool.notified = true;
        threadPool.cv.notify_all();
      }else{
        this->waitingJobs.pop();
        std::lock_guard<std::mutex> lck(threadPool.mtx);
        //this->threadPool.pushTask(job());
        this->threadPool.pushJob(job);
        //this->threadPool.pushTask(std::bind(&doJob2,job));
        threadPool.notified = true;
        threadPool.cv.notify_all();

      }
    }
  }

}

JobScheduler::~JobScheduler() {

}

JobScheduler::JobScheduler(): threadPool(num_threads){

}

