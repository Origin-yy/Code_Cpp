#ifndef OPTION_HPP
#define OPTION_HPP

#include "TCPServer.hpp"
#include "../lib/Command.hpp"
#include "../lib/Color.hpp"
#include "redis.hpp"
#include <hiredis/hiredis.h>
#include <string>

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
#define LOOKSYSTEMMSG   13
#define REFUSEADDFRIEND 14
#define CREATGROUP      15

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
void AgreeAddFriend(TcpSocket cfd_class, Command command);
void ListFriend(TcpSocket cfd_class, Command command);
void ChatFriend(TcpSocket cfd_class, Command command);
void FriendMsg(TcpSocket cfd_class, Command command);
void ExitChatFriend(TcpSocket cfd_class, Command command);
void ShieldFriend(TcpSocket cfd_class, Command command);
void DeleteFriend(TcpSocket cfd_class, Command command);
void Restorefriend(TcpSocket cfd_class, Command command);
void NewList(TcpSocket cfd_class, Command command);
void LookSystemMsg(TcpSocket cfd_class, Command command);
void RefuseAddFriend(TcpSocket cfd_class, Command command);
void CreateGroup(TcpSocket cfd_class, Command command);

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
        case AGREEADDFRIEND:
            AgreeAddFriend(cfd_class, command);
            break;
        case LISTFRIEND : 
            ListFriend(cfd_class, command);
            break;
        case CHATFRIEND:
            ChatFriend(cfd_class, command);
            break;
        case FRIENDMSG:
            FriendMsg(cfd_class, command);
            break;
        case EXITCHAT:
            ExitChatFriend(cfd_class, command);
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
            NewList(cfd_class, command);
            break;
        case LOOKSYSTEMMSG:
            LookSystemMsg(cfd_class, command);
            break;
        case REFUSEADDFRIEND:
            RefuseAddFriend(cfd_class, command);
            break;
    }
    return;
}
void Login(TcpSocket cfd_class, Command command){
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
            redis.hsetValue(command.m_uid, "在线状态", to_string(cfd_class.getfd()));
            redis.hsetValue("fd-uid对应表", to_string(cfd_class.getfd()), command.m_uid);
            redis.hsetValue(command.m_uid, "聊天对象", "0");
            redis.hsetValue(command.m_uid, "通知套接字", "-1");
            cfd_class.sendMsg("ok");
            cout << "已发送ok" << endl; 
        }
    }
}
void Register(TcpSocket cfd_class, Command command){
    srand((unsigned)time(NULL));
    while(true){                                                   
        string new_uid = to_string((rand()+1111)%10000);
        if(redis.sismember("accounts", new_uid)){  // 直到随机一个未注册  uid发过去，并建立账号基本信息
            continue;
        }else if(stoi(new_uid) < 1000){
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
            redis.hsetValue(new_uid, "好友数量", "0");
            redis.hsetValue(new_uid, "屏蔽数量", "0");
            redis.hsetValue(new_uid, "聊天对象", "无");
            redis.hsetValue(new_uid + "的未读消息", "系统消息：", "0");
            cfd_class.sendMsg(new_uid);
            return;
        }
    }
}
void AddFriend(TcpSocket cfd_class, Command command){
    // 系统消息里是否有该用户未处理的申请，有的话就不能发送申请，通知客户端
    if(redis.hashexists(command.m_uid + "--系统", command.m_option[0])){
        string msg = redis.gethash(command.m_uid + "--系统", command.m_option[0]);
        string pass(msg.end() - 11,msg.end());
        if(pass == "(未处理)"){
            cfd_class.sendMsg("cannot");
            return;
        }
    }

    // 判断准好友是否已在好友列表里                                                    
    string friendNum = redis.gethash(command.m_uid, "好友数量");        // 获得好友数量
    if(friendNum != "0"){                                                         // 好友数量不为0时，列表中可能已经有准好友
        redisReply** f_uid =  redis.hkeys(command.m_uid + "的好友列表");       // 得到好友列表
        for(int i = 0; i < stoi(friendNum); i++){                            // 遍历好友列表，看看准好友是否在里面
            if(f_uid[i]->str == command.m_option[0]){                            // 准好友已在列表中就告诉客户端并停止添加好友
                cfd_class.sendMsg("had");
                return;
            }
        }
    }

    // 不在就添加他，发送申请
    else if(redis.sismember("accounts", command.m_option[0])){                // 不在就判断准好友的账户是否存在
        // 账户存在就在准好友的列表里更新自己的申请消息，只会存在一条由自己发来的好友申请，一旦申请通过，申请者不能再发申请，接收者不能再同意申请（申请和同意都要查好友列表里是否有该好友）
        string wait = "(未处理)";
        string apply = "来自" + command.m_uid + "的好友申请：" + command.m_option[1] + GetNowTime() + wait;
        redis.hsetValue(command.m_option[0] + "--系统", command.m_uid, apply);
        // 被申请者未读消息中的系统消息数量+1
        string num = redis.gethash(command.m_option[0] + "的未读消息", "系统消息：");
        redis.hsetValue(command.m_option[0] + "的未读消息", "系统消息：", to_string(stoi(num)+1));
        // 如果准好友在线，给他的通知套接字一个提醒
        string online = redis.gethash(command.m_option[0], "在线状态");
        if(online != "-1"){
            string friend_recvfd = redis.gethash(command.m_option[0], "通知套接字");
            TcpSocket friendFd_class(stoi(friend_recvfd));
            friendFd_class.sendMsg("收到一条好友申请." + GetNowTime());
        }
        cfd_class.sendMsg("ok");
    }else{
        cfd_class.sendMsg("nofind");
    }
}
void AgreeAddFriend(TcpSocket cfd_class, Command command){
    // 看看自己的好友列表里是否已有该好友，没有就可以同意申请，有就不可以同意申请，回复had
    if( redis.gethash(command.m_uid, "好友数量") == "0" || !redis.hashexists(command.m_uid + "的好友列表", command.m_option[0])){
        // 系统消息列表里有没有他的申请
        if(!redis.hashexists(command.m_uid + "--系统", command.m_option[0])){
            cfd_class.sendMsg("nofind");
            return;
        }
        // 更改申请者收到的申请消息状态为已通过
        string pass = "(已通过)";
        string msg = redis.gethash(command.m_uid + "--系统", command.m_option[0]);
        string newmsg(msg.begin(), msg.end() - 11);
        string Newmsg = newmsg + pass;
        redis.hsetValue(command.m_uid + "--系统", command.m_option[0], Newmsg);
        // 同意者的信息完善
        string friend_mark0  = redis.gethash(command.m_option[0], "昵称");                         // 获得申请者的昵称作为默认备注
        redis.hsetValue(command.m_uid + "的好友列表", command.m_option[0], friend_mark0);    // 在同意者好友列表里插入申请者的uid和默认备注
        string friendNum0 = redis.gethash(command.m_uid, "好友数量");                              // 获得同意者当前的好友数量
        redis.hsetValue(command.m_uid, "好友数量", to_string(stoi(friendNum0)+1));  // 同意者的好友数量+1
        redis.lpush(command.m_uid + "--" + command.m_option[0], "*********************");         // 为同意者建一个聊天会话
        // 申请者的信息完善
        string friend_mark1 = redis.gethash(command.m_uid, "昵称");                                    // 获得同意者的昵称
        redis.hsetValue(command.m_option[0] + "的好友列表", command.m_uid, friend_mark1);        // 在申请者的好友列表里插入申请者的uid和默认备注
        string friendNum1 = redis.gethash(command.m_option[0], "好友数量");                            // 获得申请者当前的好友数量
        redis.hsetValue(command.m_option[0], "好友数量", to_string(stoi(friendNum1)+1));// 申请者的好友数量+1
        redis.lpush(command.m_option[0] + "--" + command.m_uid, "*********************");             // 为申请者建一个聊天会话
        // 在申请者的系统消息里写入通过消息
        redis.hsetValue(command.m_option[0] + "--系统", "\r" + command.m_uid, command.m_uid + "通过了您的好友申请." + GetNowTime());
        // 申请者未读消息中的系统消息数量+1
        string num1 = redis.gethash(command.m_option[0] + "的未读消息", "系统消息：");
        redis.hsetValue(command.m_option[0] + "的未读消息", "系统消息：", to_string(stoi(num1)+1));
        // 如果申请者在线，给他的通知套接字一个提醒
        string online = redis.gethash(command.m_option[0], "在线状态");
        if(online != "-1"){
            string friend_recvfd = redis.gethash(command.m_option[0], "通知套接字");
            TcpSocket friendFd_class(stoi(friend_recvfd));
            string kaitou = UP;
            friendFd_class.sendMsg(kaitou + "\r" + command.m_uid + "已通过了您的好友申请." + GetNowTime());
        }
    }else{
        cfd_class.sendMsg("had");
        return;
    }
    cfd_class.sendMsg("ok");
    return;
}
void ListFriend(TcpSocket cfd_class, Command command){
    string friendNum = redis.gethash(command.m_uid, "好友数量");     // 获得账户信息中的好友数量
    if(friendNum == "0"){
        cfd_class.sendMsg("none");
    }else {
        // 好友数量不为0，就遍历好友列表，根据在线状态发送要展示的内容                                                       
        redisReply **f_uid = redis.hkeys(command.m_uid + "的好友列表");
        for(int i = 0; i < stoi(friendNum); i++){
            string friend_mark = redis.gethash(command.m_uid + "的好友列表", f_uid[i]->str);
            string isonline = redis.gethash(f_uid[i]->str, "在线状态");
            if(isonline != "-1"){
                cfd_class.sendMsg(L_GREEN + friend_mark + NONE + "(" + f_uid[i]->str + ")");
            }else {
                cfd_class.sendMsg(L_WHITE + friend_mark + NONE + "(" + f_uid[i]->str + ")");
            }
        }
        cfd_class.sendMsg("end");
    }
}
void ChatFriend(TcpSocket cfd_class, Command command){
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
        string name = redis.gethash(command.m_uid + "的好友列表", command.m_option[0]);
        if(redis.hashexists(command.m_uid + "的未读消息", name + "：")){
            redis.hsetValue(command.m_uid + "的未读消息", name + "：", "0");
        }
        cfd_class.sendMsg("以上为历史聊天记录");
    }
}
void FriendMsg(TcpSocket cfd_class, Command command){
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
    if(redis.gethash(command.m_option[0], "屏蔽数量") != "0"){
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
        friendFd_class.sendMsg("\r" + msg1);
    }
    else{// 否则，好友的未读消息中的来自我的消息数量+1
        string name = redis.gethash(command.m_option[0] + "的好友列表", command.m_uid);
        if(redis.hashexists(command.m_option[0] + "的未读消息", name + "：")){
            string num1 = redis.gethash(command.m_option[0] + "的未读消息", name + "：");
            redis.hsetValue(command.m_option[0] + "的未读消息", name + "：", to_string(stoi(num1)+1));
        }else{
            redis.hsetValue(command.m_option[0] + "的未读消息", name + "：", "1");
        }
    }
    // 如果好友在线但是没和我聊天，让通知套接字告知来消息
    if(online != "-1" && ChatFriend != command.m_uid){  // 好友在线但没和我聊天
        string friend_recvfd = redis.gethash(command.m_option[0], "通知套接字");
        TcpSocket friendFd_class(stoi(friend_recvfd));
        friendFd_class.sendMsg(command.m_uid + "发来了一条消息.........." + GetNowTime());
    }
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
void ShieldFriend(TcpSocket cfd_class, Command command){
    // 不存在该好友就通知客户端并返回
    if(!redis.hashexists(command.m_uid + "的好友列表", command.m_option[0])){  
        cfd_class.sendMsg("no");
        return;
    }
    // 如果之前没有好友被屏蔽，就直接屏蔽该好友，并返回
    string shieldNum = redis.gethash(command.m_uid, "屏蔽数量");   
    if(shieldNum == "0"){
        redis.saddvalue(command.m_uid + "的屏蔽列表", command.m_option[0]);
        redis.hsetValue(command.m_uid, "屏蔽数量", to_string(stoi(shieldNum) + 1));
        cfd_class.sendMsg("ok");
        return;
    }
    // 如果屏蔽列表里已经有了该好友，通知客户端并返回
    redisReply** shield_list = redis.hkeys(command.m_uid + "的屏蔽列表");    
    for(int i = 0; i < stoi(shieldNum); i++){                            
        if(shield_list[i]->str == command.m_option[0]){
            cfd_class.sendMsg("had");
            return;
        }
    }
    // 该好友没有被屏蔽，就屏蔽
    redis.saddvalue(command.m_uid + "的屏蔽列表", command.m_option[0]);
    redis.hsetValue(command.m_uid, "屏蔽数量", to_string(stoi(shieldNum) + 1));
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
    // 双方好友数量-1
    string friendNum0 = redis.gethash(command.m_uid, "好友数量");
    redis.hsetValue(command.m_uid, "好友数量", to_string(stoi(friendNum0) - 1));
    string friendNum1 = redis.gethash(command.m_option[0], "好友数量");
    redis.hsetValue(command.m_option[0], "好友数量", to_string(stoi(friendNum1) - 1));
    // 系统消息里告诉被删者
    redis.hsetValue(command.m_option[0] + "的系统消息", command.m_uid, command.m_uid + "删除了您          " + GetNowTime());

    // 屏蔽列表里是否有该好友，有就删掉，没有就不管
    string shieldNum = redis.gethash(command.m_uid, "屏蔽数量");            // 是否有好友被屏蔽
    if(shieldNum != "0"){
        redisReply** shield_list = redis.hkeys(command.m_uid + "的屏蔽列表");        // 该好友是否被屏蔽
        for(int i = 0; i < stoi(shieldNum); i++){                            
            if(shield_list[i]->str == command.m_option[0]){                            // 被屏蔽就解屏蔽
                redis.sremvalue(command.m_uid + "的屏蔽列表", command.m_option[0]);
                redis.hsetValue(command.m_uid, "屏蔽数量", to_string(stoi(shieldNum) - 1));
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
    string shieldNum = redis.gethash(command.m_uid, "屏蔽数量");            // 是否有好友被屏蔽
    if(shieldNum == "0"){
        cfd_class.sendMsg("nofind");
        return;
    }
    redisReply** shield_list = redis.hkeys(command.m_uid + "的屏蔽列表");        // 该好友是否被屏蔽
    for(int i = 0; i < stoi(shieldNum); i++){                            
        if(shield_list[i]->str == command.m_option[0]){                            // 被屏蔽就解屏蔽
            redis.sremvalue(command.m_uid + "的屏蔽列表", command.m_option[0]);
            redis.hsetValue(command.m_uid, "屏蔽数量", to_string(stoi(shieldNum) - 1));
            cfd_class.sendMsg("ok");
            return;
        }
    }

    cfd_class.sendMsg("nofind");
    return;
}
void NewList(TcpSocket cfd_class, Command command){
    int NewNum = redis.hlen(command.m_uid + "的未读消息");
        redisReply **NewList = redis.hkeys(command.m_uid + "的未读消息");
        for(int i = 0;i < NewNum; i++){
            string oneNum = redis.gethash(command.m_uid + "的未读消息", NewList[i]->str);
            string oneline = static_cast<string>(NewList[i]->str) + "：" + oneNum;
            cfd_class.sendMsg(oneline);
        }
        cfd_class.sendMsg("end");
}
void LookSystemMsg(TcpSocket cfd_class, Command command){
    redis.hsetValue(command.m_uid + "的未读消息", "系统消息：", "0");
    int num = redis.hlen(command.m_uid + "--系统");
    if(num == 0){
        cfd_class.sendMsg("none");
        return;
    }
    redisReply **SysMsgList = redis.hkeys(command.m_uid + "--系统");
    for(int i = 0; i < num; i++){
        string sysmsg = redis.gethash(command.m_uid + "--系统", SysMsgList[i]->str);
        cfd_class.sendMsg(sysmsg);
    }
    cfd_class.sendMsg("end");
}
void RefuseAddFriend(TcpSocket cfd_class, Command command){
    // 看看自己的好友列表里是否已有该好友，没有就可以修改申请，有就不可以修改申请，回复had
    if( redis.gethash(command.m_uid, "好友数量") == "0" || !redis.hashexists(command.m_uid + "的好友列表", command.m_option[0])){
        // 系统消息列表里有没有他的申请
        if(!redis.hashexists(command.m_uid + "--系统", command.m_option[0])){
            cfd_class.sendMsg("nofind");
            return;
        }
        // 更改申请者收到的申请消息状态为已通过
        string pass = "(已拒绝)";
        string msg = redis.gethash(command.m_uid + "--系统", command.m_option[0]);
        string newmsg(msg.begin(), msg.end() - 11);
        string Newmsg = newmsg + pass;
        redis.hsetValue(command.m_uid + "--系统", command.m_option[0], Newmsg);
        
        // 在申请者的系统消息里写入未通过消息
        redis.hsetValue(command.m_option[0] + "--系统", "\r" + command.m_uid, command.m_uid + "拒绝了您的好友申请." + GetNowTime());
        // 申请者未读消息中的系统消息数量+1
        string num1 = redis.gethash(command.m_option[0] + "的未读消息", "系统消息：");
        redis.hsetValue(command.m_option[0] + "的未读消息", "系统消息：", to_string(stoi(num1)+1));
        // 如果申请者在线，给他的通知套接字一个提醒
        string online = redis.gethash(command.m_option[0], "在线状态");
        if(online != "-1"){
            string friend_recvfd = redis.gethash(command.m_option[0], "通知套接字");
            TcpSocket friendFd_class(stoi(friend_recvfd));
            string kaitou = UP;
            friendFd_class.sendMsg(kaitou + "\r" + command.m_uid + "已拒绝了您的好友申请." + GetNowTime());
        }
    }else{
        cfd_class.sendMsg("had");
        return;
    }
    cfd_class.sendMsg("ok");
    return;
}
void CreateGroup(TcpSocket cfd_class, Command command){
    // 检查发过来的uid是否都是用户的好友，有一个不是就返回并提醒客户端
    int len = command.m_option[0].size();
    vector<string> members;
    for(int i = 0; i < len/5; i++){
        string member(command.m_option[0].begin() + i*5, command.m_option[0].begin() + i*5 + 4);
        if(!redis.hashexists(command.m_uid + "的好友列表", command.m_option[0])){
            cfd_class.sendMsg("nofind" + member);
            return;
        }
        members.push_back(member);
    }
    string last(command.m_option[0].end() - 4, command.m_option[0].end());
    if(!redis.hashexists(command.m_uid + "的好友列表", command.m_option[0])){
        cfd_class.sendMsg("nofind" + last);
        return;
    }
    members.push_back(last);
    srand((unsigned)time(NULL));
    while(true){                                                   
        string new_gid = to_string((rand()+111)%1000);
        if(redis.sismember("群聊集合", new_gid)){  // 直到随机一个未注册的gid发过去，并建立群聊基本信息
            continue;
        }else if(stoi(new_gid) < 100){
            continue;
        }else{
            redis.saddvalue("群聊集合", new_gid);
            redis.hsetValue(new_gid + "基本信息", "群号", new_gid);
            redis.hsetValue(new_gid + "基本信息", "群名", new_gid);
            redis.hsetValue(new_gid + "基本信息", "创建时间", GetNowTime());
            redis.hsetValue(new_gid + "基本信息", "群介绍", "暂无");
            redis.hsetValue(new_gid + "基本信息", "群公告", "暂无");

            redis.hsetValue(new_gid + "群成员列表", command.m_uid, "群主");
            redis.hsetValue(command.m_uid + "的群聊列表", new_gid, new_gid);
            redis.lpush(command.m_uid + "--" + new_gid, "***************");

            for(auto member: members){
                redis.hsetValue(new_gid + "群成员列表", member, "群成员");
                redis.hsetValue(member + "的群聊列表", new_gid, new_gid);
                redis.lpush(member + "--" + new_gid, "***************");
            }
            cfd_class.sendMsg(new_gid);
            return;
        }
    }
}
#endif