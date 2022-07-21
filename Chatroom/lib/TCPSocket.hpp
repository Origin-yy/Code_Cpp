#include <asm-generic/errno.h>
#include <iostream>
#include <string>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#include <arpa/inet.h>
#include <fcntl.h>
#include <cstring>

using namespace std;

#define BACKLOG 5  //listen的第二个参数，listen后为accept就收到connect的连接的数量的上限，之内立刻连接，之外阻塞
#define TIME    5  //read和write的超时时间

class TcpSocket{
    private:
        int sockfd;
    public:
        TcpSocket() : sockfd(-1) {}    //0参构造函数
    
    int GetFd(){
        return sockfd;
    }
    void SetFd(int fd){
        sockfd = fd;
    }
    void SetNoBlock(){
        int flag = fcntl(sockfd, F_GETFL, 0);
        fcntl(sockfd,F_SETFL, flag | O_NONBLOCK);
    }
    
    bool CreatSockfd();
    bool Bind(const string& ip, const int& port);
    bool Listen(int backlog = BACKLOG);
    bool Connect(const string& ip, const int& port);
    bool Accept(TcpSocket* sock, string* ip = NULL, int* port = NULL);

    int write_timeout(string& msg, size_t n);
    int read_timeout(string& msg, size_t n);

    int sendmsg(string& msg);
    int recvmsg(string& msg);
    bool Close();
};
