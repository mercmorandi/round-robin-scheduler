//
// Created by merkm on 02/06/2019.
//

#include <iostream>
#include "ThreadPool.h"
#include <future>

std::mutex mtx_cout;

void doNothing() {}

std::string getTime() {
  auto nowTime = std::chrono::system_clock::now();
  std::time_t sleepTime = std::chrono::system_clock::to_time_t(nowTime); //time to outoput
  return std::ctime(&sleepTime);
}

ThreadPool::ThreadPool(int nr_threads) : done(false) {
  if (nr_threads <= 0)
    this->thread_count = std::thread::hardware_concurrency();
  else
    this->thread_count = nr_threads;
}


void ThreadPool::worker_thread() {

  while (!done) {
    while (!notified) {
      std::unique_lock<std::mutex> lck(mtx);
      std::cout << "thread in attesa" << std::endl;
      cv.wait(lck);
      std::cout << "thread risvegliato" << std::endl;
    }
    //while(!task_queue.getQueue().empty()){
    while (!job_queue.getQueue().empty()) {
      //std::function<Job()> f = this->task_queue.get();
      Job &result = this->job_queue.get()();
      //Job result = f();
      //Job result = job;
      //std::cout<<"remaining time: "<<result.duration - result.execution_time<<std::endl;
      //std::cout<<"thread id = "<<std::this_thread::get_id()<<" -- remaining time = "<<result.duration - result.execution_time<<" -- job "<<result.id<<std::endl;
      std::lock_guard<std::mutex> lock(mtx_cout);
      //mtx_cout.lock();
      std::cout <<"THREAD:>"<<getTime()<< "id = "<<std::this_thread::get_id()<< " running job:" << result.id<< " -- remaining time = " << result.duration - result.execution_time << std::endl;
      //mtx_cout.unlock();
      if (result.duration - result.execution_time > 0) {
        std::unique_lock<std::mutex> lck(mtx);
        //this->pushTask(result());
        this->pushJob(result);
        notified = true;
        //cv.notify_all();
      } else {
        std::cout << "Job " << result.id << " terminated" << std::endl;
        this->finishedJob.put(result);

      }
      notified = false;
    }
    //notified = false;
  }


}

ThreadPool::~ThreadPool() {
  this->done = true;
  //for (unsigned int i=0; i < thread_count; ++i) pushTask(&doNothing);
  for (auto &th: this->threads) {
    if (th.joinable())
      th.join();

  }

}

void ThreadPool::initThreadPool() {
  for (unsigned int i = 0; i < this->thread_count; i++) {
    this->threads.emplace_back(std::thread(&ThreadPool::worker_thread, this));
  }

}

const SynchronizedQueue<std::function<Job()>> &ThreadPool::getTaskQueue() const {
  return task_queue;
}

const SynchronizedQueue<Job> &ThreadPool::getJobQueue() const {
  return job_queue;
}


