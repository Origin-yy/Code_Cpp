#ifndef OPTION_HPP
#define OPTION_HPP

#include "TCPServer.hpp"
#include "../lib/Command.hpp"
#include "../lib/Color.hpp"
#include "redis.hpp"
#include <string>

#define SETRECVFD      -1
#define QUIT            0
#define LOGHIN_CHECK    1
#define REGISTER_CHECK  2
#define ADDFRIEND       3
#define AGREEADDFRIEND  4
#define LISTFRIEND      5
#define CHATFRIEND      6
#define FRIENDMSG       7
#define EXITCHAT        8
#define SHIELDFRIEND    9
#define DELETEFRIEND   10
#define RESTOREFRIEND  11

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
        case LOGHIN_CHECK :{
            Login(cfd_class, command);
            break;
        }
        case REGISTER_CHECK :{
            Register(cfd_class, command);
            break;
        }
        case ADDFRIEND:{
            AddFriend(cfd_class, command);
            break;
        }
        case AGREEADDFRIEND:{
            AgreeAddFriend(cfd_class, command);
            break;
        }
        case LISTFRIEND : {
            ListFriend(cfd_class, command);
            break;
        }
        case CHATFRIEND:{
            ChatFriend(cfd_class, command);
            break;
        }
        case FRIENDMSG:{
            FriendMsg(cfd_class, command);
            break;
        }
        case EXITCHAT:{
            ExitChatFriend(cfd_class, command);
            break;
        }
        case SHIELDFRIEND:{
            ShieldFriend(cfd_class, command);
            break;
        }
        case DELETEFRIEND:{
            DeleteFriend(cfd_class, command);
            break;
        }
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
            redis.hsetValue(new_uid, "好友数量", "0");
            redis.hsetValue(new_uid, "群聊数量", "0");
            redis.hsetValue(new_uid, "消息数量", "0");
            redis.hsetValue(new_uid, "屏蔽数量", "0");
            redis.hsetValue(new_uid, "聊天对象", "无");
            cfd_class.sendMsg(new_uid);
            return;
        }
    }
}
void AddFriend(TcpSocket cfd_class, Command command){
    // 判断准好友是否已在好友列表里
    bool had = false;                                                             // 默认准好友不在列表中
    string friendNum = redis.gethash(command.m_uid, "好友数量");        // 获得好友数量
    if(friendNum != "0"){                                                         // 好友数量不为0时，列表中可能已经有准好友
        redisReply** f_uid =  redis.hkeys(command.m_uid + "的好友列表");       // 得到好友列表
        for(int i = 0; i < stoi(friendNum); i++){                            // 遍历好友列表，看看准好友是否在里面
            if(f_uid[i]->str == command.m_option[0]){
                had = true;
                return;
            }
        }
    }
    if(had == true){                                                   // 准好友已在列表中就告诉客户端并停止添加好友
        cfd_class.sendMsg("had");
        return;
    }

    // 不在就添加他，发送申请
    else if(redis.sismember("accounts", command.m_option[0])){                // 不在就判断准好友的账户是否存在
        // 账户存在就在准好友的列表里更新自己的申请消息，只会存在一条由自己发来的好友申请，一旦申请通过，申请者不能再发申请，接收者不能再同意申请（申请和同意都要查好友列表里是否有该好友）
        string apply = command.m_option[1] + GetNowTime();
        json jn = apply;
        string apply_js = jn.dump();
        cout << apply_js << endl;
        redis.hsetValue(command.m_option[0] + "--系统", command.m_uid, apply_js);
        // 如果准好友在线，给他的通知套接字一个提醒,不在就只加入到未读消息列表
        string online = redis.gethash(command.m_option[0], "在线状态");
        if(online != "-1"){
            string friend_recvfd = redis.gethash(command.m_option[0], "通知套接字");
            TcpSocket friendFd_class(stoi(friend_recvfd));
            friendFd_class.sendMsg("收到一条好友申请." + GetNowTime());
            cfd_class.sendMsg("ok");
        }else {
            // 加入到未读消息列表
            cfd_class.sendMsg("ok");
        };
    }else{
        cfd_class.sendMsg("nofind");
    }
}
void AgreeAddFriend(TcpSocket cfd_class, Command command){
    // 看看自己的好友列表里是否已有该好友，没有就可以同意申请，有就不可以同意申请，回复had
    if( redis.gethash(command.m_uid, "好友数量") == "0" || !redis.hashexists(command.m_uid + "的好友列表", command.m_option[0])){
        // 系统消息列表里又没有他的申请
        if(!redis.hashexists(command.m_uid + "--系统", command.m_option[0])){
            cfd_class.sendMsg("nofind");
            return;
        }
        // 同意者的信息完善
        string friend_mark0  = redis.gethash(command.m_option[0], "昵称");                         // 获得申请者的昵称作为默认备注
        redis.hsetValue(command.m_uid + "的好友列表", command.m_option[0], friend_mark0);    // 在同意者好友列表里插入申请者的uid和默认备注
        string friendNum0 = redis.gethash(command.m_uid, "好友数量");                              // 获得同意者当前的好友数量
        redis.hsetValue(command.m_uid, "好友数量", to_string(stoi(friendNum0)+1));  // 同意者的好友数量+1
        redis.lpush(command.m_uid + "--" + command.m_option[0], "\"*********************\"");         // 为同意者建一个聊天会话
        // 申请者的信息完善
        string friend_mark1 = redis.gethash(command.m_uid, "昵称");                                    // 获得同意者的昵称
        redis.hsetValue(command.m_option[0] + "的好友列表", command.m_uid, friend_mark1);        // 在申请者的好友列表里插入申请者的uid和默认备注
        string friendNum1 = redis.gethash(command.m_option[0], "好友数量");                            // 获得申请者当前的好友数量
        redis.hsetValue(command.m_option[0], "好友数量", to_string(stoi(friendNum1)+1));// 申请者的好友数量+1
        redis.lpush(command.m_option[0] + "--" + command.m_uid, "\"*********************\"");             // 为申请者建一个聊天会话
        // 结果处理，通知双方
        redis.hsetValue(command.m_option[0] + "--系统", command.m_uid, command.m_uid + "通过了您的好友申请." + GetNowTime());// 在申请者的系统消息里写入通过消息
        redis.delhash(command.m_uid + "--系统", command.m_option[0]);        // 在同意者的系统消息这边删除申请者的申请

        // 加入到未读消息队列

        // 如果申请者在线，给他的通知套接字一个提醒并加入到未读消息列表，如果不在，就只加入到未读消息队列
        string online = redis.gethash(command.m_option[0], "在线状态");
        if(online != "-1"){
            string friend_recvfd = redis.gethash(command.m_option[0], "通知套接字");
            TcpSocket friendFd_class(stoi(friend_recvfd));
            friendFd_class.sendMsg(UP + command.m_uid + "已通过了您的好友申请." + GetNowTime());
            cfd_class.sendMsg("ok");
            return;
        }else{
            cfd_class.sendMsg("ok");
            return;
        }
    }else{
        cfd_class.sendMsg("had");
        return;
    }
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
            json msg_js = msg_js.parse(MsgHistory[i]->str);
            string msg = msg_js.get<string>();
            cfd_class.sendMsg(msg);
        }
        cfd_class.sendMsg("以上为历史聊天记录");
        redis.hsetValue(command.m_uid, "聊天对象", command.m_option[0]);
    }
}
void FriendMsg(TcpSocket cfd_class, Command command){
    if(!redis.hashexists(command.m_uid + "的好友列表", command.m_option[0])){
        cfd_class.sendMsg("nohave");
        return;
    }
    // 将新的消息加入到两个人的两个消息队列
    string msg0 = "我：" + command.m_option[1] + ".........." + GetNowTime();
    json jn0 = msg0;
    string msg0_js = jn0.dump();
    redis.lpush(command.m_uid + "--" + command.m_option[0], msg0_js);// 把聊天消息加入到我的聊天队列里
    bool sheild = false;
    // 如果好友把自己屏蔽的话，什么都不做，直接返回，如果没有被屏蔽，就进行下面的操作：  
    if(redis.gethash(command.m_option[0], "屏蔽数量") != "0"){
        if(redis.sismember(command.m_option[0] + "的屏蔽列表", command.m_uid)){
            sheild = true;
        }
    }
    if(sheild == false){
        string name0 = redis.gethash(command.m_option[0] + "的好友列表", command.m_uid);
        string msg1 = name0 + "：" + command.m_option[1] + ".........." + GetNowTime();   // 得到好友给我的备注
        json jn1 = msg1;
        string msg1_js = jn1.dump();
        redis.lpush(command.m_option[0] + "--" + command.m_uid, msg1_js);  // 把消息加入的好友的聊天队列

        string my_recvfd = redis.gethash(command.m_uid, "通知套接字");
        TcpSocket myFd_class(stoi(my_recvfd));
        myFd_class.sendMsg(msg0);

        // 如果好友把自己屏蔽的话，啥都不做，如果没有被屏蔽，就进行下面的操作：
        // 如果好友在线且处于和自己的聊天界面，就把消息内容发给通知套接字；
        // 如果好友在线但不处于和自己的聊天界面，就把提示消息发给通知套接字
        // 如果好友不在线，不给通知套接字发消息，但是把消息添加到未读消息队列里
        string online = redis.gethash(command.m_option[0], "在线状态");
        string ChatFriend = redis.gethash(command.m_option[0], "聊天对象");

        if(online != "-1" && ChatFriend == command.m_uid){   // 好友在线且和我聊天
            string friend_recvfd = redis.gethash(command.m_option[0], "通知套接字");
            TcpSocket friendFd_class(stoi(friend_recvfd));
            friendFd_class.sendMsg(msg1);

        }else if(online != "-1" && ChatFriend != command.m_uid){  // 好友在线但没和我聊天
            string friend_recvfd = redis.gethash(command.m_option[0], "通知套接字");
            TcpSocket friendFd_class(stoi(friend_recvfd));
            friendFd_class.sendMsg(command.m_uid + "发来了一条消息.........." + GetNowTime());
        }else if(online == "-1" ){ // 好友不在线
            // 加入到未读消息队列
        };
    }
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
    if(!redis.hashexists(command.m_uid + "的好友列表", command.m_option[0])){
        cfd_class.sendMsg("no");
    }else{
        string shieldNum = redis.gethash(command.m_uid, "屏蔽数量");
        if(shieldNum == "0"){
            redis.saddvalue(command.m_uid + "的屏蔽列表", command.m_option[0]);
            redis.hsetValue(command.m_uid, "屏蔽数量", to_string(stoi(shieldNum) + 1));
            cfd_class.sendMsg("ok");
            return;
        }else{
            bool had = false;
            redisReply** shield_list = redis.hkeys(command.m_uid + "的屏蔽列表");    
            for(int i = 0; i < stoi(shieldNum); i++){                            
                if(shield_list[i]->str == command.m_option[0]){
                    had = true;
                    return;
                }
            }
            if(had == true){ 
                cfd_class.sendMsg("had");
                return;
            }else{
                redis.saddvalue(command.m_uid + "的屏蔽列表", command.m_option[0]);
                redis.hsetValue(command.m_uid, "屏蔽数量", to_string(stoi(shieldNum) + 1));
                cfd_class.sendMsg("ok");
                return;
            }
        }
    }
}
void DeleteFriend(TcpSocket cfd_class, Command command){
    if(!redis.hashexists(command.m_uid + "的好友列表", command.m_option[0])){
        cfd_class.sendMsg("nofind");
        return;
    }else{
        // 双方好友列表删除对方
        redis.delhash(command.m_uid + "的好友列表", command.m_option[0]);
        redis.delhash(command.m_option[0] + "的好友列表", command.m_uid);
        // 双方好友数量-1
        string friendNum0 = redis.gethash(command.m_uid, "好友数量");
        redis.hsetValue(command.m_uid, "好友数量", to_string(stoi(friendNum0)+1));
        string friendNum1 = redis.gethash(command.m_option[0], "好友数量");
        redis.hsetValue(command.m_option[0], "好友数量", to_string(stoi(friendNum1)+1));
        // 系统消息里告诉被删者
        redis.hsetValue(command.m_option[0] + "的系统消息", command.m_uid, command.m_uid + "删除了您          " + GetNowTime());
        cfd_class.sendMsg("ok");
        return;
    }
}

#endif