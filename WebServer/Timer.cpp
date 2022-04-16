#include "Timer.h"
#include<sys/time.h>
#include<unistd.h>
#include<queue>

TimerNode::TimerNode(std::shared_ptr<HttpData> requestData,int timeout):deleted_(false),SPHttpData(requestData){
    struct timeval now;
    gettimeofday(&now, NULL);
    expiredTime_=((now.tv_sec*1000)+(now.tv_usec/1000))+timeout;
}

TimerNode::~TimerNode(){
    if(SPHttpData) SPHttpData->handleClose();
}

