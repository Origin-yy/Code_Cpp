#include <iostream>
#include "tcpsocket.hpp"
 
#define CHECK_RET(q) if(q==false){return -1;}
 
int main(int argc,char* argv[]){
    TcpSocket lis_sock;
    string ip = "0x00000000";
    int port = atoi("60000");
    //创建套接字
    CHECK_RET(lis_sock.Socket());
    //绑定地址信息
    CHECK_RET(lis_sock.Bind(ip,port));
    //监听
    CHECK_RET(lis_sock.Listen());
    while(1){
        cout << "here" << endl;
        TcpSocket srv_sock;  
        string cli_ip;
        int cli_port;
        //获取新连接套接字
        bool ret = lis_sock.Accept(&srv_sock,&cli_ip,&cli_port);
        if(ret == false){
            continue;
        }
        //收发数据
        string buf;
        ret = srv_sock.Recv(&buf);
        if(ret == false){
            srv_sock.Close();
            continue;
        }
        printf("client[ %s : %d ] say %s:\n",cli_ip.c_str(),cli_port,buf.c_str());
        buf.clear();
        cin >> buf;
        ret = srv_sock.Send(buf);
        if(ret == false){
            srv_sock.Close();
        }
    }
    //关闭套接字连接
    CHECK_RET(lis_sock.Close());
    return 0;
}