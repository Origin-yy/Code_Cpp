#include "TCPServer.cc"
#include "../lib/ThreadPool.cc"
#include "nlohmann/json.hpp"
#include "../lib/Command.hpp"
#include <cerrno>
#include <cstdio>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <map>

void my_error(const char* errorMsg);  //错误函数
void taskfunc(void * arg);            //处理一条命令的任务函数