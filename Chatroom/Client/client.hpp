#include "../lib/TCPSocket.cc"
#include <jsoncpp/json/json.h>

#define LOGIN_OPTIONS 0

string get_input(int flag);  //得到用户输入的正确内容
void login();