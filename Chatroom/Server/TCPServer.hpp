#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include "../lib/TCPSocket.cc"

class TcpServer
{
public:
    TcpServer();
    ~TcpServer();
    int getfd() const {return m_fd;}
    int setListen(unsigned short port);
    TcpSocket* acceptConn(struct sockaddr_in* addr = nullptr);

private:
    int m_fd;	// 监听的套接字
};

#endif