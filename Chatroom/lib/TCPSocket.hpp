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
    ~TcpSocket();
    int getfd() const { return m_fd; }
    int connectToHost(string ip, unsigned short port);
    int sendMsg(string msg);
    string recvMsg();

private:
    int readn(char* buf, int size);
    int writen(const char* msg, int size);

private:
    int m_fd;	// 通信的套接字
};

#endif