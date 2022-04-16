#pragma once
#include<cstdlib>
#include<string>

ssize_t readn(int fd,void *buff,size_t n);
ssize_t readn(int fd,std::string &inBuffer,bool &zero);
ssize_t readn(int fd, std::string &inBuffer);
ssize_t writen(int fd, std::string &sbuff);
ssize_t writern(int fd,std::string &sbuff);
void handle_for_sigpipe();
int setSocketForNonBlocking(int fd);
void setSocketNoDelay(int fd);
void setSockerNoLinger(int fd);
void shutDownWR(int fd);
int socket_bind_listen(int port);