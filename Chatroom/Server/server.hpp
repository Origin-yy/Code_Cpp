#include "TCPServer.hpp"
#include "ThreadPool.hpp"
#include "ThreadPool.cc"
#include "../lib/Command.hpp"
#include "redis.hpp"
#include <bits/types/time_t.h>
#include <cerrno>
#include <cstdio>
#include <netinet/in.h>
#include <string>
#include <sys/epoll.h>
#include <map>
#include <hiredis/hiredis.h> 
#include <ctime>

#define QUIT            0
#define LOGHIN_CHECK    1
#define REGISTER_CHECK  2
#define ADDFRIEND       3
#define ADDGROUP        4
#define AGREEADDFRIEND  6
#define LISTFRIEND      7
#define LISTGROUP       8

struct Argc_func{
public:
    Argc_func(TcpSocket fd_class, string command_string) : cfd_class(fd_class), command_string(command_string) {}
    TcpSocket cfd_class;
    string command_string;
};

void my_error(const char* errorMsg);  // 错误函数
string GetNowTime();                  // h获得当前时间
void taskfunc(void * arg);            // 处理一条命令的任务函数

using namespace std;

extern Redis redis;
// 错误函数
void my_error(const char* errorMsg) {
    cout << errorMsg << endl;
    strerror(errno);
    exit(1);
}

string GetNowTime(){
    time_t nowtime;
    struct tm*p;
    time(&nowtime);
    p = localtime(&nowtime);
    string now_time = "——" + to_string(p->tm_hour) + ":" + to_string(p->tm_min) + "——" +
                        to_string(p->tm_mon+1) + "." + to_string(p->tm_mday);
    return now_time;
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
                if(pwd != command.m_option[0]){   // 密码错误
                    cfd_class.sendMsg("incorrect");
                }else if(redis.gethash(command.m_uid, "在线状态") != "-1"){  // 用户在登录
                    cfd_class.sendMsg("online");
                }
                else{            // 可以登录，并改变登录状态
                    cfd_class.sendMsg("ok");
                    cout << "已发送ok" << endl;  
                    redis.hsetValue(command.m_uid, "在线状态", to_string(cfd_class.getfd()));
                    redis.hsetValue("fd-uid对应表", to_string(cfd_class.getfd()), command.m_uid);
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
                    redis.hsetValue(new_uid, "昵称", new_uid);
                    redis.hsetValue(new_uid, "在线状态", "-1");
                    redis.hsetValue(new_uid, "性别", "未知");
                    redis.hsetValue(new_uid, "其他信息", "无");
                    redis.hsetValue(new_uid, "通知套接字", "-1");
                    redis.hsetValue(new_uid, "好友数量", "空");
                    redis.hsetValue(new_uid, "群聊列表", "空");
                    redis.hsetValue(new_uid, "群聊列表", "空");
                    redis.hsetValue(new_uid, "消息列表", "空");
                    cfd_class.sendMsg(new_uid);
                    break;
                }
            }
            break;
        }
        case ADDFRIEND:{
            // 判断准好友是否已在好友列表里
            if(redis.sismember("accounts", command.m_option[0])){
                cfd_class.sendMsg("ok");
                string msg = "来自用户" + command.m_uid + "的好友申请.\n" + "验证消息：\n" +
                             command.m_option[1] + "\n" + GetNowTime();
                json jn = msg;
                string msg_json = jn.dump();
                cout << "msg: " << msg << endl;
                redis.lpush(command.m_option[0] + "--系统", msg_json);
                string friend_recvfd = redis.gethash(command.m_option[0], "通知套接字");
                TcpSocket friendFd_class(stoi(friend_recvfd));
                friendFd_class.sendMsg("收到一条好友申请." + GetNowTime());
            }else{
                cfd_class.sendMsg("nofind");
            }
            break;
        }
        case AGREEADDFRIEND:{
            string friend_mark0  = redis.gethash(command.m_option[0], "昵称");                            // 获得申请者的昵称作为默认备注
            redis.hsetValue(command.m_uid + "的好友列表", command.m_option[0], friend_mark0);        // 在同意者好友列表里插入申请者的uid和默认备注
            cfd_class.sendMsg("ok");                                                                            // 给同意者回信
            string friendNum0 = redis.gethash(command.m_uid, "好友数量");                                  // 获得同意者当前的好友数量
            redis.hsetValue(command.m_uid, "好友数量", to_string(stoi(friendNum0)+1));     // 同意者的好友数量+1

            string friend_mark1 = redis.gethash(command.m_uid, "昵称");                                   // 获得同意者的昵称
            redis.hsetValue(command.m_option[0] + "的好友列表", command.m_uid, friend_mark1);        // 在申请者的好友列表里插入申请者的uid和默认备注
            redis.lpush(command.m_option[0] + "--系统", command.m_uid + "通过了您的好友申请.");              // 在申请者的系统消息里写入通过消息
            string friendNum1 = redis.gethash(command.m_option[0], "好友数量");                            // 获得申请者当前的好友数量
            redis.hsetValue(command.m_option[0], "好友数量", to_string(stoi(friendNum1)+1));// 申请者的好友数量+1
        }
        case LISTFRIEND : {
            string friendNum = redis.gethash(command.m_uid, "好友数量");
            if(friendNum == "0"){
                cfd_class.sendMsg("none");
            }else {
                redisReply **f_uid = redis.hkeys(command.m_uid + "的好友列表");
                for(int i = 0; i < stoi(friendNum); i++){
                    string friend_mark = redis.gethash(command.m_uid + "的好友列表", f_uid[i]->str);
                    string isonline = redis.gethash(f_uid[i]->str, "在线状态");
                    if(isonline != "-1"){
                        cfd_class.sendMsg(friend_mark + "-" + f_uid[i]->str + "(*)");
                    }else {
                        cfd_class.sendMsg(friend_mark + "-" + f_uid[i]->str + "(-)");
                    }
                }
                cfd_class.sendMsg("end");
            }
        }

    }
    return;
}