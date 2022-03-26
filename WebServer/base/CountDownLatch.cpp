#include"CountDownLatch.h"

CountDownLatch::CountDownLatch(int count):mutexLock_(), condition_(mutexLock_), count_(count){}

void CountDownLatch::wait(){
    MutexLockGuard lock(mutexLock_);
    while(count_>0) condition_.wait();
}

void CountDownLatch::countDown(){
    MutexLockGuard lock(mutexLock_);
    --count_;
    if(count_==0) condition_.notifyAll();
}