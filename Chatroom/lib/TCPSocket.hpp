#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H

#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

using namespace std;

class TcpSocket
{
public:
    TcpSocket();
    TcpSocket(int socket);
    TcpSocket(string recv);
    ~TcpSocket();
    int getfd() const { return m_fd; }
    int getrecvfd() const { return recv_fd; }
    int connectToHost(string ip, unsigned short port);
    int sendMsg(string msg);
    string recvMsg();
private:
    int readn(char* buf, int size);
    int writen(const char* msg, int size);
private:
    int m_fd = -1;     // 通信的套接字
    int recv_fd = -1;  // 接收提示消息的套接字
};

#endif