#include "TCPServer.cc"
#include "../lib/ThreadPool.cc"
#include <jsoncpp/json/json.h>
#include <cerrno>
#include <cstdio>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <map>

void my_error(const char* errorMsg);  //错误函数
void loginTask(void *arg);   //登录任务函数
