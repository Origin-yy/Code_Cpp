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
         << L_YELLOW "*" << NONE << L_BLUE << "------------未读消息(new)---------" << NONE << L_YELLOW "*" NONE << endl
         << L_YELLOW "*" << NONE << L_BLUE << "------------系统消息(system)------" << NONE << L_YELLOW "*" NONE << endl
         << L_YELLOW "*" << NONE << L_BLUE << "------------聊天(chat-)-----------" << NONE << L_YELLOW "*" NONE << endl
         << L_YELLOW "*" << NONE << L_BLUE << "------------好友列表(list-f)------" << NONE << L_YELLOW "*" NONE << endl
         << L_YELLOW "*" << NONE << L_BLUE << "------------群聊列表(list-g)------" << NONE << L_YELLOW "*" NONE << endl
         << L_YELLOW "*" << NONE << L_BLUE << "------------添加好友/群聊(add-)---" << NONE << L_YELLOW "*" NONE << endl
         << L_YELLOW "*" << NONE << L_BLUE << "------------创建群聊(create-)-----" << NONE << L_YELLOW "*" NONE << endl
         << L_YELLOW "*" << NONE << L_BLUE << "------------屏蔽会话(shield-)-----" << NONE << L_YELLOW "*" NONE << endl
         << L_YELLOW "*" << NONE << L_BLUE << "------------恢复会话(restore-)----" << NONE << L_YELLOW "*" NONE << endl
         << L_YELLOW "*" << NONE << L_BLUE << "------------删除好友(delete-)-----" << NONE << L_YELLOW "*" NONE << endl
         << L_YELLOW "*" << NONE << L_BLUE << "------------同意申请(add-)--------" << NONE << L_YELLOW "*" NONE << endl
         << L_YELLOW "*" << NONE << L_BLUE << "------------拒绝申请(refuse-)-----" << NONE << L_YELLOW "*" NONE << endl
         << L_YELLOW "*" << NONE << L_BLUE << "------------帮助(help)------------" << NONE << L_YELLOW "*" NONE << endl
         << L_YELLOW << "**************chatroom**************" << NONE << endl;
}

#endif
