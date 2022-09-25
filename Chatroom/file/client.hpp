#include "../lib/TCPSocket.hpp"
#include "Display.hpp"
#include "Input.hpp"
#include <cstdio>
#include <cstring>
#include <iostream>
#include <pthread.h>
#include <sys/types.h>
#include <string>
#include <signal.h>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>

void my_error(const char* errorMsg);
void *recvfunc(void* arg);
void Quit();
string Login(TcpSocket cfd_class);
bool Register(TcpSocket cfd_class);
void Quit(TcpSocket cfd_class);
bool AddFriend(TcpSocket cfd_class, Command command);
bool AddGroup(TcpSocket cfd_class, Command command);
bool AgreeAddFriend(TcpSocket cfd_class, Command command);
bool ListFirHistory(TcpSocket cfd_class, Command command);
bool ChatFriend(TcpSocket cfd_class, Command command);
bool ChatGroup(TcpSocket cfd_class, Command command);
bool ExitChatFriend(TcpSocket cfd_class, Command command);
bool ExitChatGroup(TcpSocket cfd_class, Command command);
bool ShieldFriend(TcpSocket cfd_class, Command command);
bool DeleteFriend(TcpSocket cfd_class, Command command);
bool Restorefriend(TcpSocket cfd_class, Command command);
bool NewMessage(TcpSocket cfd_class, Command command);
bool LookSystem(TcpSocket cfd_class, Command command);
bool LookNotice(TcpSocket cfd_class, Command command);
bool RefuseAddFriend(TcpSocket cfd_class, Command command);
bool CreateGroup(TcpSocket cfd_class, Command command);
bool ListGroup(TcpSocket cfd_class, Command command);
bool AboutGroup(TcpSocket cfd_class, Command command);
bool RequestList(TcpSocket cfd_class, Command command);
bool PassApply(TcpSocket cfd_class, Command command);
bool DenyApply(TcpSocket cfd_class, Command command);
bool SetMember(TcpSocket cfd_class, Command command);
bool ExitGroup(TcpSocket cfd_class, Command command);
bool DisplyMember(TcpSocket cfd_class, Command command);
bool RemoveMember(TcpSocket cfd_class, Command command);
bool InfoXXXX(TcpSocket cfd_class, Command command);

struct RecvArg{
    string myuid;
    int recv_fd = -1;
    RecvArg(string Myuid, int Recv_uid) : myuid(Myuid),recv_fd(Recv_uid) {} 
};
void my_error(const char* errorMsg){
    cout << errorMsg << endl;
    strerror(errno);
    exit(1);
}
void *recvfunc(void* arg){
    RecvArg *recv_arg = static_cast<RecvArg*>(arg);
    TcpSocket recv_class(recv_arg->recv_fd);
    recv_class.connectToHost("127.0.0.1", 6666);
    Command command(recv_arg->myuid, SETRECVFD, {"空"});
    int ret = recv_class.sendMsg(command.To_Json());
    if(ret == 0 || ret == -1){
        delete recv_arg;
        cout << "服务器已关闭" << endl;
        exit(0);
    }
    while(true){
        string message = recv_class.recvMsg();
        if(message == "close" || message == "-1"){
            cout << "通知套接字已关闭" << endl;
            delete recv_arg;
            exit(0);
        }
        cout << message << endl;
    }
    return nullptr;
}
void Quit(TcpSocket cfd_class){
    cfd_class.sendMsg("quit");
}
string Login(TcpSocket cfd_class){
    string input_uid;
    string pwd;
    input_uid = get_uid();  // 读入账号
    pwd = get_pwd();        // 读入密码
   
    // 命令包装成command类
    Command command(input_uid, LOGHIN_CHECK, {pwd});
    int ret = cfd_class.sendMsg(command.To_Json());  // 命令类将类转josn格式，再转json字符串格式，套接字类发送json字符串格式
    if(ret == 0 || ret == -1){
        cout << "服务器已关闭" << endl;
        exit(0);
    }
    // 收到操作结果
    // cout << "准备收到" << endl;
    string check = cfd_class.recvMsg();
    // cout << "收到" << endl;
    // cout << check << endl;
    if (check == "close" || check == "-1"){
        exit(0);
    }else if(check == "incorrect"){
        cout << "账号或密码错误." << endl;
        return "false";
    }else if (check == "online"){
        cout << "该账号正在登录中" << endl;
        return "false";
    }else if(check == "ok"){
        // 登录成功就新建一个线程等回信
        pthread_t tid;
        RecvArg *recv_arg = new RecvArg(input_uid,cfd_class.getrecvfd());
        ret = pthread_create(&tid, NULL, &recvfunc,