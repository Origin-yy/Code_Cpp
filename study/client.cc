#include "tcpsocket.hpp"
 
 
#define CHECK_RET(q) if(q==false){return -1;}
 
int main(){
    TcpSocket cli_sock;
    //创建套接字
    CHECK_RET(cli_sock.Socket());
    //建立连接请求
    CHECK_RET(cli_sock.Connect("127.0.0.1",60000));
    //进行收发数据
    while(1){
        cout << "while开始" << endl;
        string data;
        cin >> data;
        CHECK_RET(cli_sock.Send(data));
        data.clear();
        CHECK_RET(cli_sock.Recv(&data));
        printf("server say:%s",data.c_str());
    }
    //关闭连接
    CHECK_RET(cli_sock.Close());
    return 0;
}