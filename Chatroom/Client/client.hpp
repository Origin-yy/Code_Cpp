#include "../lib/TCPSocket.hpp"
#include "../lib/Command.hpp"
#include <sys/types.h>
#include <iostream>
#include <string>
#include <signal.h>

#define LOGHIN_CHECK    1
#define REGISTER_CHECK  2

using namespace std;

void my_error(const char* errorMsg){
    cout << errorMsg << endl;
    strerror(errno);
    exit(1);
}

// 展示界面的函数
void display_login1(){
    cout << "Welcome to the chat room" << endl
         << "Please select" << endl
         << "login/register/quit" << endl;
}

// 获取正确输入的函数
// 获得登录选项
string get_login(){
    string command;
    getline(cin,command);
    while(command != "login" && command != "register" && command != "quit") {
        cout << "请输入login、register or quit." << endl;
        //cin.clear(); cin做if判断时用，把无效的cin改成有效的，不用的话，第一次cin无效后一直无效，死循环
        cin.sync();  //清除输入缓冲区中所有内容
        cin >>command;
    }
    return command;
}
// 得到uid
string get_uid(){
        // 读入输入格式正确的账号
    string input_uid;
    cout << "账号: ";
    getline(cin, input_uid);
    bool isNum = true;
    for(auto c : input_uid){
        if(!isdigit(c)){
            isNum = false;
        }
    }
    while(input_uid.size() != 4 && !isNum) {
                cout << "请重新输入4位数字." << endl;
                cin.clear();
                cin.ignore(102400,'\n');   //或者用cin.sync();
                cout << "账号: ";
                getline(cin, input_uid);
            }
    return input_uid;
}
// 得到密码
string get_pwd(){
    string password;
    cout << "密码: ";
    getline(cin, password);
    bool isAlnum = true;
    for(auto c : password){
        if(!isalnum(c)){
            isAlnum = false;
        }
    }
    while(password.size() > 16 && !isAlnum) {
                cout << "请重新输入16位及以下位数的密码." << endl;
                cin.clear();
                cin.ignore(102400,'\n');   //或者用cin.sync();
                cout << "密码: ";
                getline(cin, password);
            }
    return password;
}
// 信息交互函数（发送命令并收到回复）
bool Login(TcpSocket cfd_class,int option_flag){
    string input_uid;
    string pwd;
    while(true){
        input_uid = get_uid();  // 读入账号
        pwd = get_pwd();        // 读入密码
        if(option_flag == REGISTER_CHECK){
            cout << "请确认";
            string pwd2 = get_pwd();
            if(pwd != pwd2){
                cout << "前后两次密码输入不一致，请重新注册" << endl;
                continue;
            }else{
                break;
            }
        }else {
            break;
        }
    }
    // 命令包装成command类
    Command command(input_uid, cfd_class.getfd(), LOGHIN_CHECK, pwd);

    cfd_class.sendMsg(command.To_Json());  // 命令类将类转josn格式，再转json字符串格式，套接字类发送json字符串格式

    // 收到操作结果
    string iscorrect = cfd_class.recvMsg();
    if(iscorrect == "incorrect"){
        if(option_flag == LOGHIN_CHECK)
            cout << "账号或密码错误." << endl;
        else
            cout << "用户已存在.";
        return false;
    }else{
        if(option_flag == LOGHIN_CHECK)
            cout << "登录成功." << endl;
        else
            cout << "用户已存在";
        return true;
    }
    return false;
}
