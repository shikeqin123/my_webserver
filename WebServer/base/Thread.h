#pragma once

#include"nocopyable.h"
#include"CurrentThread.h"
#include"CountDownLatch.h"

#include<string>
#include<functional>
#include<pthread.h>

class Thread:nocopyable{
public:
    typedef std::function<void()> ThreadFunc;
    explicit Thread(const ThreadFunc&, const std::string &name=std::string());
    ~Thread();
    void start();
    int join();
    bool started() const { return started_;}
    pid_t tid() const { return tid_;}
    const std::string& name() const { return name_;}

private:
    void setDefaultName();
    bool started_;
    bool joined_;
    pthread_t pthreadId_;
    pid_t tid_;
    ThreadFunc func_;
    std::string name_;
    CountDownLatch latch_;
};