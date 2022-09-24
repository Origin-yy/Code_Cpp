#ifndef GISPLAY_HPP
#define GISPLAY_HPP
#include <iostream>
#include "../lib/Color.hpp"

using namespace std;

// 展示界面的函数
void display_login(){
    cout<< L_YELLOW << "***************聊天室***************" << NONE << endl
        << L_YELLOW "*" << NONE << L_BLUE << "------------登录(login)-----------" << NONE << L_YELLOW "*" NONE << endl
        << L_YELLOW "*" << NONE << L_BLUE << "------------注册(register)--------" << NONE << L_YELLOW "*" NONE << endl
        << L_YELLOW "*" << NONE << L_BLUE << "------------退出(quit)------------" << NONE << L_YELLOW "*" NONE << endl
        << L_YELLOW << "**************chatroom**************" << NONE << endl;
}
void display_menu1(){
    cout<< L_YELLOW << "***************聊天室***************" << NONE << endl
        << L_YELLOW "*" << NONE << L_WHITE << "--------------通用----------------" << NONE << L_WHITE "*" NONE << endl
        << L_YELLOW "*" << NONE << L_BLUE << "------------未读消息(message)-----" << NONE << L_YELLOW "*" NONE << endl
        << L_YELLOW "*" << NONE << L_BLUE << "------------系统消息(system)------" << NONE << L_YELLOW "*" NONE << endl
        << L_YELLOW "*" << NONE << L_BLUE << "------------系统通知(notice)------" << NONE << L_YELLOW "*" NONE << endl
        << L_YELLOW "*" << NONE << L_BLUE << "------------聊天(chat-)-----------" << NONE << L_YELLOW "*" NONE << endl
        << L_YELLOW "*" << NONE << L_BLUE << "------------好友列表(list-f)------" << NONE << L_YELLOW "*" NONE << endl
        << L_YELLOW "*" << NONE << L_BLUE << "------------群聊列表(list-g)------" << NONE << L_YELLOW "*" NONE << endl
        << L_YELLOW "*" << NONE << L_BLUE << "------------添加好友/群聊(add-)---" << NONE << L_YELLOW "*" NONE << endl
        << L_YELLOW "*" << NONE << L_BLUE << "------------创建群聊(create)------" << NONE << L_YELLOW "*" NONE << endl
        << L_YELLOW "*" << NONE << L_BLUE << "------------关于群聊(about)-------" << NONE << L_YELLOW "*" NONE << endl
        << L_YELLOW "*" << NONE << L_BLUE << "------------功能菜单(menu)--------" << NONE << L_YELLOW "*" NONE << endl
        << L_YELLOW "*" << NONE << L_BLUE << "------------帮助(help)------------" << NONE << L_YELLOW "*" NONE << endl

        << L_YELLOW "*" << NONE << L_WHITE << "-------------好友相关-------------" << NONE << L_WHITE "*" NONE << endl
        << L_YELLOW "*" << NONE << L_BLUE << "------------屏蔽会话(shield-)-----" << NONE << L_YELLOW "*" NONE << endl
        << L_YELLOW "*" << NONE << L_BLUE << "------------恢复会话(restore-)----" << NONE << L_YELLOW "*" NONE << endl
        << L_YELLOW "*" << NONE << L_BLUE << "------------删除好友(delete-)-----" << NONE << L_YELLOW "*" NONE << endl
        << L_YELLOW "*" << NONE << L_BLUE << "------------同意申请(agree-)------" << NONE << L_YELLOW "*" NONE << endl
        << L_YELLOW "*" << NONE << L_BLUE << "------------拒绝申请(refuse-)-----" << NONE << L_YELLOW "*" NONE << endl

        << L_YELLOW << "**************chatroom**************" << NONE << endl;
}
void display_help(){

}
void display_menu2(){
        cout<< L_YELLOW << "***************聊天室***************" << NONE << endl
        << L_YELLOW "*" << NONE << L_WHITE << "-------------群聊相关-------------" << NONE << L_WHITE "*" NONE << endl
        << L_YELLOW "*" << NONE << L_BLUE << "------------成员列表(members)-----" << NONE << L_YELLOW "*" NONE << endl
        << L_YELLOW "*" << NONE << L_BLUE << "------------申请列表(request)-----" << NONE << L_YELLOW "*" NONE << endl
        << L_YELLOW "*" << NONE << L_BLUE << "------------通过申请(pass-)-------" << NONE << L_YELLOW "*" NONE << endl
        << L_YELLOW "*" << NONE << L_BLUE << "------------拒绝申请(deny-)-------" << NONE << L_YELLOW "*" NONE << endl
        << L_YELLOW "*" << NONE << L_BLUE << "------------设置权限(set-x-x)-----" << NONE << L_YELLOW "*" NONE << endl
        << L_YELLOW "*" << NONE << L_BLUE << "------------移出成员(remove-)-----" << NONE << L_YELLOW "*" NONE << endl
        << L_YELLOW "*" << NONE << L_BLUE << "------------退出群聊(exit)--------" << NONE << L_YELLOW "*" NONE << endl
        << L_YELLOW << "**************chatroom**************" << NONE << endl;
}
#endif
