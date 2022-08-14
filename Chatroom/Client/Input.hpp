#include <iostream>
#include <iterator>
#include <string>
#include "../lib/Command.hpp"
#include "../lib/Color.hpp"
#include "Display.hpp"

using namespace std;
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
#define LOOKSYSTEM      13
#define REFUSEADDFRIEND 14
#define CREATEGROUP     15
#define LISTGROUP       16
#define ADDGROUP        17
#define ADD             18
#define LOOKNOTICE      19
#define ABOUTGROUP      20
#define REQUSTLIST      21
#define PASSAPPLY       22
#define DENYAPPLY       23
#define SETMEMBER       24
#define EXITGROUP       25
#define DISPLAYMEMBER   26
#define REMOVEMEMBER    27
#define INFOXXXX        28
#define CHATGROUP       29
#define GROUPMSG        30
#define EXITGROUPCHAT   31
string get_login();
string get_uid();
string get_pwd();
Command get_command(string my_uid);

string get_login(){
    string command;
    cout << L_WHITE << "就决定是你了：" << NONE << endl;
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
    cout << L_WHITE << "就决定是你了：" << NONE << endl;
    cin.sync();
    getline(cin, input);
    //  循环判断输入是否合法，不合法要求再次输入并判断
    while(true){
        // 是添加好友/群聊的命令
        if(input.find("add-") == 0 && input.size() >= 7){
            // 命令长度为7的话，就是没有验证的加群
            if(input.size() == 7){
                string option0(input.begin() + 4, input.begin() + 7);
                string option1("无"); 
                Command command(my_uid, ADDGROUP, {option0, option1});
                return command;
            }
            // 7位置上为‘-’,说明是加群，可能有验证，也可能没验证
            if(input[7] == '-'){
                // 长度大于8，说明有验证
                if(input.size() > 8){
                    string option0(input.begin() + 4, input.begin() + 7);
                    string option1(input.begin() + 8, input.end()); 
                    if(option1.size() > 100){
                        cout << "验证消息最多100个字节." << endl;
                        cout << "就决定是你了：" << endl;
                        cin.sync();
                        getline(cin, input);
                        continue;
                    }
                    for(auto c :option1){
                        if(c == ' ' || c == '\n' || c == '\t' ){
                            cout << "验证消息不能包含空白字符." << endl;
                            cout << "就决定是你了：" << endl;
                            cin.sync();
                            getline(cin, input);
                            continue;
                        }
                    }
                    Command command(my_uid, ADDGROUP, {option0, option1});
                    return command;
                }// 长度大于7但不小于8，说明长度等于8，其7上为‘-’,带了‘-’但无验证
                else{ 
                    string option0(input.begin() + 4, input.begin() + 7);
                    Command command(my_uid, ADDGROUP, {option0, "无"});
                    return command;
                }
            }
            // 排除以上情况，说明是加好友
            string option0(input.begin() + 4, input.begin() + 8);  // 存下要添加的账号
            if(option0 == my_uid){
                cout << "不允许添加自己" << endl;
                cout << "就决定是你了：" << endl;
                cin.sync();
                getline(cin, input);
                continue;
            }
            // 有验证消息
            if(input.size() > 9 && input[8] == '-'){
                string option1(input.begin() + 9, input.end()); 
                if(option1.size() > 100){
                    cout << "验证消息最多100个字节" << endl;
                    cout << "就决定是你了：" << endl;
                    cin.sync();
                    getline(cin, input);
                    continue;
                }
                for(auto c :option1){
                        if(c == ' '){
                            cout << "验证消息不能包含空白字符." << endl;
                            cout << "就决定是你了：" << endl;
                            cin.sync();
                            getline(cin, input);
                            continue;
                        }
                    }
                Command command(my_uid, ADDFRIEND, {option0, option1});
                return command;
            }// 没有验证消息
            else{
                Command command(my_uid, ADDFRIEND, {option0, "无"});
                return command;
            }
        }
        else if(input.find("agree-") == 0 && input.size() ==10){
            string option0(input.begin() + 6,input.end());
            Command command(my_uid, AGREEADDFRIEND, {option0});
            return command;
        }
        else if(input == "list-f"){ 
            string option0 = "list-f";
            Command command(my_uid, LISTFRIEND, {option0});
            return command;
        }
        else if(input == "list-g"){
            string option0 = "list-g";
            Command command(my_uid, LISTGROUP, {option0});
            return command;
        }
        else if(input.find("chat-") == 0 && input.size() == 9){
            string option0(input.begin() + 5, input.end());
            Command command(my_uid, CHATFRIEND, {option0});
            return command;
        }
        else if(input.find("chat-") == 0 && input.size() == 8){
            string option0(input.begin() + 5, input.end());
            Command command(my_uid, CHATGROUP, {option0});
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
            Command command(my_uid, RESTOREFRIEND, {option0});
            return command;
        }
        else if(input.find("message") == 0 && input.size() == 7){
            string option0 = input;
            Command command(my_uid, NEWMESSAGE, {option0});
            return command;
        }
        else if(input.find("system") == 0 && input.size() == 6){
            string option0 = input;
            Command command(my_uid, LOOKSYSTEM, {option0});
            return command;
        }
        else if(input.find("notice") == 0 && input.size() == 6){
            string option0 = input;
            Command command(my_uid, LOOKNOTICE, {option0});
            return command;
        }else if(input.find("refuse-") == 0 && input.size() > 7){
            string option0(input.begin() + 7,input.end());
            Command command(my_uid, REFUSEADDFRIEND, {option0});
            return command;
        }else if(input == "create"){
            string option0 = input;
            Command command(my_uid, CREATEGROUP, {option0});
            return command;
        }else if(input.find("about-") == 0 && input.size() == 9){
            string option0(input.begin() + 6, input.end());
            Command command(my_uid, ABOUTGROUP, {option0});
            return command;
        }else if(input == "menu"){
            display_menu1();
            cout << "就决定是你了：" << endl;
            cin.sync();
            getline(cin, input);
            continue;
        }else if(input == "help"){
            display_help();
            cout << "就决定是你了：" << endl;
            cin.sync();
            getline(cin, input);
            continue;
        }
        else if(input == "quit"){
            Command command(my_uid, QUIT, {"无"});
                return command;
        }else{
            cout << "无效的操作，请重新输入." << endl;
            cout << "就决定是你了：" << endl;
            cin.sync();
            getline(cin, input);
            continue;
        }
    }
}