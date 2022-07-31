#include "../lib/TCPSocket.hpp"
#include "Display.hpp"
#include "Input.hpp"
#include <pthread.h>
#include <sys/types.h>
#include <string>
#include <signal.h>

void my_error(const char* errorMsg);
void *recvfunc(void* arg);
void Quit();
string Login(TcpSocket cfd_class);
bool Register(TcpSocket cfd_class);
void Quit(TcpSocket cfd_class);
bool AddFriend(TcpSocket cfd_class, Command command);
bool AddGroup(TcpSocket cfd_class, Command command);
bool AgreeAddFriend(TcpSocket cfd_class, Command command);

struct RecvArg{
    string myuid;
    int recv_fd = -1;
    RecvArg(string Myuid, int Recv_uid) : myuid(Myuid),recv_fd(Recv_uid) {} 
};

// 错误函数
void my_error(const char* errorMsg){
    cout << errorMsg << endl;
    strerror(errno);
    exit(1);
}

// 接收通知的线程任务函数
void *recvfunc(void* arg){
    RecvArg *recv_arg = static_cast<RecvArg*>(arg);
    TcpSocket recv_class(recv_arg->recv_fd);
    recv_class.sendMsg(recv_arg->myuid);
    while(true){
        string message = recv_class.recvMsg();
        if(message == "close" || message == "-1"){
            cout << "通知套接字已关闭" << endl;
            break;
        }
        cout << message << endl;
    }
    return nullptr;
}

// 信息交互函数（发送命令并收到回复）
// 登录函数
void Quit(TcpSocket cfd_class){
    cfd_class.sendMsg("quit");
}

// 登录函数
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
        return "close";
        exit(0);
    }
    // 收到操作结果
    cout << "准备收到" << endl;
    string check = cfd_class.recvMsg();
    cout << "收到" << endl;
    cout << check << endl;
    if (check == "close" || check == "-1"){
        return "close";
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
        pthread_create(&tid, NULL, &recvfunc, static_cast<void*>(recv_arg));
        ret = pthread_detach(tid);
        if(ret != 0){
            my_error("pthread_detach()");
        }
        cout << "登录成功" << endl;
        return input_uid;
    }else
        return "false";
}
// 注册函数
bool Register(TcpSocket cfd_class){
    string pwd;
    while(true){
        cout << "请设置" << endl;
        pwd = get_pwd();        // 读入密码
        cout << "请确认";
        string pwd2 = get_pwd();
        if(pwd != pwd2){
            cout << "前后两次密码输入不一致，请重新选择操作·" << endl;
            return false;
        }else{
            break;
        }
    }
    Command command("NULL", REGISTER_CHECK,{pwd});
    int ret = cfd_class.sendMsg(command.To_Json());  // 命令类将类转josn格式，再转json字符串格式，套接字类发送json字符串格式
    if(ret == 0 || ret == -1){
        cout << "服务器已关闭" << endl;
        exit(0);
    }
    // 收到生成的密码
    string new_uid = cfd_class.recvMsg();
    cout << "您注册的uid为: " << new_uid << endl << "忘记后无法找回，请牢记." << endl;
    return true;
}

bool AddFriend(TcpSocket cfd_class, Command command){
    int ret = cfd_class.sendMsg(command.To_Json());
    if(ret == 0 || ret == -1){
        cout << "服务器已关闭." << endl;
        exit (0);
    }
    string check = cfd_class.recvMsg();
    if(check == "close"){
        cout << "服务器已关闭." << endl;
        exit (0);
    }else if(check == "ok"){
        cout << "好友添加申请已发送,等待通过." << endl;
        return true;
    }else{
        cout << "未找到该用户." << endl;
        return false;
    }
}

bool AddGroup(TcpSocket cfd_class, Command command){
    int ret = cfd_class.sendMsg(command.To_Json());
    if(ret == 0 || ret == -1){
        cout << "服务器已关闭." << endl;
        exit (0);
    }
    string check = cfd_class.recvMsg();
    if(check == "close"){
        cout << "服务器已关闭." << endl;
        exit (0);
    }else if(check == "ok"){
        cout << "群聊添加申请已发送,等待通过." << endl;
        return true;
    }else{
        cout << "未找到该群聊." << endl;
        return false;
    }
}

bool AgreeAddFriend(TcpSocket cfd_class, Command command){
    int ret = cfd_class.sendMsg(command.To_Json());
    if(ret == 0 || ret == -1){
        cout << "服务器已关闭." << endl;
        exit (0);
    }
    string check = cfd_class.recvMsg();
    if(check == "close"){
        cout << "服务器已关闭." << endl;
        exit (0);
    }else if(check == "ok"){
        cout << "已通过" << command.m_option[0] << "的好友申请" << endl;
        return true;
    }else{
        cout << "未找到该用户的好友申请." << endl;
        return false;
    }
}