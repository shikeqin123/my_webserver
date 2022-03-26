#pragma once

#include<pthread.h>
#include<errno.h>
#include"MutexLock.h"
#include"nocopyable.h"

class Condition{
public:
    Condition(MutexLock &mutexLock):mutexLock_(mutexLock){
        pthread_cond_init(&cond,NULL);
    }
    ~Condition(){
        pthread_cond_destroy(&cond);
    }
    void wait(){ pthread_cond_wait(&cond, mutexLock_.get());};
    void notify(){ pthread_cond_signal(&cond);}
    void notifyAll(){ pthread_cond_broadcast(&cond);}
    bool waitForSeconds(int seconds){
        timespec abstime;
        clock_gettime(CLOCK_REALTIME, &abstime);
        abstime.tv_sec+=static_cast<time_t>(seconds);
        return ETIMEDOUT == pthread_cond_timedwait(&cond, mutexLock_.get(), &abstime);
    }
private:
    MutexLock &mutexLock_;
    pthread_cond_t cond;
};