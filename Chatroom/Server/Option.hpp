#ifndef OPTION_HPP
#define OPTION_HPP

#include "TCPServer.hpp"
#include "../lib/Command.hpp"
#include "../lib/Color.hpp"
#include "redis.hpp"
#include <bits/types/FILE.h>
#include <cstdio>
#include <hiredis/hiredis.h>
#include <string>
#include <fcntl.h>

#define SETRECVFD       -1
#define QUIT             0
#define LOGHIN_CHECK     1
#define REGISTER_CHECK   2
#define ADDFRIEND        3
#define AGREEADDFRIEND   4
#define LISTFRIEND       5
#define CHATFRIEND       6
#define FRIENDMSG        7
#define EXITCHAT         8
#define SHIELDFRIEND     9
#define DELETEFRIEND    10
#define RESTOREFRIEND   11
#define NEWMESSAGE      12
#define LOOKSYSTEM      13
#define REFUSEADDFRIEND 14
#define CREATEGROUP     15
#define LISTGROUP       16
#define ADDGROUP        17
#define ADD             18
#define LOOKNOTICE      19
#define ABOUTGROUP      20
#define REQUSTLIST      21
#define PASSAPPLY       22
#define DENYAPPLY       23
#define SETMEMBER       24
#define EXITGROUP       25
#define DISPLAYMEMBER   26
#define REMOVEMEMBER    27
#define INFOXXXX        28
#define CHATGROUP       29
#define GROUPMSG        30
#define EXITGROUPCHAT   31
#define SENDFILE        32
#define RECVFILE        33

using namespace std;
extern Redis redis;
struct Argc_func{
public:
    Argc_func(TcpSocket fd_class, string command_string) : cfd_class(fd_class), command_string(command_string) {}
    TcpSocket cfd_class;
    string command_string;
};

void my_error(const char* errorMsg);  // 错误函数
string GetNowTime();                  // h获得当前时间
void taskfunc(void * arg);            // 处理一条命令的任务函数
void Login(TcpSocket cfd_class, Command command);
void Register(TcpSocket cfd_class, Command command);
void AddFriend(TcpSocket cfd_class, Command command);
void AddGroup(TcpSocket cfd_class, Command command);
void AgreeAddFriend(TcpSocket cfd_class, Command command);
void ListFriend(TcpSocket cfd_class, Command command);
void ChatFriend(TcpSocket cfd_class, Command command);
void ChatGroup(TcpSocket cfd_class, Command command);
void FriendMsg(TcpSocket cfd_class, Command command);
void GroupMsg(TcpSocket cfd_class, Command command);
void ExitChatGroup(TcpSocket cfd_class, Command command);
void ExitChatFriend(TcpSocket cfd_class, Command command);
void ShieldFriend(TcpSocket cfd_class, Command command);
void DeleteFriend(TcpSocket cfd_class, Command command);
void Restorefriend(TcpSocket cfd_class, Command command);
void NewMessage(TcpSocket cfd_class, Command command);
void LookSystem(TcpSocket cfd_class, Command command);
void LookNotice(TcpSocket cfd_class, Command command);
void RefuseAddFriend(TcpSocket cfd_class, Command command);
void CreateGroup(TcpSocket cfd_class, Command command);
void ListGroup(TcpSocket cfd_class, Command command);
void LookGroupApply(TcpSocket cfd_class, Command command);
void AboutGroup(TcpSocket cfd_class, Command command);
void RequestList(TcpSocket cfd_class, Command command);
void PassApply(TcpSocket cfd_class, Command command);
void DenyApply(TcpSocket cfd_class, Command command);
void SetMember(TcpSocket cfd_class, Command command);
void ExitGroup(TcpSocket cfd_class, Command command);
void DisplyMember(TcpSocket cfd_class, Command command);
void RemoveMember(TcpSocket cfd_class, Command command);
void InfoXXXX(TcpSocket cfd_class, Command command);
void SendFile(TcpSocket cfd_class, Command command);
void RecvFile(TcpSocket cfd_class, Command command);

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
    string t = "-";
    string now_time = TILT + t + to_string(p->tm_hour) + ":" + to_string(p->tm_min) + t +
                        to_string(p->tm_mon+1) + "." + to_string(p->tm_mday) + NONE;
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
        case LOGHIN_CHECK :
            Login(cfd_class, command);
            break;
        case REGISTER_CHECK :
            Register(cfd_class, command);
            break;
        case ADDFRIEND:
            AddFriend(cfd_class, command);
            break;
        case ADDGROUP:
            AddGroup(cfd_class, command);
            break;
        case AGREEADDFRIEND:
            AgreeAddFriend(cfd_class, command);
            break;
        case LISTFRIEND : 
            ListFriend(cfd_class, command);
            break;
        case CHATFRIEND:
            ChatFriend(cfd_class, command);
            break;
        case CHATGROUP:
            ChatGroup(cfd_class, command);
            break;
        case FRIENDMSG:
            FriendMsg(cfd_class, command);
            break;
        case GROUPMSG :
            GroupMsg(cfd_class, command);
            break;
        case EXITCHAT:
            ExitChatFriend(cfd_class, command);
            break;
        case EXITGROUPCHAT:
            ExitChatGroup(cfd_class, command);
            break;
        case SHIELDFRIEND:
            ShieldFriend(cfd_class, command);
            break;
        case DELETEFRIEND:
            DeleteFriend(cfd_class, command);
            break;
        case RESTOREFRIEND:
            Restorefriend(cfd_class, command);
            break;
        case NEWMESSAGE:
            NewMessage(cfd_class, command);
            break;
        case LOOKSYSTEM:
            LookSystem(cfd_class, command);
            break;
        case LOOKNOTICE:
            LookNotice(cfd_class, command);
            break;
        case REFUSEADDFRIEND:
            RefuseAddFriend(cfd_class, command);
            break;
        case CREATEGROUP:
            CreateGroup(cfd_class, command);
            break;
        case LISTGROUP:
            ListGroup(cfd_class, command);
            break;
        case ABOUTGROUP :
            AboutGroup(cfd_class, command);
            break;
        case REQUSTLIST :
            RequestList(cfd_class, command);
            break;
        case PASSAPPLY :
            PassApply(cfd_class, command);
            break;
        case DENYAPPLY :
            DenyApply(cfd_class, command);
            break;
        case SETMEMBER :
            SetMember(cfd_class, command);
            break;
        case EXITGROUP :
            ExitGroup(cfd_class, command);
            break;
        case DISPLAYMEMBER :
            DisplyMember(cfd_class, command);
            break;
        case REMOVEMEMBER :
            RemoveMember(cfd_class, command);
            break;
        case INFOXXXX :
            InfoXXXX(cfd_class, command);
            break;
        case SENDFILE :
            SendFile(cfd_class, command);
            break;
        case RECVFILE :
            RecvFile(cfd_class, command);
            break;
        }
}
void Login(TcpSocket cfd_class, Command command){
    // 从数据库调取对应数据进行核对，并回复结果
    if(!redis.sismember("用户uid集合", command.m_uid)){  // 如果没有账号，返回错误
        cfd_class.sendMsg("incorrect");
    }else{         // 否则账号存在，通过uid找到这个用户的哈希表，进行密码匹配和登录工作
        string pwd = redis.gethash(command.m_uid, "密码");
        if(pwd != command.m_option[0]){   // 密码错误
            cfd_class.sendMsg("incorrect");
        }else if(redis.gethash(command.m_uid, "在线状态") != "-1"){  // 用户在登录
            cfd_class.sendMsg("online");
        }
        else{            // 可以登录，并改变登录状态 
            redis.hsetValue(command.m_uid, "在线状态", to_string(cfd_class.getfd()));
            redis.hsetValue("fd-uid对应表", to_string(cfd_class.getfd()), command.m_uid);
            redis.hsetValue(command.m_uid, "聊天对象", "0");
            redis.hsetValue(command.m_uid, "通知套接字", "-1");
            cfd_class.sendMsg("ok");
            cout << "用户" << command.m_uid << "登录成功" << endl; 
        }
    }
    return;
}
void Register(TcpSocket cfd_class, Command command){
    srand((unsigned)time(NULL));
    while(true){                                                   
        string new_uid = to_string((rand()+1111)%10000);
        if(redis.sismember("用户uid集合", new_uid)){  // 直到随机一个未注册  uid发过去，并建立账号基本信息
            continue;
        }else if(stoi(new_uid) < 1000){
            continue;
        }else{
            redis.saddvalue("用户uid集合", new_uid);
            redis.hsetValue(new_uid, "账号", new_uid);
            redis.hsetValue(new_uid, "密码", command.m_option[0]);
            redis.hsetValue(new_uid, "昵称", new_uid);
            redis.hsetValue(new_uid, "在线状态", "-1");
            redis.hsetValue(new_uid, "性别", "未知");
            redis.hsetValue(new_uid, "其他信息", "无");
            redis.hsetValue(new_uid, "通知套接字", "-1");
            redis.hsetValue(new_uid, "聊天对象", "无");
            redis.hsetValue(new_uid + "的未读消息", "系统消息", "0");
            redis.hsetValue(new_uid + "的未读消息", "通知消息", "0");
            cfd_class.sendMsg(new_uid);
            cout << "用户" << new_uid << "注册成功" << endl; 
            return;
        }
    }
}
void AddFriend(TcpSocket cfd_class, Command command){
    // 账号不存在就通知客户端并返回
    if(!redis.sismember("用户uid集合", command.m_option[0])){
        cfd_class.sendMsg("nofind");
        return;
    }
    // 判断账号是否已经是自己的好友                                                    
    int friendNum = redis.hlen(command.m_uid + "的好友列表");    // 获得好友数量
    if(friendNum != 0){                                                     // 好友数量不为0时，列表中可能已经有准好友，为0时肯定没有，跳过此步骤
        redisReply** f_uid =  redis.hkeys(command.m_uid + "的好友列表"); // 得到好友列表
        for(int i = 0; i < friendNum; i++){                                 // 遍历好友列表，看看账号是否在里面
            if(f_uid[i]->str == command.m_option[0]){                       // 账号已在列表中就告诉客户端并停止添加好友
                cfd_class.sendMsg("had");
                return;
            }
        }
    }
    // 自己的系统消息里如果有对方发来的未处理的申请，就不能发送申请，通知客户端
    if(redis.hashexists(command.m_uid + "的系统消息", command.m_option[0])){
        string msg = redis.gethash(command.m_uid + "的系统消息", command.m_option[0]);
        string pass(msg.end() - 11,msg.end());
        if(pass == "(未处理)"){
            cfd_class.sendMsg("cannot1");
            return;
        }
    }
    // 对方的系统消息里如果自己发过去的未处理的申请，就不能发送申请，通知客户端
    if(redis.hashexists(command.m_option[0] + "的系统消息", command.m_uid)){
        string msg = redis.gethash(command.m_option[0] + "的系统消息", command.m_uid);
        string pass(msg.end() - 11,msg.end());
        if(pass == "(未处理)"){
            cfd_class.sendMsg("cannot2");
            return;
        }
    }

    // 在账户的系统消息里加入/更新自己的申请消息，只会存在一条由自己发来的好友申请。
    string wait = "(未处理)";
    string apply = "来自" + command.m_uid + "的好友申请：" + command.m_option[1] + GetNowTime() + wait;
    redis.hsetValue(command.m_option[0] + "的系统消息", command.m_uid, apply);
    // 被申请者未读消息中的系统消息数量+1
    string num = redis.gethash(command.m_option[0] + "的未读消息", "系统消息");
    redis.hsetValue(command.m_option[0] + "的未读消息", "系统消息", to_string(stoi(num)+1));
    // 如果准好友在线，给他的通知套接字一个提醒
    string online = redis.gethash(command.m_option[0], "在线状态");
    if(online != "-1"){
        string friend_recvfd = redis.gethash(command.m_option[0], "通知套接字");
        TcpSocket friendFd_class(stoi(friend_recvfd));
        friendFd_class.sendMsg("收到一条好友申请." + GetNowTime());
    }
    cfd_class.sendMsg("ok");
    
}
void AddGroup(TcpSocket cfd_class, Command command){
    // 群聊不存在，通知客户端
    if(!redis.sismember("群聊集合", command.m_option[0])){   
        cfd_class.sendMsg("nofind");
        return;
    }
    // 群聊存在,判断用户是否已在该群聊里                                                    
    int GroupNum = redis.hlen(command.m_uid + "的群聊列表"); 
    if(GroupNum != 0){                                                       // 群聊数量不为0时，列表中可能已经有该群聊,为0时可定没有，跳过此步骤
        redisReply** g_uid =  redis.hkeys(command.m_uid + "的群聊列表");  // 得到群聊列表
        for(int i = 0; i < GroupNum; i++){                                   // 遍历群聊列表，看看该群聊是否在里面
            if(g_uid[i]->str == command.m_option[0]){                        // 该群聊已在列表中就告诉客户端并停止申请添加群聊
                cfd_class.sendMsg("had");
                return;
            }
        }
    }
    // 用户不在群聊里，就查看该群聊的申请队列里是否已有该用户未处理的申请消息
    if(redis.hashexists(command.m_option[0] + "的申请列表", command.m_uid)){
        string msg = redis.gethash(command.m_option[0] + "的申请列表", command.m_uid);
        string pass(msg.end() - 11,msg.end());
        if(pass == "(未处理)"){
            cfd_class.sendMsg("cannot");
            return;
        }
    }
    // 没有未处理的申请就添加申请进该群聊的申请队列
    string wait = "(未处理)";
    string apply = "来自" + command.m_uid + "的入群申请：" + command.m_option[1] + GetNowTime() + wait;
    redis.hsetValue(command.m_option[0] + "的申请列表", command.m_uid, apply);
    // 该群聊管理员和群主的未读消息中的通知消息数量 + 1
    int num = redis.hlen(command.m_option[0] + "的群成员列表");
    redisReply** members = redis.hkeys(command.m_option[0] + "的群成员列表");
    for(int i = 0; i < num; i++){
        cout << "0" << endl;
        string position = redis.gethash(command.m_option[0] + "的群成员列表", members[i]->str);
        if(position == "管理员" || position == "群主"){
            string num = redis.gethash( members[i]->str + static_cast<string>("的未读消息"), "通知消息");
            redis.hsetValue( members[i]->str + string("的未读消息"), "通知消息", to_string(stoi(num)+1));
            redis.lpush(members[i]->str + static_cast<string>("的通知消息"), 
                     "您管理的群聊" + command.m_option[0] + "收到用户" + command.m_uid + "的入群申请." + GetNowTime());
            // 如果群主或者管理员在线，给他的通知套接字一个提醒
            string online = redis.gethash(members[i]->str, "在线状态");
            if(online != "-1"){
                string friend_recvfd = redis.gethash(members[i]->str, "通知套接字");
                TcpSocket friendFd_class(stoi(friend_recvfd));
                friendFd_class.sendMsg("您管理的群" + command.m_option[0] + "收到一条入群申请.");
            }
        }
    }
    cfd_class.sendMsg("ok");
}
void AgreeAddFriend(TcpSocket cfd_class, Command command){
    // 看看自己的好友列表里是否已有该好友，没有就可以同意申请，有就不可以同意申请，回复had
    if( redis.hlen(command.m_uid + "的好友列表") != 0 || !redis.hashexists(command.m_uid + "的好友列表", command.m_option[0])){
        // 系统消息列表里有没有他的申请
        if(!redis.hashexists(command.m_uid + "的系统消息", command.m_option[0])){
            cfd_class.sendMsg("nofind");
            return;
        }
        // 如果系统消息列表里有申请者的申请,但是已经处理过了，就不能在处理,只能处理申请状态为未处理的申请
        string msg = redis.gethash(command.m_uid + "的系统消息", command.m_option[0]);
        string state(msg.end() - 11,msg.end());
        if(state == "(已拒绝)" || state == "(已通过)"){
            cfd_class.sendMsg("haddeal");
            return;
        }
        // 更改自己这边的申请者的申请状态为已通过
        string pass = "(已通过)";
        string newmsg(msg.begin(),msg.end() - 11);
        string Newmsg = newmsg + pass;
        redis.hsetValue(command.m_uid + "的系统消息", command.m_option[0], Newmsg);
        // 同意者的系统消息数量-1
        string nownum = redis.gethash(command.m_uid + "的未读消息", "系统消息");
        redis.hsetValue(command.m_uid + "的未读消息", "系统消息", (to_string(stoi(nownum) - 1)));
        // 同意者的信息完善
        string friend_mark0  = redis.gethash(command.m_option[0], "昵称");                         // 获得申请者的昵称作为默认备注
        redis.hsetValue(command.m_uid + "的好友列表", command.m_option[0], friend_mark0);    // 在同意者好友列表里插入申请者的uid和默认备注
        redis.lpush(command.m_uid + "--" + command.m_option[0], "*********************");         // 为同意者建一个聊天会话
        // 申请者的信息完善
        string friend_mark1 = redis.gethash(command.m_uid, "昵称");                                    // 获得同意者的昵称
        redis.hsetValue(command.m_option[0] + "的好友列表", command.m_uid, friend_mark1);        // 在申请者的好友列表里插入申请者的uid和默认备注
        redis.lpush(command.m_option[0] + "--" + command.m_uid, "*********************");             // 为申请者建一个聊天会话
        // 在申请者的通知消息里写入通过消息
        redis.lpush(command.m_option[0] + "的通知消息", command.m_uid + "通过了您的好友申请." + GetNowTime());
        // 申请者未读消息中的通知消息数量+1
        string num1 = redis.gethash(command.m_option[0] + "的未读消息", "通知消息");
        redis.hsetValue(command.m_option[0] + "的未读消息", "通知消息", to_string(stoi(num1)+1));
        // 如果申请者在线，给他的通知套接字一个提醒
        string online = redis.gethash(command.m_option[0], "在线状态");
        if(online != "-1"){
            string friend_recvfd = redis.gethash(command.m_option[0], "通知套接字");
            TcpSocket friendFd_class(stoi(friend_recvfd));
            friendFd_class.sendMsg(command.m_uid + "通过了您的好友申请.");
        }
    }else{
        cfd_class.sendMsg("had");
        return;
    }
    cfd_class.sendMsg("ok");
    return;
}
void ListFriend(TcpSocket cfd_class, Command command){
    int friendNum = redis.hlen(command.m_uid + "的好友列表");     // 获得好友列表的好友数量
    if(friendNum == 0){
        cfd_class.sendMsg("none");
    }else {
        // 好友数量不为0，就遍历好友列表，根据在线状态发送要展示的内容                                                       
        redisReply **f_uid = redis.hkeys(command.m_uid + "的好友列表");
        for(int i = 0; i < friendNum; i++){
            string friend_mark = redis.gethash(command.m_uid + "的好友列表", f_uid[i]->str);
            string isonline = redis.gethash(f_uid[i]->str, "在线状态");
            if(!redis.sismember(command.m_uid + "的屏蔽列表", f_uid[i]->str)){
                if(isonline != "-1"){
                    cfd_class.sendMsg(L_GREEN + friend_mark + NONE + "(" + f_uid[i]->str + ")");
                }else {
                    cfd_class.sendMsg(L_WHITE + friend_mark + NONE + "(" + f_uid[i]->str + ")");
                }
            }
        }
        cfd_class.sendMsg("end");
    }
}
void ChatFriend(TcpSocket cfd_class, Command command){
    // 好友数量是否为0
    if(redis.hlen(command.m_uid + "的好友列表") == 0){
        cfd_class.sendMsg("none");
        return;
    }
    // 好友列表列是否有这个好友
    if(!redis.hashexists(command.m_uid + "的好友列表", command.m_option[0])){
        cfd_class.sendMsg("nofind");
    }else{
        cfd_class.sendMsg("have");
        // 好友列表里有这个人就发送历史聊天记录，并把客户端的的聊天对象改为该好友
        int HistoryMsgNum = redis.llen(command.m_uid + "--" + command.m_option[0]);
        redisReply** MsgHistory = redis.lrange(command.m_uid + "--" + command.m_option[0]);
        for(int i = HistoryMsgNum - 1; i >= 0; i--){
            cfd_class.sendMsg(MsgHistory[i]->str);
        }
        redis.hsetValue(command.m_uid, "聊天对象", command.m_option[0]);
        // 将我的未读消息列表里来自好友的未读消息数量清零
        if(redis.hashexists(command.m_uid + "的未读消息", "来自" + command.m_option[0] + "的未读消息")){
            redis.hsetValue(command.m_uid + "的未读消息", "来自" + command.m_option[0] + "的未读消息", "0");
        }
        cfd_class.sendMsg("以上为历史聊天记录");
    }
    return;
}
void ChatGroup(TcpSocket cfd_class, Command command){
     // 群聊数量是否为0
    if(redis.hlen(command.m_uid + "的群聊列表") == 0){
        cfd_class.sendMsg("none");
        return;
    }
    // 群聊列表列是否有这个群聊
    if(!redis.hashexists(command.m_uid + "的群聊列表", command.m_option[0])){
        cfd_class.sendMsg("nofind");
    }else{
        cfd_class.sendMsg("have");
        // 群聊列表里有这个人就发送历史聊天记录，并把客户端的的聊天对象改为该群聊
        int HistoryMsgNum = redis.llen(command.m_option[0] + "的聊天消息队列");
        redisReply** MsgHistory = redis.lrange(command.m_option[0] + "的聊天消息队列");
        for(int i = HistoryMsgNum - 1; i >= 0; i--){
            if(static_cast<string>(MsgHistory[i]->str) == "begin"){
                continue;
            }else{
                string msg(MsgHistory[i]->str);
                string end(msg, msg.find("：") + 3);
                string sender_uid(msg, 0, msg.find("："));
                if(sender_uid == command.m_uid){
                    cfd_class.sendMsg("我：" + end);
                }else{
                    string name = redis.gethash(sender_uid, "昵称");
                    cfd_class.sendMsg(name + "：" + end);
                }
            }
        }
        redis.hsetValue(command.m_uid, "聊天对象", command.m_option[0]);
        // 将我的未读消息列表里来自好友的未读消息数量清零
        if(redis.hashexists(command.m_uid + "的未读消息", "来自" + command.m_option[0] + "的未读消息")){
            redis.hsetValue(command.m_uid + "的未读消息", "来自" + command.m_option[0] + "的未读消息", "0");
        }
        cfd_class.sendMsg("以上为历史聊天记录");
    }
    return;
}
void FriendMsg(TcpSocket cfd_class, Command command){
    // 是否存在该好友
    if(!redis.hashexists(command.m_uid + "的好友列表", command.m_option[0])){
        cfd_class.sendMsg("nohave");
        return;
    }
    // 将新的消息加入到我对他的消息队列
    string msg0 = "我：" + command.m_option[1] + ".........." + GetNowTime();
    redis.lpush(command.m_uid + "--" + command.m_option[0], msg0);
    // 当前聊天界面展示我的消息
    string my_recvfd = redis.gethash(command.m_uid, "通知套接字");
    TcpSocket myFd_class(stoi(my_recvfd));
    myFd_class.sendMsg(UP + msg0);
    // 如果好友把自己屏蔽的话，什么都不做，直接返回
    if(redis.scard(command.m_option[0] + "的屏蔽列表")){
        if(redis.sismember(command.m_option[0] + "的屏蔽列表", command.m_uid)){
            return;
        }
    }
    // 没有被屏蔽，消息加到他对我的消息队列，并给相应通知
    string name0 = redis.gethash(command.m_option[0] + "的好友列表", command.m_uid);  // 得到好友給我的备注
    string msg1 = name0 + "：" + command.m_option[1] + ".........." + GetNowTime();   
    redis.lpush(command.m_option[0] + "--" + command.m_uid, msg1);  // 把消息加入的好友的聊天队列

    // 如果好友把自己屏蔽的话，啥都不做，如果没有被屏蔽，就进行下面的操作：
    // 如果好友在线且处于和自己的聊天界面，就把消息内容发给通知套接字
    // 否则，把消息添加到未读消息里
    // 如果好友在线但不处于和自己的聊天界面，把提示消息发给通知套接字
    string online = redis.gethash(command.m_option[0], "在线状态");
    string ChatFriend = redis.gethash(command.m_option[0], "聊天对象");
    // 好友在线且和我聊天，让通知套接字展示消息，并返回
    if(online != "-1" && ChatFriend == command.m_uid){   // 好友在线且和我聊天
        string friend_recvfd = redis.gethash(command.m_option[0], "通知套接字");
        TcpSocket friendFd_class(stoi(friend_recvfd));
        string begin = "\r\n";
        friendFd_class.sendMsg(begin + UP + msg1);
    }
    else{// 否则，好友的未读消息中的来自我的消息数量+1
        if(redis.hashexists(command.m_option[0] + "的未读消息", "来自" + command.m_uid + "的未读消息")){
            string num1 = redis.gethash(command.m_option[0] + "的未读消息", "来自" + command.m_uid + "的未读消息");
            redis.hsetValue(command.m_option[0] + "的未读消息", "来自" + command.m_uid + "的未读消息", to_string(stoi(num1)+1));
        }else{
            redis.hsetValue(command.m_option[0] + "的未读消息", "来自" + command.m_uid + "的未读消息", "1");
        }
    }
    // 如果好友在线但是没和我聊天，让通知套接字告知来消息
    if(online != "-1" && ChatFriend != command.m_uid){  // 好友在线但没和我聊天
        string friend_recvfd = redis.gethash(command.m_option[0], "通知套接字");
        TcpSocket friendFd_class(stoi(friend_recvfd));
        friendFd_class.sendMsg(command.m_uid + "发来了一条消息");
    }
    cfd_class.sendMsg("ok");
    return;
}
void GroupMsg(TcpSocket cfd_class, Command command){
    // 是否存在该群聊
    if(!redis.hashexists(command.m_uid + "的群聊列表", command.m_option[0])){
        cfd_class.sendMsg("nohave");
        return;
    }
    // 将新的消息加入到群聊消息队列
    string msg0 = command.m_uid + "：" + command.m_option[1] + ".........." + GetNowTime();
    redis.lpush(command.m_option[0] + "的聊天消息队列", msg0);
    // 当前聊天界面展示我的消息
    string my_recvfd = redis.gethash(command.m_uid, "通知套接字");
    TcpSocket myFd_class(stoi(my_recvfd));
    string up = UP;
    myFd_class.sendMsg(up + "我：" + command.m_option[1] + ".........." + GetNowTime());
    // 如果群成员的聊天对象不是该群，未读消息数+1
    // 如果群成员的聊天对象不是该群，在线，给一个提示消息，不在线就不给
    // 如果群成员的聊天对象是该群，通知套接字展示消息内容
    int num = redis.hlen(command.m_option[0] + "的群成员列表");
    redisReply** members = redis.hkeys(command.m_option[0] + "的群成员列表");
    for(int i = 0; i < num; i++){
        if(members[i]->str != command.m_uid){
            string online = redis.gethash(members[i]->str, "在线状态");
            string Chatgroup = redis.gethash(members[i]->str, "聊天对象");
            // 群成员在线且和我聊天，让通知套接字展示消息，并返回
            if(online != "-1" && Chatgroup == command.m_option[0]){   // 群成员在线且和在群里聊天
                string member_recvfd = redis.gethash(members[i]->str, "通知套接字");
                TcpSocket friendFd_class(stoi(member_recvfd));
                string begin = "\r\n";
                friendFd_class.sendMsg(begin + UP + msg0);
            }
            else{// 否则，群成员的未读消息中的来自群聊的消息数量+1    
                if(redis.hashexists(static_cast<string>(members[i]->str) + "的未读消息", "来自" + command.m_option[0] + "的未读消息")){
                    string num1 = redis.gethash(static_cast<string>(members[i]->str) + "的未读消息", "来自" + command.m_option[0] + "的未读消息");
                    redis.hsetValue(static_cast<string>(members[i]->str) + "的未读消息", 
                                    "来自" + command.m_option[0] + "的未读消息", to_string(stoi(num1)+1));                   
                }else{
                    redis.hsetValue(static_cast<string>(members[i]->str) + "的未读消息", "来自" + command.m_option[0] + "的未读消息", "1");
                }
            }
            // 如果群成员在线但是没和我聊天，让通知套接字告知来消息
            if(online != "-1" && Chatgroup != command.m_option[0]){  // 群成员在线但没在群里聊天
                string member_recvfd = redis.gethash(members[i]->str, "通知套接字");
                TcpSocket friendFd_class(stoi(member_recvfd));
                friendFd_class.sendMsg(command.m_option[0] + "发来了一条消息");
            }
        }
    }
    cfd_class.sendMsg("ok");
    return;
}
void ExitChatFriend(TcpSocket cfd_class, Command command){
    if (redis.gethash(command.m_uid, "聊天对象") == "0"){
        cfd_class.sendMsg("no");
        return;
    }else{
        redis.hsetValue(command.m_uid, "聊天对象", "0");
        cfd_class.sendMsg("ok");
        return;
    }
}
void ExitChatGroup(TcpSocket cfd_class, Command command){
    if (redis.gethash(command.m_uid, "聊天对象") == "0"){
        cfd_class.sendMsg("no");
        return;
    }else{
        redis.hsetValue(command.m_uid, "聊天对象", "0");
        cfd_class.sendMsg("ok");
        return;
    }
}
void ShieldFriend(TcpSocket cfd_class, Command command){
    // 不存在该好友就通知客户端并返回
    if(!redis.hashexists(command.m_uid + "的好友列表", command.m_option[0])){  
        cfd_class.sendMsg("no");
        return;
    }
    // 如果之前没有好友被屏蔽，就直接屏蔽该好友，并返回
    int shieldNum = redis.scard(command.m_uid + "屏蔽列表");   
    if(shieldNum == 0){
        redis.saddvalue(command.m_uid + "的屏蔽列表", command.m_option[0]);
        cfd_class.sendMsg("ok");
        return;
    }
    // 如果屏蔽列表里已经有了该好友，通知客户端并返回
    redisReply** shield_list = redis.smembers(command.m_uid + "的屏蔽列表");    
    for(int i = 0; i < shieldNum; i++){                            
        if(shield_list[i]->str == command.m_option[0]){
            cfd_class.sendMsg("had");
            return;
        }
    }
    // 该好友没有被屏蔽，就屏蔽
    redis.saddvalue(command.m_uid + "的屏蔽列表", command.m_option[0]);
    cfd_class.sendMsg("ok");
    return;
}
void DeleteFriend(TcpSocket cfd_class, Command command){
    if(!redis.hashexists(command.m_uid + "的好友列表", command.m_option[0])){// 好友列表里没有这个人，直接返回
        cfd_class.sendMsg("nofind");
        return;
    }
    
    // 双方好友列表删除对方
    redis.delhash(command.m_uid + "的好友列表", command.m_option[0]);
    redis.delhash(command.m_option[0] + "的好友列表", command.m_uid);
    // 被删者未读消息中的通知消息数量+1
    string num1 = redis.gethash(command.m_option[0] + "的未读消息", "通知消息");
    redis.hsetValue(command.m_option[0] + "的未读消息", "通知消息", to_string(stoi(num1)+1));
    // 通知消息里告诉被删者
    redis.lpush(command.m_uid + "的通知消息", command.m_uid + "解除了和您的好友关系" + GetNowTime());
    // 如果被删者在线，给他的通知套接字一个提醒
    string online = redis.gethash(command.m_option[0], "在线状态");
    if(online != "-1"){
        string friend_recvfd = redis.gethash(command.m_option[0], "通知套接字");
        TcpSocket friendFd_class(stoi(friend_recvfd));
        friendFd_class.sendMsg(command.m_uid + "解除了和您的好友关系");
    }
    // 我的屏蔽列表里是否有该好友，有就删掉，没有就不管
    int shieldNum = redis.scard(command.m_uid + "的屏蔽列表");            // 是否有好友被屏蔽
    if(shieldNum != 0){
        redisReply** shield_list = redis.smembers(command.m_uid + "的屏蔽列表");        // 该好友是否被屏蔽
        for(int i = 0; i < shieldNum; i++){                            
            if(shield_list[i]->str == command.m_option[0]){                            // 被屏蔽就解屏蔽
                redis.sremvalue(command.m_uid + "的屏蔽列表", command.m_option[0]);
                break;
            }
        }
    }
    // 对方的屏蔽列表里是否有该好友，有就删掉，没有就不管
    int shieldNum1 = redis.scard(command.m_uid + "的屏蔽列表");            // 是否有好友被屏蔽
    if(shieldNum1 != 0){
        redisReply** shield_list1 = redis.smembers(command.m_uid + "的屏蔽列表");        // 该好友是否被屏蔽
        for(int i = 0; i < shieldNum1; i++){                            
            if(shield_list1[i]->str == command.m_option[0]){                            // 被屏蔽就解屏蔽
                redis.sremvalue(command.m_uid + "的屏蔽列表", command.m_option[0]);
                break;
            }
        }
    }
    // 删除历史聊天记录
    redis.delKey(command.m_uid + "--" + command.m_option[0]);
    redis.delKey(command.m_option[0] + "--" + command.m_uid);
    cfd_class.sendMsg("ok");
    return;
}
void Restorefriend(TcpSocket cfd_class, Command command){
    if(!redis.hashexists(command.m_uid + "的好友列表", command.m_option[0])){   // 是否有该好友
        cfd_class.sendMsg("nohave");
        return;
    }
    // 是否有好友被屏蔽
    int shieldNum = redis.scard(command.m_uid + "的屏蔽列表");
    if(shieldNum == 0){
        cfd_class.sendMsg("nofind");
        return;
    }
    redisReply** shield_list = redis.smembers(command.m_uid + "的屏蔽列表");        // 该好友是否被屏蔽
    for(int i = 0; i < shieldNum; i++){                            
        if(shield_list[i]->str == command.m_option[0]){                            // 被屏蔽就解屏蔽
            redis.sremvalue(command.m_uid + "的屏蔽列表", command.m_option[0]);
            cfd_class.sendMsg("ok");
            return;
        }
    }
    // 没有别屏蔽就告诉客户端
    cfd_class.sendMsg("nofind");
    return;
}
void NewMessage(TcpSocket cfd_class, Command command){
    int NewNum = redis.hlen(command.m_uid + "的未读消息");
        redisReply **NewList = redis.hkeys(command.m_uid + "的未读消息");
        for(int i = 0;i < NewNum; i++){
            string oneNum = redis.gethash(command.m_uid + "的未读消息", NewList[i]->str);
            string oneline = static_cast<string>(NewList[i]->str) + "：" + oneNum;
            cfd_class.sendMsg(oneline);
        }
        cfd_class.sendMsg("end");
}
void LookSystem(TcpSocket cfd_class, Command command){
    int num = redis.hlen(command.m_uid + "的系统消息");
    if(num == 0){
        cfd_class.sendMsg("none");
        return;
    }
    redisReply **SysMsgList = redis.hkeys(command.m_uid + "的系统消息");
    for(int i = num - 1; i >= 0; i--){
        string sysmsg = redis.gethash(command.m_uid + "的系统消息", SysMsgList[i]->str);
        cfd_class.sendMsg(sysmsg);
    }
    cfd_class.sendMsg("end");
    return;
}
void LookNotice(TcpSocket cfd_class, Command command){
    redis.hsetValue(command.m_uid + "的未读消息", "通知消息", "0");
    int num = redis.llen(command.m_uid + "的通知消息");
    if(num == 0){
        cfd_class.sendMsg("none");
        return;
    }
    redisReply **NoticList = redis.lrange(command.m_uid + "的通知消息");
    for(int i = num - 1; i >= 0; i--){
        cfd_class.sendMsg(NoticList[i]->str);
    }
    cfd_class.sendMsg("end");
    return;
}
void RefuseAddFriend(TcpSocket cfd_class, Command command){
    // 看看自己的好友列表里是否已有该好友，没有就可以修改申请，有就不可以修改申请，回复had
    if( redis.hlen(command.m_uid + "的好友列表") == 0 || !redis.hashexists(command.m_uid + "的好友列表", command.m_option[0])){
        // 系统消息列表里有没有他的申请
        if(!redis.hashexists(command.m_uid + "的系统消息", command.m_option[0])){
            cfd_class.sendMsg("nofind");
            return;
        }
        // 如果系统消息列表里有申请者的申请,但是已经处理过了，就不能在处理
        string msg = redis.gethash(command.m_uid + "的系统消息", command.m_option[0]);
        string newmsg(msg.begin(), msg.end() - 11);
        if(newmsg == "(已拒绝)" || newmsg == "(已通过)"){
            cfd_class.sendMsg("haddeal");
            return;
        }
        // 更改申请者收到的申请消息状态为已拒绝
        string pass = "(已拒绝)";
        string Newmsg = newmsg + pass;
        redis.hsetValue(command.m_uid + "的系统消息", command.m_option[0], Newmsg);
        // 拒绝者的系统消息数量-1
        string nownum = redis.gethash(command.m_uid + "的未读消息", "系统消息");
        redis.hsetValue(command.m_uid + "的未读消息", "系统消息", (to_string(stoi(nownum) - 1)));
        // 在申请者的通知消息里写入未通过消息
        redis.lpush(command.m_option[0] + "的通知消息", command.m_uid + "拒绝了您的好友申请." + GetNowTime());
        // 申请者未读消息中的通知消息数量+1
        string num1 = redis.gethash(command.m_option[0] + "的未读消息", "通知消息");
        redis.hsetValue(command.m_option[0] + "的未读消息", "通知消息", to_string(stoi(num1)+1));
        // 如果申请者在线，给他的通知套接字一个提醒
        string online = redis.gethash(command.m_option[0], "在线状态");
        if(online != "-1"){
            string friend_recvfd = redis.gethash(command.m_option[0], "通知套接字");
            TcpSocket friendFd_class(stoi(friend_recvfd));
            string kaitou = UP;
            friendFd_class.sendMsg(kaitou + "\r" + command.m_uid + "拒绝了您的好友申请.");
        }
    }else{
        cfd_class.sendMsg("had");
        return;
    }
    cfd_class.sendMsg("ok");
    return;
}
void CreateGroup(TcpSocket cfd_class, Command command){
    // 检查发过来的uid是否都是用户的好友，有一个不是就返回并提醒客户端,都是就加入到一个vector里，作为初始群成员
    int len = command.m_option[0].size();
    vector<string> members;
    if(len < 4 ){
        cfd_class.sendMsg("nofind" + command.m_option[0]);
        return;
    }
    for(int i = 0; i + 4 <= len; i += 5){
        string member(command.m_option[0].begin() + i, command.m_option[0].begin() + i + 4);
        if(!redis.hashexists(command.m_uid + "的好友列表", member)){
            cfd_class.sendMsg("nofind" + member);
            return;
        }
        members.push_back(member);
    }
    // 给群随机一个群号，并建立群的基本信息
    srand((unsigned)time(NULL));
    while(true){                                                   
        string new_gid = to_string((rand()+111)%1000);
        if(redis.sismember("群聊集合", new_gid)){  // 直到随机一个未注册的gid发过去，并建立群聊基本信息
            continue;
        }else if(stoi(new_gid) < 100){
            continue;
        }else{
            redis.saddvalue("群聊集合", new_gid);
            redis.hsetValue(new_gid + "的基本信息", "群号", new_gid);
            redis.hsetValue(new_gid + "的基本信息", "群名", new_gid);
            redis.hsetValue(new_gid + "的基本信息", "创建时间", GetNowTime());
            redis.hsetValue(new_gid + "的基本信息", "群介绍", "暂无");
            redis.hsetValue(new_gid + "的基本信息", "群公告", "暂无");
            // 为群主建立群聊基本要素：群成员列表里的身份，自己的群聊里加上这个群，自己和群聊的消息队列加结尾
            redis.hsetValue(new_gid + "的群成员列表", command.m_uid, "群主");
            redis.hsetValue(command.m_uid + "的群聊列表", new_gid, new_gid);
            redis.lpush(new_gid + "的聊天消息队列", "begin");
            // 为初始群成员建立群聊基本要素：群成员列表里的身份，自己的群聊里加上这个群，自己和群聊的消息队列加结尾
            for(auto member: members){
                // 在初始成员的系统消息里写入加入通知
                redis.lpush(member + "的通知消息", "您作为" + command.m_uid + "创建的群聊" + new_gid + "的初始群成员加入了该群聊." + GetNowTime());
                // 初始成员的未读消息中的通知消息数量+1
                string num = redis.gethash(member + "的未读消息", "通知消息");
                redis.hsetValue(member + "的未读消息", "通知消息", to_string(stoi(num)+1));
                redis.hsetValue(new_gid + "的群成员列表", member, "群成员");
                redis.hsetValue(member + "的群聊列表", new_gid, new_gid);
                string online = redis.gethash(member, "在线状态");
                if(online != "-1"){
                    string friend_recvfd = redis.gethash(member, "通知套接字");
                    TcpSocket friendFd_class(stoi(friend_recvfd));
                    friendFd_class.sendMsg("您被您的好友拉入了一个群聊.");
                }
            }
            cfd_class.sendMsg(new_gid);
            return;
        }
    }
}
void ListGroup(TcpSocket cfd_class, Command command){
    int GroupNum = redis.hlen(command.m_uid + "的群聊列表");
    if(GroupNum == 0){
        cfd_class.sendMsg("none");
    }else {
        // 群聊数量不为0，就遍历群聊列表，根据在线状态发送要展示的内容                                                       
        redisReply **g_uid = redis.hkeys(command.m_uid + "的群聊列表");
        for(int i = 0; i < GroupNum; i++){
            string group_mark = redis.gethash(command.m_uid + "的群聊列表", g_uid[i]->str);
                cfd_class.sendMsg(L_GREEN + group_mark + NONE + "(" + g_uid[i]->str + ")");
        }
        cfd_class.sendMsg("end");
    }
}
void AboutGroup(TcpSocket cfd_class, Command command){
    // 判断群聊是否存在
    if(!redis.sismember("群聊集合", command.m_option[0])){
        cfd_class.sendMsg("nohave");
        return;
    }
    // 判断用户是否已在该群聊里                                                    
    int GroupNum = redis.hlen(command.m_uid + "的群聊列表"); 
    if(GroupNum != 0){                                                       // 群聊数量不为0时，列表中可能已经有准好友,为0时可定没有，跳过此步骤
        redisReply** g_uid =  redis.hkeys(command.m_uid + "的群聊列表");  // 得到群聊列表
        for(int i = 0; i < GroupNum; i++){                                   // 遍历群聊列表，看看该群聊是否在里面
            if(g_uid[i]->str == command.m_option[0]){                        // 该群聊已在列表中就告诉客户端，可以进行接下来操作
                cfd_class.sendMsg("ok");
                return;
            }
        }
    }
    cfd_class.sendMsg("no");// 遍历群聊列表没找到该群聊，说明用户不在里面，告诉用户返回
    return;
}
void RequestList(TcpSocket cfd_class, Command command){
    // 判断查看的人是否为管理员或群主
    string position = redis.gethash(command.m_option[0] + "的群成员列表", command.m_uid);
    if(position == "群成员"){
        cfd_class.sendMsg("cannot");
        return;
    }
    // 获得申请列表里申请的数量
    int len = redis.hlen(command.m_option[0] + "的申请列表");
    if(len == 0 ){
        cfd_class.sendMsg("none");
    }else{
        redisReply** applicants = redis.hkeys(command.m_option[0] + "的申请列表");
        for(int i = 0; i < len; i++){
            string apply = redis.gethash(command.m_option[0] + "的申请列表", applicants[i]->str);
            cfd_class.sendMsg(apply);
        }
        cfd_class.sendMsg("end");
    }
    return;
}
void PassApply(TcpSocket cfd_class, Command command){
    // 是否为群主或者管理员
    string position = redis.gethash(command.m_option[0] + "的群成员列表", command.m_uid);
    if(position == "群成员"){
        cfd_class.sendMsg("cannot");
        return;
    }
    // 被操作人是否在群里
    if(redis.hashexists(command.m_option[0] + "的群成员列表", command.m_option[1])){
        cfd_class.sendMsg("had");
        return;
    }
    // 申请列表里有没有这个人的申请
    if(!redis.hashexists(command.m_option[0] + "的申请列表", command.m_option[1])){
        cfd_class.sendMsg("nofind");
        return;
    }
    // 如果申请列表里有申请者的申请,但是已经处理过了，就不能在处理,只能处理申请状态为未处理的申请
    string apply_old = redis.gethash(command.m_option[0] + "的申请列表", command.m_option[1]);
    string state(apply_old.end() - 11, apply_old.end());
    if(state == "(已拒绝)" || state == "(已通过)"){
        cfd_class.sendMsg("haddeal");
        return;
    }
    // 获取这个人进群之前的群成员列表
    int num = redis.hlen(command.m_option[0] + "的群成员列表");
    redisReply** members = redis.hkeys(command.m_option[0] + "的群成员列表");
    // 群成员列表里加他
    redis.hsetValue(command.m_option[0] + "的群成员列表", command.m_option[1], "群成员");
    // 在新成员的群聊列表里加上该群聊
    redis.hsetValue(command.m_option[1] + "的群聊列表", command.m_option[0], command.m_option[0]);
    // 更改申请消息为已通过
    string apply(apply_old.begin(), apply_old.end() - 11);
    string pass= "(已通过)";
    string apply_new = apply + pass;
    redis.hsetValue(command.m_option[0] + "的申请列表", command.m_option[1], apply_new);
    // 在申请者的通知消息里写入通过消息
    redis.lpush(command.m_option[1] + "的通知消息", "群聊" + command.m_option[0] + "通过了您的入群申请." + GetNowTime());
    // 申请者未读消息中的通知消息数量+1
    string num1 = redis.gethash(command.m_option[1] + "的未读消息", "通知消息");
    redis.hsetValue(command.m_option[1] + "的未读消息", "通知消息", to_string(stoi(num1) + 1));
    // 如果申请者在线，给他的通知套接字一个提醒
    string online = redis.gethash(command.m_option[1], "在线状态");
    if(online != "-1"){
        string friend_recvfd = redis.gethash(command.m_option[1], "通知套接字");
        TcpSocket friendFd_class(stoi(friend_recvfd));
        friendFd_class.sendMsg("群聊" + command.m_option[0] + "通过了您的入群申请.");
    }
    // 给这个人进群之前的群成员列表中的管理员和群主通知
    for(int i = 0; i < num; i++){
        string position = redis.gethash(command.m_option[0] + "的群成员列表", members[i]->str);
        if(position == static_cast<string>("管理员") || position == static_cast<string>("群主")){
            if(members[i]->str != command.m_uid){
                string num0 = redis.gethash( members[i]->str + static_cast<string>("的未读消息"), "通知消息");
                redis.hsetValue( members[i]->str + string("的未读消息"), "通知消息", to_string(stoi(num0)+1));
                redis.lpush(members[i]->str + static_cast<string>("的通知消息"), 
                        "您管理的群聊" + command.m_option[0] + "同意了用户" + command.m_option[1] + "的入群申请.处理人：" + command.m_uid + GetNowTime());
                // 如果群主或者管理员在线，给他的通知套接字一个提醒
                string online = redis.gethash(members[i]->str, "在线状态");
                if(online != "-1"){
                    string friend_recvfd = redis.gethash(members[i]->str, "通知套接字");
                    TcpSocket friendFd_class(stoi(friend_recvfd));
                    friendFd_class.sendMsg("您管理的群" + command.m_option[0] + "通过了一条入群申请.");
                }
            }
        }
    }
    cfd_class.sendMsg("ok");
    return;
}
void DenyApply(TcpSocket cfd_class, Command command){
    // 是否为群主或者管理员
    string position = redis.gethash(command.m_option[0] + "的群成员列表", command.m_uid);
    if(position == "群成员"){
        cfd_class.sendMsg("cannot");
        return;
    }
    // 被操作人是否在群里
    if(redis.hashexists(command.m_option[0] + "的群成员列表", command.m_option[1])){
        cfd_class.sendMsg("had");
        return;
    }
    // 申请列表里有没有这个人的申请
    if(!redis.hashexists(command.m_option[0] + "的申请列表", command.m_option[1])){
        cfd_class.sendMsg("nofind");
        return;
    }
    // 如果申请列表里有申请者的申请,但是已经处理过了，就不能在处理,只能处理申请状态为未处理的申请
    string apply_old = redis.gethash(command.m_option[0] + "的申请列表", command.m_option[1]);
    string state(apply_old.end() - 11, apply_old.end());
    if(state == "(已拒绝)" || state == "(已通过)"){
        cfd_class.sendMsg("haddeal");
        return;
    }
    // 获取这个人进群之前的群成员列表
    int num = redis.hlen(command.m_option[0] + "的群成员列表");
    redisReply** members = redis.hkeys(command.m_option[0] + "的群成员列表");
    // 更改申请消息为已通过
    string apply(apply_old.begin(), apply_old.end() - 11);
    string deny= "(已拒绝)";
    string apply_new = apply + deny;
    redis.hsetValue(command.m_option[0] + "的申请列表", command.m_option[1], apply_new);
    // 在申请者的通知消息里写入通过消息
    redis.lpush(command.m_option[0] + "的通知消息", "群聊" + command.m_option[1] + "拒绝了您的入群申请." + GetNowTime());
    // 申请者未读消息中的通知消息数量+1
    string num1 = redis.gethash(command.m_option[0] + "的未读消息", "通知消息");
    redis.hsetValue(command.m_option[0] + "的未读消息", "通知消息", to_string(stoi(num1) + 1));
    // 如果申请者在线，给他的通知套接字一个提醒
    string online = redis.gethash(command.m_option[0], "在线状态");
    if(online != "-1"){
        string friend_recvfd = redis.gethash(command.m_option[0], "通知套接字");
        TcpSocket friendFd_class(stoi(friend_recvfd));
        friendFd_class.sendMsg("群聊" + command.m_option[1] + "拒绝了您的入群申请.");
    }
     // 用这个人进群之前的群成员列表，该群聊管理员和群主的未读消息中的通知消息数量 + 1
    for(int i = 0; i < num; i++){
        string position = redis.gethash(command.m_option[0] + "的群成员列表", members[i]->str);
        if(position == static_cast<string>("管理员") || position == static_cast<string>("群主")){
            if(members[i]->str != command.m_uid){
                string num0 = redis.gethash( members[i]->str + static_cast<string>("的未读消息"), "通知消息");
                redis.hsetValue( members[i]->str + string("的未读消息"), "通知消息", to_string(stoi(num0)+1));
                redis.lpush(members[i]->str + static_cast<string>("的通知消息"), 
                        "您管理的群聊" + command.m_option[0] + "拒绝了用户" + command.m_option[1] + "的入群申请.处理人：" + command.m_uid + GetNowTime());
                // 如果群主或者管理员在线，给他的通知套接字一个提醒
                string online = redis.gethash(members[i]->str, "在线状态");
                if(online != "-1"){
                    string friend_recvfd = redis.gethash(members[i]->str, "通知套接字");
                    TcpSocket friendFd_class(stoi(friend_recvfd));
                    friendFd_class.sendMsg("您管理的群" + command.m_option[0] + "拒绝了一条入群申请.");
                }
            }
        }
    }
    cfd_class.sendMsg("ok");
}
void SetMember(TcpSocket cfd_class, Command command){
    // 操作人是否为群主
    string position = redis.gethash(command.m_option[0] + "的群成员列表", command.m_uid);
    if(position != "群主"){
        cfd_class.sendMsg("cannot");
        return;
    }
    // 被操作人是否在群里
    if(!redis.hashexists(command.m_option[0] + "的群成员列表", command.m_option[1])){
        cfd_class.sendMsg("nohave");
        return;
    }
    // 被操作人是否是群主
    string position_old = redis.gethash(command.m_option[0] + "的群成员列表", command.m_option[1]);
    if(position_old == "群主"){
        cfd_class.sendMsg("cannot1");
        return;
    }
    // 被操作人是否为已经为该职位
    if(position_old == command.m_option[2]){
        cfd_class.sendMsg("hadis");
        return;
    }
    // 是否是群主转让群
    if(command.m_option[2] == "leader"){
        // 把自己改成群成员
        redis.hsetValue(command.m_option[0] + "的群成员列表", command.m_uid, "群成员");
        // 设置新群主
        redis.hsetValue(command.m_option[0] + "的群成员列表", command.m_option[1], "群主");
        // 通知新群主
        string num = redis.gethash( command.m_option[1] + static_cast<string>("的未读消息"), "通知消息");
        redis.hsetValue( command.m_option[1] + string("的未读消息"), "通知消息", to_string(stoi(num)+1));
        redis.lpush(command.m_option[1] + static_cast<string>("的通知消息"), "您所在的群聊" + command.m_option[0] + "的群主" + 
                    command.m_uid + "把群聊转让给了你" + GetNowTime());
        // 如果新群主在线，给他的通知套接字一个提醒
        string online = redis.gethash(command.m_option[1], "在线状态");
        if(online != "-1"){
            string friend_recvfd = redis.gethash(command.m_option[1], "通知套接字");
            TcpSocket friendFd_class(stoi(friend_recvfd));
            friendFd_class.sendMsg("您成为了群聊" + command.m_option[0] + "的新群主.");
        }
        cfd_class.sendMsg("ok");
        return;
    }
    // 为被操作人设置新职位
    if(command.m_option[2] == "member"){
        redis.hsetValue(command.m_option[0] + "的群成员列表", command.m_option[1], "群成员");
        // 通知被操作人
        string num = redis.gethash( command.m_option[1] + static_cast<string>("的未读消息"), "通知消息");
        redis.hsetValue( command.m_option[1] + string("的未读消息"), "通知消息", to_string(stoi(num)+1));
        redis.lpush(command.m_option[1] + static_cast<string>("的通知消息"), "您所在的群聊" + command.m_option[0] + "的群主" + 
                    command.m_uid + "撤销了您的管理员权限" + GetNowTime());
        // 如果新群主在线，给他的通知套接字一个提醒
        string online = redis.gethash(command.m_option[1], "在线状态");
        if(online != "-1"){
            string friend_recvfd = redis.gethash(command.m_option[1], "通知套接字");
            TcpSocket friendFd_class(stoi(friend_recvfd));
            friendFd_class.sendMsg("您在群聊" + command.m_option[0] + "的管理员权限被撤销.");
        }
    }else if(command.m_option[2] == "manager"){
        redis.hsetValue(command.m_option[0] + "的群成员列表", command.m_option[1], "管理员");
        // 通知被操作人
        string num = redis.gethash( command.m_option[1] + static_cast<string>("的未读消息"), "通知消息");
        redis.hsetValue( command.m_option[1] + string("的未读消息"), "通知消息", to_string(stoi(num)+1));
        redis.lpush(command.m_option[1] + static_cast<string>("的通知消息"), "您所在的群聊" + command.m_option[0] + "的群主" + 
                    command.m_uid + "将你设为管理员" + GetNowTime());
        // 如果新群主在线，给他的通知套接字一个提醒
        string online = redis.gethash(command.m_option[1], "在线状态");
        if(online != "-1"){
            string friend_recvfd = redis.gethash(command.m_option[1], "通知套接字");
            TcpSocket friendFd_class(stoi(friend_recvfd));
            friendFd_class.sendMsg("您被设为群聊" + command.m_option[0] + "的管理员.");
        }
    }
    cfd_class.sendMsg("ok");
    return;

}
void ExitGroup(TcpSocket cfd_class, Command command){
    // 如果是群主，他无法退群
    string position = redis.gethash(command.m_option[0] + "的群成员列表", command.m_uid);
    if(position == "群主"){
        cfd_class.sendMsg("cannot");
        return;
    }
    // 群成员列表里删除此人，此人的群聊列表里删除该群聊
    redis.delhash(command.m_option[0] + "的群成员列表", command.m_uid);
    redis.delhash(command.m_uid + "的群聊列表", command.m_option[0]);
    // 这个人退出后，通知群里剩下的群主和管理员
    int num = redis.hlen(command.m_option[0] + "的群成员列表");
    redisReply** members = redis.hkeys(command.m_option[0] + "的群成员列表");
    for(int i = 0; i < num; i++){
        string position = redis.gethash(command.m_option[0] + "的群成员列表", members[i]->str);
        if(position == static_cast<string>("管理员") || position == static_cast<string>("群主")){
            string num0 = redis.gethash( members[i]->str + static_cast<string>("的未读消息"), "通知消息");
            redis.hsetValue( members[i]->str + string("的未读消息"), "通知消息", to_string(stoi(num0)+1));
            redis.lpush(members[i]->str + static_cast<string>("的通知消息"), "用户" + command.m_uid +
                        "退出了您管理的群聊" + command.m_option[0] + GetNowTime());
            // 如果群主或者管理员在线，给他的通知套接字一个提醒
            string online = redis.gethash(members[i]->str, "在线状态");
            if(online != "-1"){
                string friend_recvfd = redis.gethash(members[i]->str, "通知套接字");
                TcpSocket friendFd_class(stoi(friend_recvfd));
                friendFd_class.sendMsg("一名用户退出了您管理的群聊" + command.m_option[0]);
            }
        }
    }
    cfd_class.sendMsg("ok");
}
void DisplyMember(TcpSocket cfd_class, Command command){
    int memberdNum = redis.hlen(command.m_option[0] + "的群成员列表");     // 获得群成员列表的成员数量
    // 群成员数量肯定不为0，就遍历成员列表，根据在线状态发送要展示的内容,先展示在线的，再展示不在线的                                                       
    redisReply **member_uid = redis.hkeys(command.m_option[0] + "的群成员列表");
    for(int i = 0; i < memberdNum; i++){
        string member_mark = redis.gethash(member_uid[i]->str, "昵称");
        string isonline = redis.gethash(member_uid[i]->str, "在线状态");
        string position = redis.gethash(command.m_option[0] + "的群成员列表", member_uid[i]->str);
        if(isonline != "-1"){
            cfd_class.sendMsg(L_GREEN + member_mark + NONE + "(" + member_uid[i]->str + ")" + "————" + position);
        }
    }
    for(int i = 0; i < memberdNum; i++){
        string member_mark = redis.gethash(member_uid[i]->str, "昵称");
        string isonline = redis.gethash(member_uid[i]->str, "在线状态");
        string position = redis.gethash(command.m_option[0] + "的群成员列表", member_uid[i]->str);
        if(isonline == "-1"){
            cfd_class.sendMsg(L_WHITE + member_mark + NONE + "(" + member_uid[i]->str + ")" + "————" + position);
        }
    }
    cfd_class.sendMsg("end");
}
void RemoveMember(TcpSocket cfd_class, Command command){
    // 操作者是否为群主或者管理员
    string position = redis.gethash(command.m_option[0] + "的群成员列表", command.m_uid);
    if(position == "群成员"){
        cfd_class.sendMsg("cannot");
        return;
    }
    // 只能移除群成员
    string position0 = redis.gethash(command.m_option[0] + "的群成员列表", command.m_option[1]);
    if(position0 != "群成员"){
        cfd_class.sendMsg("cannot0");
        return;
    }
    // 被操作人是否在群里
    if(!redis.hashexists(command.m_option[0] + "的群成员列表", command.m_option[1])){
        cfd_class.sendMsg("no");
        return;
    }
    // 群成员列表里删除此人，删此人的群聊列表里删除该群聊
    redis.delhash(command.m_option[0] + "的群成员列表", command.m_option[1]);
    redis.delhash(command.m_option[1] + "的群聊列表", command.m_option[0]);
    // 通知这个人
    string num0 = redis.gethash( command.m_option[1] + "的未读消息", "通知消息");
    redis.hsetValue( command.m_option[1] + "的未读消息", "通知消息", to_string(stoi(num0)+1));
    redis.lpush(command.m_option[1] + "的通知消息",  "您被群聊" +
                    command.m_option[0] + "的" + position + command.m_uid + "移出了群聊" + GetNowTime());
    // 如果这个人在线，给他的通知套接字一个提醒
    string online = redis.gethash(command.m_option[1], "在线状态");
    if(online != "-1"){
        string member_recvfd = redis.gethash(command.m_option[1], "通知套接字");
        TcpSocket friendFd_class(stoi(member_recvfd));
        friendFd_class.sendMsg("您被移移出了群聊" + command.m_option[0]);
    }
    // 通知群主和管理员
    int num = redis.hlen(command.m_option[0] + "的群成员列表");
    redisReply** members = redis.hkeys(command.m_option[0] + "的群成员列表");
    for(int i = 0; i < num; i++){
        string position = redis.gethash(command.m_option[0] + "的群成员列表", members[i]->str);
        if(position == static_cast<string>("管理员") || position == static_cast<string>("群主")){
            if(members[i]->str != command.m_uid){
                string num0 = redis.gethash( members[i]->str + static_cast<string>("的未读消息"), "通知消息");
                redis.hsetValue( members[i]->str + static_cast<string>("的未读消息"), "通知消息", to_string(stoi(num0)+1));
                redis.lpush(members[i]->str + static_cast<string>("的通知消息"),  "您管理的群聊" +
                             command.m_option[0] + "的" + position + command.m_uid + "将用户" + 
                             command.m_option[1] + "移出了群聊" + GetNowTime());
                // 如果群主或者管理员在线，给他的通知套接字一个提醒
                string online = redis.gethash(members[i]->str, "在线状态");
                if(online != "-1"){
                    string friend_recvfd = redis.gethash(members[i]->str, "通知套接字");
                    TcpSocket friendFd_class(stoi(friend_recvfd));
                    friendFd_class.sendMsg("一名用户被移出了您管理的群聊" + command.m_option[0]);
                }
            }
        }
    }
    cfd_class.sendMsg("ok");
    return;
}
void InfoXXXX(TcpSocket cfd_class, Command command){
    return;
}
void SendFile(TcpSocket cfd_class, Command command){
    int filefd;
    int n = command.m_option[2].size();
    string buffer = command.m_option[2];
    string FilePath = command.m_option[1];
    string filename(FilePath,FilePath.rfind("/") + 1); 
    string filepath = "/home/yuanye/Code/Code_Cpp/Chatroom/file/" + command.m_uid + "-" + command.m_option[0];
    // 如果是刚开始发文件，就先创建目录，并返回
    if(command.m_option[2] == "begin"){
        string cmd = "777 " + filepath;
        system(string("mkdir -m " + cmd).c_str());
        cout << "目录创建成功." << endl;
        return;
    }
    // 接收文件内容
    string File = filepath + "/" + filename;
    cout << "接收到的文件存储位置为："<<  File << endl;
    if((filefd = open(File.c_str(), O_WRONLY | O_CREAT | O_APPEND, S_IRWXU)) < 0){
        cout << "文件打开失败." << endl;
        cfd_class.sendMsg("no");
    }
    int r = write(filefd, buffer.c_str(), n);
    if(r < 0){
        cout << "write() failed." << endl;
        cfd_class.sendMsg("no");
    }
    //文件接收完毕的处理
    if(command.m_option[2] == "end"){
        // 将新的消息加入到我对他的消息队列
        string msg0 = "我发送了一个文件：" + filename + ".........." + GetNowTime();
        redis.lpush(command.m_uid + "--" + command.m_option[0], msg0);
        // 当前聊天界面展示我的消息
        string my_recvfd = redis.gethash(command.m_uid, "通知套接字");
        TcpSocket myFd_class(stoi(my_recvfd));
        myFd_class.sendMsg(UP + msg0);
        // 如果好友把自己屏蔽的话，什么都不做，直接返回
        if(redis.scard(command.m_option[0] + "的屏蔽列表")){
            if(redis.sismember(command.m_option[0] + "的屏蔽列表", command.m_uid)){
                return;
            }
        }
        // 没有被屏蔽，消息加到他对我的消息队列，并给相应通知
        string name0 = redis.gethash(command.m_option[0] + "的好友列表", command.m_uid);  // 得到好友給我的备注
        string msg1 = name0 + "发送了一个文件：" + filename + ".........." + GetNowTime();   
        redis.lpush(command.m_option[0] + "--" + command.m_uid, msg1);  // 把消息加入的好友的聊天队列

        // 如果好友把自己屏蔽的话，啥都不做，如果没有被屏蔽，就进行下面的操作：
        // 如果好友在线且处于和自己的聊天界面，就把消息内容发给通知套接字
        // 否则，把消息添加到未读消息里
        // 如果好友在线但不处于和自己的聊天界面，把提示消息发给通知套接字
        string online = redis.gethash(command.m_option[0], "在线状态");
        string ChatFriend = redis.gethash(command.m_option[0], "聊天对象");
        // 好友在线且和我聊天，让通知套接字展示消息，并返回
        if(online != "-1" && ChatFriend == command.m_uid){   // 好友在线且和我聊天
            string friend_recvfd = redis.gethash(command.m_option[0], "通知套接字");
            TcpSocket friendFd_class(stoi(friend_recvfd));
            string begin = "\r\n";
            friendFd_class.sendMsg(begin + UP + msg1);
        }
        else{// 否则，好友的未读消息中的来自我的消息数量+1
            if(redis.hashexists(command.m_option[0] + "的未读消息", "来自" + command.m_uid + "的未读消息")){
                string num1 = redis.gethash(command.m_option[0] + "的未读消息", "来自" + command.m_uid + "的未读消息");
                redis.hsetValue(command.m_option[0] + "的未读消息", "来自" + command.m_uid + "的未读消息", to_string(stoi(num1)+1));
            }else{
                redis.hsetValue(command.m_option[0] + "的未读消息", "来自" + command.m_uid + "的未读消息", "1");
            }
        }
        // 如果好友在线但是没和我聊天，让通知套接字告知来消息
        if(online != "-1" && ChatFriend != command.m_uid){  // 好友在线但没和我聊天
            string friend_recvfd = redis.gethash(command.m_option[0], "通知套接字");
            TcpSocket friendFd_class(stoi(friend_recvfd));
            friendFd_class.sendMsg(command.m_uid + "发来了一个文件");
        }
        redis.lpush(command.m_uid + "发给" + command.m_option[0] + "的文件", FilePath);
    }
    cfd_class.sendMsg("ok");
    close(filefd);
}
void RecvFile(TcpSocket cfd_class, Command command){
    
}
#endif 