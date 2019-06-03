//
// Created by merkm on 03/06/2019.
//

#ifndef ROUND_ROBIN_SCHEDULER_SYNCHRONIZEDQUEUE_H
#define ROUND_ROBIN_SCHEDULER_SYNCHRONIZEDQUEUE_H

#include <condition_variable>
#include <list>
#include <mutex>

template <typename T>
class SynchronizedQueue {

    SynchronizedQueue(const SynchronizedQueue &)=delete;
    SynchronizedQueue & operator=(const SynchronizedQueue &)=delete;
    std::list<T> queue;
    std::mutex myMutex;
    std::condition_variable myCv;
public:
    SynchronizedQueue() = default;
    void put(const T & data);
    T get();
    size_t size();

    const std::list<T> &getQueue() const;


};

template<typename T>
const std::list<T> &SynchronizedQueue<T>::getQueue() const {
  return queue;
}

template<typename T>
void SynchronizedQueue<T>::put(const T &data) {
  std::unique_lock<std::mutex> lck(myMutex);
  queue.push_back(data);
  myCv.notify_one();
}

template<typename T>
T SynchronizedQueue<T>::get() {
  std::unique_lock<std::mutex> lck(myMutex);
  while(queue.empty())
    myCv.wait(lck);
  T result=queue.front();
  queue.pop_front();
  return result;
}

template<typename T>
size_t SynchronizedQueue<T>::size() {
  std::unique_lock<std::mutex> lck(myMutex);
  return queue.size();
}


#endif //ROUND_ROBIN_SCHEDULER_SYNCHRONIZEDQUEUE_H
