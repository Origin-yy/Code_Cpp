#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include "TCPSocket.hpp"

class TcpServer
{
public:
    TcpServer();
    ~TcpServer();
    int setListen(unsigned short port);
    TcpSocket* acceptConn(struct sockaddr_in* addr = nullptr);

private:
    int m_fd;	// 监听的套接字
};

#endif