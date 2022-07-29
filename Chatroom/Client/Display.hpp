#include <iostream>

using namespace std;


// 展示界面的函数
void display_login1(){
    cout << "Welcome to the chat room" << endl
         << "Please select" << endl
         << "login/register/quit" << endl;
}
void display_menu(){
    cout << "                  聊天室                  " << endl;
    cout << "-----------------------------------------" << endl;
    cout << "                1.私聊                    " << endl;
    cout << "                2.群聊                    " << endl;
    cout << "                2.查看好友申请             " << endl;
    cout << "                3.加好友                  " << endl;
    cout << "                4.查看历史聊天记录          " << endl;
    cout << "                5.查询好友信息             " << endl;
    cout << "                6.查看好友列               " << endl;
    cout << "                7.删除好友                 " << endl;
    cout << "                8.刷新                    " << endl;
    cout << "                9.注销账号                 " << endl;
    cout << "                10.群聊                   " << endl;
    cout << "                11.拉入黑名单              " << endl;
    cout << "                12.拉出黑名单              "<< endl;
    cout << "------------------------------------------" << endl;
}