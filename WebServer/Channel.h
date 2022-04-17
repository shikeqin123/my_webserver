#pragma once
#include<sys/epoll.h>
#include<functional>
#include<memory>
#include<string>
#include<unordered_map>
#include"Timer.h"

class EventLoop;
class HttpData;

//channel实际上就是epoll上就绪的事件
//channel由httpdata持有
class Channel{
private:
    typedef std::function<void()> CallBack;
    int fd_;
    uint32_t events_;
    //recent event?
    uint32_t revents_;
    uint32_t lastEvents_;
    //方便找到上层持有该channel的对象
    std::weak_ptr<HttpData> holder_;

    CallBack readHandler_;
    CallBack writeHandler_;
    CallBack errorHandler_;
    CallBack connHandler_;

public:
    Channel();
    Channel(int fd);
    ~Channel();

    int getFd();
    void setFd(int fd);

    void setHolder(std::shared_ptr<HttpData> holder){
        holder_=holder;
    }
    std::shared_ptr<HttpData> getHolder(){
        std::shared_ptr<HttpData> ret(holder_.lock());
        return ret;
    }

    void setReadHandler(CallBack &&readHandler){
        readHandler_=readHandler;
    }
    void setWriteHandler(CallBack &&writeHandler){
        writeHandler_ = writeHandler;
    }
    void setErrorHandler(CallBack &&errorHandler){
        errorHandler_ = errorHandler;
    }
    void setConnHandler(CallBack &&connHandler){ 
        connHandler_ = connHandler; 
    }

    //event_的作用是什么？
    void handleEvent(){
        events_=0;
        if((revents_&EPOLLHUP)&&!(revents_&EPOLLIN)){
            return;
        }
        if(revents_&EPOLLERR){
            if(errorHandler_) errorHandler_();
            return;
        }
        if(revents_&(EPOLLIN|EPOLLPRI|EPOLLRDHUP)){
            handleRead();
        }
        if(revents_&EPOLLOUT){
            handleWrite();
        }
        handleConn();
    }
    void handleRead();
    void handleWrite();
    void handleError(int fd,int err_num,std::string short_msg);
    void handleConn();

    void setRevents(uint32_t ev){
        revents_=ev;
    }
    void setEvents(uint32_t ev){
        events_=ev;
    }
    uint32_t &getEvents(){
        return events_;
    }

    bool EqualAndUpdataLastEvents(){
        bool ret=(lastEvents_==events_);
        lastEvents_=events_;
        return ret;
    }

    uint32_t getLastEvents(){
        return lastEvents_;
    }
};

typedef std::shared_ptr<Channel> SP_Channel;