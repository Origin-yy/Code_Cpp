#include "TCPServer.hpp"
#include "ThreadPool.hpp"
#include "ThreadPool.cc"
#include "../lib/Command.hpp"
#include "redis.hpp"
#include <cerrno>
#include <cstdio>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <map>
#include <hiredis/hiredis.h> 

#define LOGIN_OPIONS     1
#define LOGHIN_CHECK    11
#define REGISTER_CHECK  12

void my_error(const char* errorMsg);  //错误函数
void taskfunc(void * arg);            //处理一条命令的任务函数

using namespace std;

extern redisContext* redis_s;
// 错误函数
void my_error(const char* errorMsg) {
    cout << errorMsg << endl;
    strerror(errno);
    exit(1);
}
// 任务函数，获取客户端发来的命令，解析命令进入不同模块，并进行回复
void taskfunc(void *arg){
    Command command; // Command类存客户端的命令内容
    string *command_string = static_cast<string*>(arg);
    command.From_Json(*command_string);    // 命令类将json字符串格式转为josn格式，再存到command类里
    switch (command.m_flag) {
        case LOGHIN_CHECK :{
            // 从数据库调取对应数据进行核对，并回复结果
            
            break;
        }
        case REGISTER_CHECK :
            break;
    }
}