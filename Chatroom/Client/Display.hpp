#ifndef GISPLAY_HPP
#define GISPLAY_HPP
#include <iostream>
#include "../lib/Color.hpp"

using namespace std;

// 展示界面的函数
void display_login(){
    cout << L_YELLOW << "***************聊天室***************" << NONE << endl
         << L_YELLOW "*" << NONE << L_BLUE << "------------登录(login)-----------" << NONE << L_YELLOW "*" NONE << endl
         << L_YELLOW "*" << NONE << L_BLUE << "------------注册(register)--------" << NONE << L_YELLOW "*" NONE << endl
         << L_YELLOW "*" << NONE << L_BLUE << "------------退出(quit)------------" << NONE << L_YELLOW "*" NONE << endl
         << L_YELLOW << "**************chatroom**************" << NONE << endl;
}
void display_menu1(){
    cout << L_YELLOW << "***************聊天室***************" << NONE << endl
         << L_YELLOW "*" << NONE << L_BLUE << "------------消息列表(msg)---------" << NONE << L_YELLOW "*" NONE << endl
         << L_YELLOW "*" << NONE << L_BLUE << "------------聊天(chat-)-----------" << NONE << L_YELLOW "*" NONE << endl
         << L_YELLOW "*" << NONE << L_BLUE << "------------展示列表(list-)-------" << NONE << L_YELLOW "*" NONE << endl
         << L_YELLOW "*" << NONE << L_BLUE << "------------添加好友/群聊(add-)---" << NONE << L_YELLOW "*" NONE << endl
         << L_YELLOW "*" << NONE << L_BLUE << "------------屏蔽好友(shield-)-----" << NONE << L_YELLOW "*" NONE << endl
         << L_YELLOW "*" << NONE << L_BLUE << "------------删除好友(delete-)-----" << NONE << L_YELLOW "*" NONE << endl
         << L_YELLOW "*" << NONE << L_BLUE << "------------同意申请(add-)--------" << NONE << L_YELLOW "*" NONE << endl
         << L_YELLOW "*" << NONE << L_BLUE << "------------帮助(help)------------" << NONE << L_YELLOW "*" NONE << endl
         << L_YELLOW << "**************chatroom**************" << NONE << endl;
}

#endif
