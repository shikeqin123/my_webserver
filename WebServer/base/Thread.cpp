#include"Thread.h"
#include"CurrentThread.h"
#include<assert.h>
#include<sys/prctl.h>

struct ThreadData
{
    typedef Thread::ThreadFunc ThreadFunc;
    ThreadFunc func_;
    std::string name_;
    pid_t* tid_;
    CountDownLatch* latch_;

    ThreadData(const ThreadFunc& func, const std::string &name, pid_t* tid, CountDownLatch* latch)
        :func_(func), name_(name), tid_(tid), latch_(latch){}
    
    void runInThread(){
        *tid_=CurrentThread::tid();
        tid_=NULL;
        latch_->countDown();
        latch_=NULL;

        CurrentThread::t_threadName=name_.empty()?"Thread":name_.c_str();
        prctl(PR_SET_NAME,CurrentThread::t_threadName);

        func_();
        CurrentThread::t_threadName="finished";
    }
};

void* startThread(void* obj){
    ThreadData* data=static_cast<ThreadData*>(obj);
    data->runInThread();
    delete data;
    return NULL;
}

Thread::Thread(const ThreadFunc& func, const std::string& n)
    :started_(false), joined_(false),pthreadId_(0),tid_(0),func_(func),name_(n),latch_(1){
        setDefaultName();
}

Thread::~Thread(){
    if(started_&&!joined_) pthread_detach(pthreadId_); 
}

void Thread::start(){
    assert(!started_);
    started_=true;
    ThreadData* data =new ThreadData(func_, name_, &tid_, &latch_);
    if(pthread_create(&pthreadId_, NULL, &startThread, data)){
        started_=false;
        delete data;
    }else{
        latch_.wait();
        assert(tid_>0);
    }
}

int Thread::join(){
    assert(started_);
    assert(!joined_);
    joined_=true;
    return pthread_join(pthreadId_,NULL);
}
