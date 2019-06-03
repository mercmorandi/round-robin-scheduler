//
// Created by merkm on 31/05/2019.
//

#ifndef ROUND_ROBIN_SCHEDULER_WORKER_H
#define ROUND_ROBIN_SCHEDULER_WORKER_H

#include <mutex>
#include "condition_variable"


#include <thread>


class Worker {

public:
    //std::mutex mtxWorker;  //toglile da public
    //std::condition_variable cvWorker;
    Worker();
    std::thread thread;
    bool busy;

};


#endif //ROUND_ROBIN_SCHEDULER_WORKER_H
