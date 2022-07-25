#include "../lib/TCPSocket.cc"
#include "Display.cc"
#include "../lib/Command.hpp"
#include <sys/types.h>
#include <iostream>
#include <string>
#include <signal.h>

#define LOGIN_OPTIONS 0
#define LOGHIN_CHECK 1

void my_error(const char* errorMsg);
string get_input(int flag);  //得到用户输入的正确内容
bool login(TcpSocket cfd_class);
bool Register(TcpSocket cfd_class);
