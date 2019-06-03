//
// Created by merkm on 02/06/2019.
//

#ifndef ROUND_ROBIN_SCHEDULER_THREADPOOL_H
#define ROUND_ROBIN_SCHEDULER_THREADPOOL_H


#include <atomic>
#include <functional>
#include <list>
#include <mutex>
#include <vector>
#include <thread>
#include <future>
#include "Job.h"
#include "SynchronizedQueue.h"



static const int n_threads = 2;
class ThreadPool {

    unsigned int thread_count;
    SynchronizedQueue<std::function<Job()>> task_queue;
    //SynchronizedQueue<std::function<void()>> task_queue2;

    SynchronizedQueue<Job> finishedJob;
    std::vector<std::thread> threads;
    //std::vector<std::packaged_task<int()>> threads;
    void worker_thread();

public:
    std::atomic<bool> done;
    std::mutex mtx;
    std::condition_variable cv;
    bool notified = false;

    explicit ThreadPool(int nr_threads);
    virtual ~ThreadPool();
    void initThreadPool();

    void pushTask(std::function<Job()> func) {
     this->task_queue.put(func);
    }
    /*void pushTask2(std::function<void()> func) {
      this->task_queue2.put(func);
    }*/

    int getTaskQueueLength(){
      return this->task_queue.size();
    }

    bool getDone(){
      return this->done;
    }




    const SynchronizedQueue<std::function<Job()>> &getTaskQueue() const;

};


#endif //ROUND_ROBIN_SCHEDULER_THREADPOOL_H
