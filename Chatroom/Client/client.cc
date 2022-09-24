#include "client.hpp"
#include "Display.hpp"
#include <cstddef>
#include <pthread.h>

#define SERVER_IP     "127.0.0.1"
#define SERVER_PORT    6666

using json = nlohmann::json;

int main(){
    int ret;
    string my_uid;
    signal(SIGTSTP,SIG_IGN);  // 忽略 Ctrl Z
    //signal(SIGINT,SIG_IGN);             // 忽略 Ctrl C
    string begin;          // 登录或者注册
    Command command;       // 要发送的命令类
    TcpSocket cfd_class("recv");   // 本客户端的套接字类（交互套接字和通信套接字）
    // 连接服务器
    ret = cfd_class.connectToHost(SERVER_IP, SERVER_PORT);
    if(ret == -1){
        my_error("connect()");
        exit(0);
    }
    //选择登录、注册、退出操作,并进入不同的函数

    bool isok = false;
    while (!isok){
        display_login();
        begin = get_login();
        if(begin == "login"){
            my_uid = Login(cfd_class);
            if(my_uid == "close"){
                exit(0);
            }else if(my_uid != "false"){
                isok = true;
                system("clear");
                cout << "登录成功" << endl;
            }
        } else if(begin == "register"){
            Register(cfd_class);
            continue;
        } else {
            cout << "感谢您的使用,再见." << endl;
            return 0;
        }
    }
    cout << L_GREEN << "欢迎使用本聊天室." << NONE << endl;
    bool display = true;
    // 循环展示未读消息界面，并获取用户输入，进行交互，展示服务器回复
    while(true){
        // 如果上一次的操作不是展示未读消息界面，就在每一次循环获取用户输入开始前展示未读消息，是的话就不展示
        if(display){
            Command command0(my_uid, NEWMESSAGE, {"newmessage"});
            cout << L_YELLOW << "\n********************" << NONE << endl;
            NewMessage(cfd_class, command0);
            cout << L_YELLOW << "********************" << NONE << endl;
        }
        display = true;
        // 获取操作，清空界面，获得服务器回复并展示回复
        command = get_command(my_uid);
        system("clear");
        switch (command.m_flag) {
        case QUIT :
            cfd_class.sendMsg("quit");
            cout << "感谢您的使用，再见" << endl;
            return 0;
        case ADDFRIEND :
            AddFriend(cfd_class, command);
            break;
        case ADDGROUP :
            AddGroup(cfd_class, command);
            break;
        case AGREEADDFRIEND :
            AgreeAddFriend(cfd_class, command);
            break;
        case LISTFRIEND :
            ListFriend(cfd_class, command);
            break;   
        case CHATFRIEND :
            ChatFriend(cfd_class, command);
            break; 
        case CHATGROUP :
            ChatGroup(cfd_class, command);
            break; 
        case EXITCHAT :
            ExitChatFriend(cfd_class, command);
            break; 
        case SHIELDFRIEND :
            ShieldFriend(cfd_class, command);
            break;
        case DELETEFRIEND :
            DeleteFriend(cfd_class, command);
            break;
        case RESTOREFRIEND :
            Restorefriend(cfd_class, command);
            break;
        case NEWMESSAGE :
        cout << L_YELLOW << "\n********************" << NONE << endl;
            NewMessage(cfd_class, command);
        cout << L_YELLOW << "********************" << NONE << endl;
            display = false;
            break;
        case LOOKSYSTEM :
            LookSystem(cfd_class, command);
            break;
        case LOOKNOTICE :
            LookNotice(cfd_class, command);
            break;
        case REFUSEADDFRIEND :
            RefuseAddFriend(cfd_class, command);
            break;
        case CREATEGROUP :
            CreateGroup(cfd_class, command);
            break;
        case LISTGROUP :
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
        }
    }
}
