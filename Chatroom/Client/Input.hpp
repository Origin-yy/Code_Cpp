#include <iostream>
#include <string>
#include "../lib/Command.hpp"
#include "../lib/Color.hpp"

using namespace std;

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
#define NEWMESSAGE        12

string get_login();
string get_uid();
string get_pwd();
Command get_command(string my_uid);

// 获取正确输入的函数
// 获得登录选项
string get_login(){
    string command;
    cout << L_WHITE << "请选择您想进行的操作: " << NONE << endl;
    getline(cin,command);
    if(cin.eof()){
        cout << "读到EOF" << endl;
        return "EOF";
    }
    while(command != "login" && command != "register" && command != "quit") {
        cout << "请输入login、register or quit." << endl;
        //cin.clear(); cin做if判断时用，把无效的cin改成有效的，不用的话，第一次cin无效后一直无效，死循环
        cin.sync();  //清除输入缓冲区中所有内容
        getline(cin,command);
    }
    return command;
}

// 得到格式正确的uid
string get_uid(){
    // 读入输入格式正确的账号
    string input_uid;
    cout << "账号: ";
    cin.sync(); 
    getline(cin, input_uid);
    bool noNum = false;
    for(auto c : input_uid){
        if(!isdigit(c)){
            noNum = true;
        }
    }
    while(input_uid.size() != 4 || noNum) {
                //cin.clear();cin做if判断时用，把无效的cin改成有效的，不用的话，第一次cin无效后一直无效，死循环
                cin.sync();  // cin.ignore(102400,'\n'); 
                cout << "请重新输入4位数字." << endl << "账号: ";;
                getline(cin, input_uid);
                noNum = false;
                for(auto c : input_uid){
                    if(!isdigit(c)){
                        noNum = true;
                    }
                }
    }
    return input_uid;
}

// 得到密码
string get_pwd(){
    string password;
    cout << "密码: ";
    cin.sync(); 
    getline(cin, password);
    bool noAlnum = false;
    for(auto c : password){
        if(!isalnum(c)){
            noAlnum = true;
        }
    }
    while(password.size() > 16 || password.size() == 0 || noAlnum) {
                //cin.clear();cin做if判断时用，把无效的cin改成有效的，不用的话，第一次cin无效后一直无效，死循环
                cin.sync();   // cin.ignore(102400,'\n'); 
                cout << "请重新输入16位及以下位数的密码(由数字和字母组成)." << endl << "密码: ";
                getline(cin, password);
                noAlnum = false;
                for(auto c : password){
                    if(!isdigit(c)){
                        noAlnum = true;
                    }
                }
    }
    return password;
}

Command get_command(string my_uid){
    string input;
    cout << L_WHITE << "请选择您想进行的操作: " << NONE << endl;
    cin.sync();
    getline(cin, input);
    //  循环判断输入是否合法，不合法要求再次输入并判断
    while(true){
        // 添加好友命令是否合法
        if(input.find("add-") == 0 && input.size() >= 8){
            string option0(input.begin()+4, input.begin()+8);
            if(option0 == my_uid){
                cout << "不允许添加自己" << endl;
                cout << "请输入您想进行的操作:" << endl;
                cin.sync();
                getline(cin, input);
                continue;
            }else if(input.size() > 9 && input[8] == '-'){
                string option1(input.begin()+8, input.end()); 
                Command command(my_uid, ADDFRIEND, {option0, option1});
                return command;
            }else if(input.size() > 109){
                cout << "验证消息最多100个字节" << endl;
                cout << "请输入您想进行的操作:" << endl;
                cin.sync();
                getline(cin, input);
                continue;
            }
            else{
                Command command(my_uid, ADDFRIEND, {option0, "无"});
                return command;
            }
        }
        // // 添加群聊命令是否合法
        // else if(input.find("add-group-") == 0 && input.size() >= 14){
        //     string option0(input.begin()+10, input.begin()+14);
        //     if(input.size() > 14 && input[13] == '-'){
        //         string option1(input.begin()+14, input.end()); 
        //         Command command(my_uid, ADDGROUP, {option0, option1});
        //         return command;
        //     }else{
        //         Command command(my_uid, ADDGROUP, {option0, "无"});
        //         return command;
        //     }
        // }
        // 同意好友申请命令是否合法
        else if(input.find("agree-") == 0 && input.size() ==10){
            string option0(input.begin() + 6,input.end());
            Command command(my_uid, AGREEADDFRIEND, {option0});
            return command;
        }
        // 展示好友列表命令是否合法
        else if(input == "list-f"){ 
            string option0 = "list-f";
            Command command(my_uid, LISTFRIEND, {option0});
            return command;
        }
        // // 展示群组列表命令是否合法
        // else if(input == "list-group"){
        //     string option0 = "list-group";
        //     Command command(my_uid, LISTGROUP, {option0});
        //     return command;
        // }
        // 
        else if(input.find("chat-") == 0 && input.size() > 5){
            string option0(input.begin() + 5, input.end());
            Command command(my_uid, CHATFRIEND, {option0});
            return command;
        }

        else if(input.find("shield-") == 0 && input.size() > 7){
            string option0(input.begin() + 7, input.end());
            Command command(my_uid, SHIELDFRIEND, {option0});
            return command;
        }
        else if(input.find("delete-") == 0 && input.size() > 7){
            string option0(input.begin() + 7, input.end());
            Command command(my_uid, DELETEFRIEND, {option0});
            return command;
        }
        else if(input.find("restore-") == 0 && input.size() > 8){
            string option0(input.begin() + 8, input.end());
            Command command(my_uid, DELETEFRIEND, {option0});
            return command;
        }
        else if(input.find("new") == 0 && input.size() == 3){
            string option0 = input;
            Command command(my_uid, DELETEFRIEND, {option0});
            return command;
        }





        // 退出命令是否合法
        else if(input == "quit"){
            Command command(my_uid, QUIT, {"无"});
                return command;
        }else{
            cout << "无效的操作，请重新输入." << endl;
            cout << "请输入您想进行的操作:" << endl;
            cin.sync();
            getline(cin, input);
            continue;
        }
    }
}