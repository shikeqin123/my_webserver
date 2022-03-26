#pragma once

#include<stdint.h>
#include<linux/unistd.h>
#include<syscall.h>
#include<stdio.h>
#include<unistd.h>

namespace CurrentThread{

//extern 头文件不要定义全局变量，只能在源文件定义。该处为声明

extern __thread int t_cachedTid;
extern __thread char t_tidString[32];
extern __thread int t_tidStringLength;
extern __thread const char* t_threadName;

inline void cacheTid(){
    t_cachedTid=static_cast<pid_t>(::syscall(SYS_gettid));
    t_tidStringLength=snprintf(t_tidString, sizeof(t_tidString), "%5d", t_cachedTid);
}

inline int tid(){
    if(__glibc_unlikely(t_cachedTid==0)){
        cacheTid();
    }
    return t_cachedTid;
}

inline const char* tidString(){ return t_tidString;}

inline int tidStringLength(){ return t_tidStringLength;}

inline const char* name(){ return t_threadName;}

}