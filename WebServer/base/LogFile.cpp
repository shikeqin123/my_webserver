#include<assert.h>
#include<stdio.h>
#include<time.h>
#include "LogFile.h"

LogFile::LogFile(const std::string &basename, int flushEveryN)
    :basename_(basename), flushEveryN_(flushEveryN), count_(0), mutexLock_(new MutexLock()){
        file_.reset(new AppendFile(basename));
}

void LogFile::append(const char* logline, int len){
    MutexLockGuard lock(*mutexLock_);
    append_unlocked(logline,len);
}

void LogFile::flush(){
    MutexLockGuard lock(*mutexLock_);
    file_->flush();
}

void LogFile::append_unlocked(const char* logline, int len){
    file_->append(logline,len);
    ++count_;
    if(count_>=flushEveryN_){
        count_=0;
        file_->flush();
    }
}