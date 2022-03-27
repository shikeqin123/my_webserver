#pragma once

#include<pthread.h>
#include"nocopyable.h"

class MutexLock:nocopyable{
public:
    MutexLock(){ pthread_mutex_init(&mutex_, NULL);}
    ~MutexLock(){
        pthread_mutex_lock(&mutex_);
        pthread_mutex_destroy(&mutex_);
    }
    void lock(){ pthread_mutex_lock(&mutex_);}
    void unlock(){ pthread_mutex_unlock(&mutex_);}
  // 友元类不受访问权限影响
private:
    pthread_mutex_t* get(){ return &mutex_;};
    friend class Condition;
private:
    pthread_mutex_t mutex_;

};

class MutexLockGuard:nocopyable{
public:
    MutexLockGuard(MutexLock &mutexLock):mutexLock_(mutexLock){
        mutexLock_.lock();
    }
    ~MutexLockGuard(){
        mutexLock_.unlock();
    }
private:
    MutexLock &mutexLock_;
};