#include "client.hpp"

using namespace std;
using json = nlohmann::json;

int main(){
    int ret;
    signal(SIGTSTP,SIG_IGN);  // 忽略 Ctrl Z
    //signal(SIGINT,SIG_IGN);   // 忽略 Ctrl C
    string command;  // 每次发送的命令内容
    TcpSocket cfd_class;  // 本客户端的套接字类
    // 连接服务器
    ret = cfd_class.connectToHost("127.0.0.1", 6666);
    if(ret == -1){
        my_error("connect()");
    }
    // 选择登录、注册、退出操作,并进入不同的函数
    bool isok = false;
    while (!isok){
        display_login1();
        command = get_login();
        if(command == "login"){
            isok = Login(cfd_class,LOGHIN_CHECK);
        } else if(command == "register"){
            isok = Login(cfd_class,REGISTER_CHECK);
        } else {
            cout << "感谢您的使用,再见." << endl;
            return 0;
        }
    }
}
