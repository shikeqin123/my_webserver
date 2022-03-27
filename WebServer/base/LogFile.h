#pragma once
#include<memory>
#include<string>
#include"FileUtil.h"
#include"MutexLock.h"
#include"nocopyable.h"

//LogFile提供定次刷新功能，线程安全
class LogFile:nocopyable{
public:
    LogFile(const std::string &basename, int flushEveryN=1024);
    ~LogFile(){};

    void append(const char *logline, int len);
    void flush();
    //bool rollFile();

private:
    void append_unlocked(const char* logline, int len);

    const std::string basename_;
    const int flushEveryN_;

    int count_;
    std::unique_ptr<MutexLock> mutexLock_;
    std::unique_ptr<AppendFile> file_;
};