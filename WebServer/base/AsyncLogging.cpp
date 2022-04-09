#include<assert.h>
#include<stdio.h>
#include<unistd.h>
#include<functional>
#include"AsyncLogging.h"
#include"LogFile.h"

AsyncLogging::AsyncLogging(std::string logFileName, int flushInterval)
    :flushInterval_(flushInterval),
    running_(false),
    basename_(logFileName),
    thread_(std::bind(&AsyncLogging::threadFunc, this), "Logging"),mutex_(),
    cond_(mutex_),
    currentBuffer_(new Buffer),
    nextBuffer_(new Buffer),
    buffers_(),
    latch_(1){
        assert(logFileName.size()>0);
        currentBuffer_->bzero();
        nextBuffer_->bzero();
        buffers_.reserve(16);
}

//前端
void AsyncLogging::append(const char* logline, int len){
    MutexLockGuard lock(mutex_);
    if(currentBuffer_->avail()>len){
        currentBuffer_->append(logline,len);
    }else{
        buffers_.push_back(currentBuffer_);
        currentBuffer_.reset();
        if(nextBuffer_){
            currentBuffer_=std::move(nextBuffer_);
        }else{
            currentBuffer_.reset(new Buffer());
        }
        currentBuffer_->append(logline,len);
        //唤醒条件变量定时等待的写入线程
        cond_.notify();
    }
}

//后端
void AsyncLogging::threadFunc(){
    assert(running_==true);
    latch_.countDown();
    LogFile output(basename_);
    printf("start log thread\n");
    BufferPtr newBuffer1(new Buffer());
    BufferPtr newBuffer2(new Buffer());
    newBuffer1->bzero();
    newBuffer2->bzero();
    BufferVector buffersToWrite;
    buffersToWrite.reserve(16);
    while(running_){
        assert(newBuffer1 && newBuffer1->length()==0);
        assert(newBuffer2 && newBuffer2->length()==0);

        {
            MutexLockGuard lock(mutex_);
            if(buffers_.empty()){
                cond_.waitForSeconds(flushInterval_);
            }
            buffers_.push_back(currentBuffer_);
            buffersToWrite.swap(buffers_);

            currentBuffer_.reset();
            currentBuffer_=std::move(newBuffer1);
            if(!nextBuffer_){
                nextBuffer_= std::move(newBuffer2);
            }
        }

        assert(!buffersToWrite.empty());
        
        //超负荷了
        if(buffersToWrite.size()>25){
            buffersToWrite.erase(buffersToWrite.begin() + 2, buffersToWrite.end());
        }

        for(size_t i=0; i<buffersToWrite.size();++i){
            output.append(buffersToWrite[i]->data(), buffersToWrite[i]->length());
        }

        if(buffersToWrite.size()>2){
            buffersToWrite.resize(2);
        }
        if(!newBuffer1){
            assert(!buffersToWrite.empty());
            newBuffer1 = buffersToWrite.back();
            buffersToWrite.pop_back();
            newBuffer1->reset();
        }
        if (!newBuffer2) {
            assert(!buffersToWrite.empty());
            newBuffer2 = buffersToWrite.back();
            buffersToWrite.pop_back();
            newBuffer2->reset();
        }
        buffersToWrite.clear();
        output.flush();
    }
    output.flush();
}
