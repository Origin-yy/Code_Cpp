#include "client.hpp"
#include "Display.cc"
#include <iostream>
#include <string>
#include<signal.h>

using namespace std;

int main(){
    signal(SIGTSTP,SIG_IGN);  //忽略Ctrl Z
    signal(SIGINT,SIG_IGN);   //忽略Ctrl C
    string command;
    TcpSocket cfd_class;
    cfd_class.connectToHost("192.168.30.148", 6666);
    display_login1();
    command = get_input(LOGIN_OPTIONS);
    cfd_class.sendMsg(command);

}

string get_input(int flag){
    string command;
    getline(cin,command);
    switch (flag) {
        case LOGIN_OPTIONS:
            while(command != "login" && command != "register" && command != "quit") {
                cout << "请输入login、register or quit." << endl;
                cin.clear();
                cin.sync();  //清除输入缓冲区中所有内容
                cin >>command;
            }
            break;
        // case :
        //     break;
        // case :
        //     break;
        // case :
        //     break;
        // case :
        //     break;
        // case :
        //     break;
        // case :
        //     break;
        // case :
        //     break;
    }
    return command;
}

void login(){
    string uid;
    cout << "账号: ";
    cin >> uid;
    while(uid.size() > 11 ) {
                cout << "请输入." << endl;
                cin.clear();
                cin.ignore(1024,'\n');   //或者用cin.ignore();
                cin >> uid;
            }
}