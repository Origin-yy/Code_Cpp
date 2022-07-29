#include "TCPServer.hpp"
#include "ThreadPool.hpp"
#include "ThreadPool.cc"
#include "../lib/Command.hpp"
#include "redis.hpp"
#include <cerrno>
#include <cstdio>
#include <netinet/in.h>
#include <string>
#include <sys/epoll.h>
#include <map>
#include <hiredis/hiredis.h> 
#include <ctime>

#define LOGHIN_CHECK    1
#define REGISTER_CHECK  2
#define ADDFRIEND       3
#define ADDGROUP        4
#define AGREEADD        5

struct Argc_func{
public:
    Argc_func(TcpSocket fd_class, string command_string) : cfd_class(fd_class), command_string(command_string) {}
    TcpSocket cfd_class;
    string command_string;
};

void my_error(const char* errorMsg);  //错误函数
void taskfunc(void * arg);            //处理一条命令的任务函数

using namespace std;

extern Redis redis;
// 错误函数
void my_error(const char* errorMsg) {
    cout << errorMsg << endl;
    strerror(errno);
    exit(1);
}
// 任务函数，获取客户端发来的命令，解析命令进入不同模块，并进行回复
void taskfunc(void *arg){
    Argc_func *argc_func = static_cast<Argc_func*>(arg);
    Command command;                               // Command类存客户端的命令内容
    TcpSocket cfd_class = argc_func->cfd_class;    // TcpSocket类用于通信
    command.From_Json(argc_func->command_string);    // 命令类将json字符串格式转为josn格式，再存到command类里
    cout << command.m_uid << endl << command.m_flag << endl << command.m_option[0] << endl;
    switch (command.m_flag) {
        case LOGHIN_CHECK :{
            // 从数据库调取对应数据进行核对，并回复结果
            if(!redis.sismember("accounts", command.m_uid)){  // 如果没有账号，返回错误
                cfd_class.sendMsg("incorrect");
            }else{         // 否则账号存在，通过uid找到这个用户的哈希表，进行密码匹配和登录工作
                string pwd = redis.gethash(command.m_uid, "密码");
                cout << pwd << endl;
                if(pwd != command.m_option[0]){   // 密码错误
                    cfd_class.sendMsg("incorrect");
                }else if(redis.gethash(command.m_uid, "在线状态") != "-1"){  // 用户在登录
                    cfd_class.sendMsg("online");
                }
                else{            // 可以登录，并改变登录状态
                    cfd_class.sendMsg("ok");   
                    redis.hsetValue(command.m_uid, "在线状态", to_string(cfd_class.getfd()));
                }
            }
            break;
        }
        case REGISTER_CHECK :{
            srand((unsigned)time(NULL));
            while(true){
                string new_uid = to_string((rand()+9999)%10000);
                if(redis.sismember("accounts", new_uid)){  // 直到随机一个未注册  uid发过去，并建立账号基本信息
                    continue;
                }else{
                    redis.saddvalue("accounts", new_uid);
                    redis.hsetValue(new_uid, "账号", new_uid);
                    redis.hsetValue(new_uid, "密码", command.m_option[0]);
                    redis.hsetValue(new_uid, "在线状态", "-1");
                    redis.hsetValue(new_uid, "性别", "未知");
                    redis.hsetValue(new_uid, "其他信息", "无");
                    redis.hsetValue(new_uid, "好友列表有无", "无");
                    redis.hsetValue(new_uid, "群聊列表有无", "无");
                    cfd_class.sendMsg(new_uid);
                    break;
                }
            }
            break;
        }
        case ADDFRIEND:{
            if(redis.sismember("accounts", command.m_option[0])){
                redis.lpush(command.m_option[0] + "的系统消息", "来自用户" + command.m_uid + "的好友申请" + "验证消息为：" + command.m_option[1]);
                cfd_class.sendMsg("")
            }
        }
    }
    return;
}