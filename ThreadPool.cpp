//
// Created by merkm on 02/06/2019.
//

#include <iostream>
#include "ThreadPool.h"
#include <future>

void doNothing() {}

ThreadPool::ThreadPool(int nr_threads): done(false) {
  if(nr_threads <= 0)
    this->thread_count = std::thread::hardware_concurrency();
  else
    this->thread_count = nr_threads;
}


void ThreadPool::worker_thread() {
  std::unique_lock<std::mutex> lck(mtx);
  while(!done){
    while(!notified){
      std::cout<<"thread in attesa"<<std::endl;
      cv.wait(lck);
      std::cout<<"thread risvegliato"<<std::endl;
    }
    while(!task_queue.getQueue().empty()){
      std::function<Job()> f = this->task_queue.get();
      Job result = f();
      std::cout<<"remaining time: "<<result.duration - result.execution_time<<std::endl;
      if(result.duration - result.execution_time > 0){
        //std::lock_guard<std::mutex> lck(mtx);
        this->pushTask(result());
        //notified = true;
        //cv.notify_all();
      } else{
        std::cout<<"Job "<<result.id<<" terminated"<<std::endl;
        this->finishedJob.put(result);
      }


    }
    notified = false;
    }


}

ThreadPool::~ThreadPool() {
  this->done = true;
  //for (unsigned int i=0; i < thread_count; ++i) pushTask(&doNothing);
  for(auto & th: this->threads){
    if(th.joinable())
      th.join();

  }

}

void ThreadPool::initThreadPool() {
  for(unsigned int i = 0; i < this->thread_count; i++){
    this->threads.emplace_back(std::thread(&ThreadPool::worker_thread,this));
  }

}

const SynchronizedQueue<std::function<Job()>> &ThreadPool::getTaskQueue() const {
  return task_queue;
}

