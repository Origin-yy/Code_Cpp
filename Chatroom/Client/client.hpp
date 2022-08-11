#include "../lib/TCPSocket.hpp"
#include "Display.hpp"
#include "Input.hpp"
#include <cstdio>
#include <iostream>
#include <pthread.h>
#include <sys/types.h>
#include <string>
#include <signal.h>
#include <cstdlib>

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
bool ExitChatFriend(TcpSocket cfd_class, Command command);
bool ShieldFriend(TcpSocket cfd_class, Command command);
bool DeleteFriend(TcpSocket cfd_class, Command command);
bool Restorefriend(TcpSocket cfd_class, Command command);
bool NewMessage(TcpSocket cfd_class, Command command);
bool LookSystem(TcpSocket cfd_class, Command command);
bool LookNotice(TcpSocket cfd_class, Command command);
bool RefuseAddFriend(TcpSocket cfd_class, Command command);
bool CreateGroup(TcpSocket cfd_class, Command command);
bool ListGroup(TcpSocket cfd_class, Command command);
bool LookGroupApply(TcpSocket cfd_class, Command command);

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
        ret = pthread_create(&tid, NULL, &recvfunc, static_cast<void*>(recv_arg));
        if(ret != 0){
            my_error("pthread_create()");
            exit(0);
        }
        ret = pthread_detach(tid);
        if(ret != 0){
            my_error("pthread_detach()");
            exit(0);
        }
         // cout << "登录成功" << endl;
        return input_uid;
    }else
        return "false";
}
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
    }else if(check == "had"){
        cout << "该用户已经是您的好友" << endl;
        return false;
    }else if(check == "cannot1"){
        cout << "您的系统消息中存在对方发来的好友申请，请先回复." << endl;
        return false;
    }else if(check == "cannot2"){
        cout << "您之前已向该用户发送好友申请,请等待回复." << endl;
        return false;
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
    }else if(check == "had"){
        cout << "您已在该群聊中" << endl;
        return false;
    }else if(check == "cannot"){
        cout << "该群聊的申请列表中已有您的未处理的申请,请先处理." << endl;
        return false;
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
        cout << "已通过" << command.m_option[0] << "的好友申请." << endl;
        return true;
    }else if(check == "had"){
        cout << "该用户已经是您的好友." << endl;
        return false;
    }else if(check == "haddeal"){
        cout << "您已经处理了该用户的申请." << endl;
        return false;
    }else if(check == "nofind"){
        cout << "未找到该用户的好友申请." << endl;
        return false;
    }
    else{
        cout << "其他错误" << endl;
        return false;
    }
}
bool ListFriend(TcpSocket cfd_class, Command command){
    int ret = cfd_class.sendMsg(command.To_Json());
    if(ret == 0 || ret == -1){
        cout << "服务器已关闭." << endl;
        exit (0);
    }
    while(true){
        string Friend = cfd_class.recvMsg();
        if(Friend == "end"){
            cout << "好友展示完毕" << endl;
            break;
        }else if(Friend == "none"){
            cout << "您当前还没有好友" << endl;
            return false;
        }else if(Friend == "close"){
            cout << "服务器已关闭." << endl;
            exit (0);
        }else {
            cout << Friend << endl;
        }
    }
    return true;
}
bool ChatFriend(TcpSocket cfd_class, Command command){
    int ret = cfd_class.sendMsg(command.To_Json());  // 发送聊天请求
    if(ret == 0 || ret == -1){
        cout << "服务器已关闭." << endl; 
        exit(0);
    }            
    string check = cfd_class.recvMsg();        // 检查回复
    if(check == "close"){
        cout << "服务器已关闭." << endl; 
        exit(0);
    }else if(check == "none"){
        cout << "您还没有好友." << endl;
        return false;
    }else if(check == "nofind"){
        cout << "未找到该好友." << endl;
        return false;
    }
    // 有这个好友就打印历史聊天记录
    else if(check == "have"){
        string HistoryMsg;
        while(true){
            HistoryMsg = cfd_class.recvMsg();
            if(HistoryMsg == "close" || HistoryMsg == "-1"){
                cout << "服务器已关闭" << endl;
                exit(0);
            } 
            cout << HistoryMsg << endl;
            if(HistoryMsg == "以上为历史聊天记录"){
                break;
            }
        }
        // 给好友发送消息,^[ (ESC） 退出聊天，并更改自己的聊天对象
        string msg;
        while(true){
            //cout << "请输入您想发送的消息：" << endl; 
            getline(cin,msg);
            // 用户想退出聊天界面，送请求并等待服务器处理完毕
            if(msg == "#"){
                Command command_exit(command.m_uid,EXITCHAT,{"空"});
                ExitChatFriend(cfd_class, command_exit);
                break;
            }
            // 把消息包装好，让服务器转发
            Command command_msg(command.m_uid, FRIENDMSG, {command.m_option[0], msg});
            int ret = cfd_class.sendMsg(command_msg.To_Json());
            if(ret == 0 || ret == -1){
                cout << "服务器已关闭." << endl; 
                exit(0);
            }
        }
    }
    return true;
}
bool ExitChatFriend(TcpSocket cfd_class, Command command){
    int ret = cfd_class.sendMsg(command.To_Json());  // 发送退出聊天请求
    if(ret == 0 || ret == -1){
        cout << "服务器已关闭." << endl; 
        exit(0);
    }
    string check = cfd_class.recvMsg();
    if(check == "close"){
        cout << "服务器已关闭." << endl;
        exit (0);
    }else if(check == "ok"){
        system("clear");
        cout << "已退出聊天" << endl;
        return true;
    }else if (check == "no"){
        cout << "无效的操作，请重新输入." << endl;
        return false;
    }
    return false;
}
bool ShieldFriend(TcpSocket cfd_class, Command command){
    int ret = cfd_class.sendMsg(command.To_Json());
    if(ret == 0 || ret == -1){
        cout << "服务器已关闭." << endl;
        exit(0);
    }
    string check = cfd_class.recvMsg();
    if(check == "close"){
        cout << "服务器已关闭." << endl;
        exit (0);
    }else if(check == "no"){
        cout << "未找到该好友." << endl;
        return false;
    }else if(check == "ok"){
        cout << "已屏蔽该好友的会话." << endl;
        return true;
    }else if(check == "had") {
        cout << "该好友已被屏蔽." << endl;
    }else{
        cout << "错误" << endl;
    }
    return false;
}
bool DeleteFriend(TcpSocket cfd_class, Command command){
    int ret = cfd_class.sendMsg(command.To_Json());
    if(ret == 0 || ret == -1){
        cout << "服务器已关闭." << endl;
        exit(0);
    }
    string check = cfd_class.recvMsg();
    if(check == "close"){
        cout << "服务器已关闭." << endl;
        exit (0);
    }else if(check == "nofind"){
        cout << "未找到该好友." << endl;
        return false;
    }else if(check == "ok"){
        cout << "已删除该好友." << endl;
        return true;
    }
    return false;
}
bool Restorefriend(TcpSocket cfd_class, Command command){
    int ret = cfd_class.sendMsg(command.To_Json());
    if(ret == 0 || ret == -1){
        cout << "服务器已关闭." << endl;
        exit(0);
    }
    string check = cfd_class.recvMsg();
    if(check == "close"){
        cout << "服务器已关闭." << endl;
        exit (0);
    }else if(check == "nohave"){
        cout << "未找到该好友." << endl;
        return false;
    }else if(check == "nofind"){
        cout << "该好友未被屏蔽." << endl;
        return false;
    }else if(check == "ok"){
        cout << "已恢复该好友的会话." << endl;
        return true;
    }else{
        cout << "该好友已被屏蔽." << endl;
    }
    return false;
}
bool NewMessage(TcpSocket cfd_class, Command command){
    int ret = cfd_class.sendMsg(command.To_Json());
    if(ret == 0 || ret == -1){
        cout << "服务器已关闭." << endl;
        exit (0);
    }
    while(true){
        string Friend = cfd_class.recvMsg();
        if(Friend == "end"){
            cout << endl;
            break;
        }else if(Friend == "none"){
            cout << "您当前没有未读消息" << endl;
            break;
        }else if(Friend == "close"){
            cout << "服务器已关闭." << endl;
            exit (0);
        }else {
            cout << Friend << endl;
        }
    }
    return true;
}
bool LookSystem(TcpSocket cfd_class, Command command){
    int ret = cfd_class.sendMsg(command.To_Json());
    if(ret == 0 || ret == -1){
        cout << "服务器已关闭." << endl;
        exit (0);
    }
    while(true){
        string SysMsg = cfd_class.recvMsg();
        if(SysMsg == "end"){
            cout << "以上为系统消息" << endl;
            break;
        }else if(SysMsg == "none"){
            cout << "系统消息为空" << endl;
            break;
        }else if(SysMsg == "close"){
            cout << "服务器已关闭." << endl;
            exit (0);
        }else {
            cout << SysMsg << endl;
        }
    }
    return true;
}
bool LookNotice(TcpSocket cfd_class, Command command){
    int ret = cfd_class.sendMsg(command.To_Json());
    if(ret == 0 || ret == -1){
        cout << "服务器已关闭." << endl;
        exit (0);
    }
    while(true){
        string notice = cfd_class.recvMsg();
        if(notice == "end"){
            cout << "以上为通知消息" << endl;
            break;
        }else if(notice == "none"){
            cout << "通知消息为空" << endl;
            break;
        }else if(notice == "close"){
            cout << "服务器已关闭." << endl;
            exit (0);
        }else {
            cout << notice << endl;
        }
    }
    return true;
}
bool RefuseAddFriend(TcpSocket cfd_class, Command command){
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
        cout << "已拒绝" << command.m_option[0] << "的好友申请" << endl;
        return true;
    }else if(check == "had"){
        cout << "该用户已经是您的好友" << endl;
        return false;
    }else if(check == "haddeal"){
        cout << "您已经处理了该用户的申请." << endl;
        return false;
    }else if(check == "nofind"){
        cout << "未找到该用户的好友申请." << endl;
        return false;
    }
    else{
        cout << UP  << "其他错误" << endl;
        return false;
    }
}
bool CreateGroup(TcpSocket cfd_class, Command command){
    // 调用展示好友列表函数展示好友列表
    Command command0(command.m_uid, LISTFRIEND, {"无"});
    if (!ListFriend(cfd_class, command0)){
        cout << "无法创建群聊." << endl;
        return false;
    }
    // 获得群聊初始成员列表
    cout << "请从以上好友中选择群聊初始成员." << endl;
    cout << "请输入好友的uid,以一个空格分割：" << endl;
    string f_list;
    getline(cin, f_list);
    Command command1(command.m_uid, CREATEGROUP, {f_list});
    // 给服务器发送创建群聊的请求
    int ret = cfd_class.sendMsg(command1.To_Json());
    if(ret == 0 || ret == -1){
        cout << "服务器已关闭." << endl;
        exit (0);
    }
    // 检查回复
    string check = cfd_class.recvMsg();        
    if(check == "close"){
        cout << "服务器已关闭." << endl; 
        exit(0);
    }else if(check.find("nofind") == 0){
        string nofriend(check.begin() + 6, check.end());
        cout << "输入格式有误." << endl;
        cout << "应输入好友的uid并以空格分割." << endl;
        return false;
    }else{
        cout << "群聊创建成功,群号为：" << check << endl;
        return true;
    }
}
bool ListGroup(TcpSocket cfd_class, Command command){
    int ret = cfd_class.sendMsg(command.To_Json());
    if(ret == 0 || ret == -1){
        cout << "服务器已关闭." << endl;
        exit (0);
    }
    while(true){
        string Group = cfd_class.recvMsg();
        if(Group == "end"){
            cout << "群聊展示完毕" << endl;
            break;
        }else if(Group == "none"){
            cout << "您当前还没有加入群聊" << endl;
            return false;
        }else if(Group == "close"){
            cout << "服务器已关闭." << endl;
            exit (0);
        }else {
            cout << Group << endl;
        }
    }
    return true;
}
bool LookGroupApply(TcpSocket cfd_class, Command command){
    int ret = cfd_class.sendMsg(command.To_Json());
    if(ret == 0 || ret == -1){
        cout << "服务器已关闭." << endl;
        exit (0);
    }
    while(true){
        string apply = cfd_class.recvMsg();
        if(apply == "close"){
            cout << "服务器已关闭." << endl;
            exit (0);
        }else if(apply == "none"){
            cout << "当前还没有入群申请" << endl;
            return false;
        }else if(apply == "cannot"){
            cout << "您在该群聊中没有此权限." << endl;
            return false;
        }else if(apply == "end"){
            cout << "入群申请展示完毕." << endl;
            exit (0);
        }else {
            cout << apply << endl;
        }
    }
    return true;
}