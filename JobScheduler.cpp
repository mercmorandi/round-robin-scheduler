//
// Created by merkm on 28/05/2019.
//

#include "JobScheduler.h"
#include <iostream>
#include <algorithm>
#include <mutex>
#include <shared_mutex>

long int executeJob(Job &job, JobScheduler&  father, Worker& worker) {
  std::this_thread::sleep_for(std::chrono::seconds(3));
  job.execution_time += 3000;
  /*if (job.duration <= job.execution_time) {
    job.completion_time += job.execution_time; //start_time??!
  }*/
  long int result = job.duration - job.execution_time;
  if (result <= 0) {
    //std::lock_guard<std::mutex> lck(finishedMutex);
    father.getFinishedJobs().push(job);
  } else {
    //std::lock_guard<std::mutex> lck(runningJobsQueueMutex);
    father.getRunningJobs().push(job);

  }
  //freeWorker->thread.join();
  //freeWorker->busy = false;
  std::cout << " -> Result from job: "<<job.id <<" remaining time = "<<result<<" on "<< job.duration<<std::endl;
  std::unique_lock<std::mutex> lock(updateWorkerCounter);
  father.threadPoolReady++;
  threadPoolCv.notify_one(); //one?????
  std::unique_lock<std::mutex> lck(getFreeWorkerMutex);
  worker.busy=false;
  return job.duration - job.execution_time;
}


JobScheduler::JobScheduler(){
  this->now = std::chrono::system_clock::now();
}

void JobScheduler::submit(const Job &j) {
  waitingJobs.push(j);
}

void JobScheduler::start() {
  auto startTime = std::chrono::system_clock::now();
  this->initThreadPool();
  Job job;
  while (true) {
    if (waitingJobs.empty() && runningJobs.empty()){
      return;
    }

    if(!waitingJobs.empty()){
      job = waitingJobs.top();
      if(startTime + std::chrono::milliseconds(job.start_time) > std::chrono::system_clock::now()){
            // wait untill startTime + job.startTime <= now()
            std::this_thread::sleep_for((startTime + std::chrono::milliseconds(job.start_time)) - std::chrono::system_clock::now());
      }

        this->makeRunnable(job);
        if (!runningJobs.empty()) {
          job = this->dequeue();
          Worker& backWorker= this->launchThread(job);

          //backWorker.thread.join();

          //backWorker->thread.join();
          //backWorker->busy = false;
        }
    }
  }
}

void JobScheduler::initThreadPool() {
  for (int i = 0; i < numThreads; ++i) {
    this->threadPool[i] = Worker();
  }
}

void JobScheduler::makeRunnable(Job &j) {
  //add lock guard on runningJobs enqueue
  this->waitingJobs.pop();
  this->enqueue(j);
}


Worker& JobScheduler::launchThread(Job &j) {

  {
    std::unique_lock<std::mutex> lck(updateWorkerCounter);
    while (threadPoolReady <= 0)
      threadPoolCv.wait(lck);
  }
  Worker& freeWorker = this->getFreeWorker();
  std::packaged_task<long int(Job &,JobScheduler &,Worker &)> task(executeJob);
  //std::future<long int> result = task.get_future();
  freeWorker.thread = std::thread(std::move(task), std::ref(j),std::ref(*this),std::ref(freeWorker));
  freeWorker.thread.detach();

  return freeWorker;
}

Worker& JobScheduler::getFreeWorker() {
  //static std::mutex mtx;
  for (auto &worker : this->threadPool) {
    std::unique_lock<std::mutex> lock(getFreeWorkerMutex);
    if (!worker.busy) {
      //freeWorker = &worker;
      this->threadPoolReady--;
      worker.busy = true;
      return  worker;
    }
  }
  //return freeWorker;
}
  /*while(freeWorker == nullptr){
    //std::unique_lock<std::mutex> lock(this->threadPoolMutex);
    std::lock_guard<std::mutex> lock(this->threadPoolMutex);
    for(auto & worker : this->threadPool){
      if(!worker.busy){
        freeWorker = &worker;
        freeWorker->busy = true;
        break;
      }

    }*/
    //std::unique_lock<std::mutex> unlock(this->threadPoolMutex);
  //}
  //return freeWorker;


void JobScheduler::enqueue(Job j) {
  //std::lock_guard<std::mutex> lock(runningJobsQueueMutex);
  this->runningJobs.push(j);
}

Job JobScheduler::dequeue() {
  std::lock_guard<std::mutex> lock(runningJobsQueueMutex);
  Job j = this->runningJobs.front();
  this->runningJobs.pop();
  return j;
}

 std::queue<Job> &JobScheduler::getRunningJobs(){
  return this->runningJobs;
}

 std::queue<Job> &JobScheduler::getFinishedJobs()  {
  return this->finishedJobs;
}
