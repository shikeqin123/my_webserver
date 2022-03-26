#pragma once

#include "Condition.h"
#include"MutexLock.h"
#include"nocopyable.h"

class CountDownLatch:nocopyable{
public:
    explicit CountDownLatch(int count);
    void wait();
    void countDown();
private:
    mutable MutexLock mutexLock_;
    Condition condition_;
    int count_;

};