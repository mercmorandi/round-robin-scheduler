#include <iostream>
#include "JobScheduler.h"


int main() {
  std::cout << "Hello, World!" << std::endl;
  JobScheduler p;
  p.submit(Job(1, 0, 15000));
  p.submit(Job(2, 0, 6000));
  p.submit(Job(3, 1000, 9000));
  p.submit(Job(4, 2000, 12000));
  p.submit(Job(5, 3000, 16000));
  p.submit(Job(6, 3000, 5000));
  p.submit(Job(7, 4000, 7000));
  p.submit(Job(8, 4000, 6000));
  p.submit(Job(9, 5000, 9000));
  p.start();
  return 0;
}