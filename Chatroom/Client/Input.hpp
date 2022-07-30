#include <iostream>
#include <string>
#include "../lib/Command.hpp"

using namespace std;

#define LOGHIN_CHECK    1
#define REGISTER_CHECK  2
#define ADDFRIEND       3
#define ADDGROUP        4
#define AGREEADD        5

// 获取正确输入的函数
// 获得登录选项
string get_login(){
    string command;
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
    cout << "请输入您想进行的操作" << endl;
    cin.sync();
    getline(cin, input);
    while(true){
        if(input.find("add-friend-") == 0 && input.size() >= 15){
            string option1(input.begin()+11, input.begin()+15);
            if(option1 == my_uid){
                cout << "不允许添加自己" << endl;
                cout << "请输入您想进行的操作:" << endl;
                cin.sync();
                getline(cin, input);
                continue;
            }else if(input.size() > 16 && input[15] == '-'){
                string option2(input.begin()+16, input.end()); 
                Command command(my_uid, ADDFRIEND, {option1, option2});
                return command;
            }else{
                Command command(my_uid, ADDFRIEND, {option1, "无"});
                return command;
            }
        }else if(input.find("agree-") == 0){
            string option1(input.begin()+6,input.end());
            Command command(my_uid, AGREEADD, {option1});
            return command;
        }
        
        
        
        
        
        
        else{
            cout << "无效的操作，请重新输入." << endl;
            cout << "请输入您想进行的操作:" << endl;
            cin.sync();
            getline(cin, input);
            continue;
        }
    }
}