#include "client.hpp"
#include <cstddef>
#include <pthread.h>

using json = nlohmann::json;

int main(){
    int ret;
    string my_uid;
    signal(SIGTSTP,SIG_IGN);  // 忽略 Ctrl Z
    //signal(SIGINT,SIG_IGN);             // 忽略 Ctrl C
    string begin;          // 登录或者注册
    TcpSocket cfd_class("recv");   // 本客户端的套接字类
    Command command;       // 要发送的命令类
    // 连接服务器
    ret = cfd_class.connectToHost("127.0.0.1", 6666);
    if(ret == -1){
        my_error("connect()");
    }
    //选择登录、注册、退出操作,并进入不同的函数

    bool isok = false;
    while (!isok){
        display_login1();
        begin = get_login();
        if(begin == "login"){
            my_uid = Login(cfd_class);
            if(my_uid == "close"){
                exit(0);
            }else if(my_uid != "false"){
                isok = true;
            }
        } else if(begin == "register"){
            Register(cfd_class);
            continue;
        } else {
            cout << "感谢您的使用,再见." << endl;
            return 0;
        }
    }
    while(true){
        command = get_command(my_uid);
        switch (command.m_flag) {
        case QUIT :
            cfd_class.sendMsg("quit");
            cout << "感谢您的使用，再见" << endl;
            return 0;
        case ADDFRIEND :
            AddFriend(cfd_class, command);
            break;
        // case ADDGROUP :
        //     AddGroup(cfd_class, command);
        //     break;
        case AGREEADDFRIEND :
            AgreeAddFriend(cfd_class, command);
            break;
        case LISTFRIEND :
            ListFriend(cfd_class, command);
            break;   
        case CHATFRIEND :
            ChatFriend(cfd_class, command);
            break; 
        }
    }
}
