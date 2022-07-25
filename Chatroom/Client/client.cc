#include "client.hpp"
using json = nlohmann::json;

using namespace std;

int main(){
    int ret;
    signal(SIGTSTP,SIG_IGN);  // 忽略 Ctrl Z
    signal(SIGINT,SIG_IGN);   // 忽略 Ctrl C
    string command;  // 每次发送的命令内容
    TcpSocket cfd_class;  // 本客户端的套接字类
    // 连接服务器
    ret = cfd_class.connectToHost("127.0.0.1", 6666);
    if(ret == -1){
        my_error("connect()");
    }
    // 选择登录、注册、退出操作,并进入不同的函数
    bool isok = false;
    while (isok){
        display_login1();
        command = get_input(LOGIN_OPTIONS);
        if(command == "login"){
            isok = login(cfd_class);
        } else if(command == "register"){
            isok = Register(cfd_class);
        } else {
            cout << "感谢您的使用,再见." << endl;
            return 0;
        }
    }
}

void my_error(const char* errorMsg){
    cout << errorMsg << endl;
    strerror(errno);
    exit(1);
}

string get_input(int flag){
    string command;
    getline(cin,command);
    switch (flag) {
        case LOGIN_OPTIONS:
            while(command != "login" && command != "register" && command != "quit") {
                cout << "请输入login、register or quit." << endl;
                //cin.clear(); cin做if判断时用，把无效的cin改成有效的，不用的话，第一次cin无效后一直无效，死循环
                cin.sync();  //清除输入缓冲区中所有内容
                cin >>command;
            }
            break;
        // case :
        //     break;
        // case :
        //     break;
        // case :
        //     break;
        // case :
        //     break;
        // case :
        //     break;
        // case :
        //     break;
        // case :
        //     break;
    }
    return command;
}

bool login(TcpSocket cfd_class){
    // 读入输入格式正确的账号
    string input_uid;
    cout << "账号: ";
    cin >> input_uid;
    bool isNum = true;
    for(auto c : input_uid){
        if(!isdigit(c)){
            isNum = false;
        }
    }
    while(input_uid.size() != 11 && !isNum) {
                cout << "请重新输入11位数字." << endl;
                cin.clear();
                cin.ignore(102400,'\n');   //或者用cin.sync();
                cout << "账号: ";
                cin >> input_uid;
            }
    // 读入输入格式正确的密码
    string password;
    cout << "密码: ";
    cin >> password;
    bool isAlnum = true;
    for(auto c : password){
        if(!isalnum(c)){
            isAlnum = false;
        }
    }
    while(password.size() > 16 && !isAlnum) {
                cout << "请重新输入16位及以下位数的密码." << endl;
                cin.clear();
                cin.ignore(102400,'\n');   //或者用cin.sync();
                cout << "密码: ";
                cin >> password;
            }
    // 发送账号和密码，接收服务器回信，判断账号密码是否正确
    Command command;
    command.flag = LOGHIN_CHECK;
    command.uid = 0;
    command.option.push_back(input_uid);
    command.option.push_back(password);
    command.Cfd_class = cfd_class;

    if(iscorrect == "incorrect"){
        cout << "用户名或密码错误." << endl;
        return false;
    }else{
        cout << "登陆成功." << endl;
        return true;
    }
}

bool Register(TcpSocket cfd_class){

}