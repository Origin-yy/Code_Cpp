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

using namespace std;

#define BACKLOG 5  //listen的第二个参数，listen后为accept就收到connect的连接的数量的上限，之内立刻连接，之外阻塞

class TcpSocket{
    private:
        int sockfd;
    public:
        TcpSocket() : sockfd(-1) {}    //0参构造函数
    
    bool CreatSockfd(){
        sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP); 
        if(sockfd < 0){
            perror("scoket() failed.\n");
            return false;
        }
        return true;
    }
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
    bool Bind(const string& ip, const int& port){
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        inet_pton(AF_INET, ip.c_str(), &addr.sin_addr.s_addr);
        socklen_t len = sizeof(struct sockaddr_in);
        int ret = bind(sockfd, (struct sockaddr*)&addr, len);
        if(ret < 0){
            perror("bind() failed.\n");
            return false;
        }
        return true;
    }
    bool Listen(int backlog = BACKLOG){
        int ret = listen(sockfd, BACKLOG);
        if(ret < 0){
            perror("bind() failed.\n");
            return false;
        }
        return true;
    }
    bool Connect(const string& ip, const int& port){
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        //ipv4地址点分十进制格式转二进制格式
        inet_pton(AF_INET, ip.c_str(), &addr.sin_addr.s_addr);  
        socklen_t len = sizeof(struct sockaddr_in);
        int ret = connect(sockfd, (struct sockaddr*)&addr, len);
        if(ret < 0){
            perror("connect() failed.\n");
            return false;
        }
        return true;
    }
    bool Accept(TcpSocket* sock, string* ip = NULL, int* port = NULL){
        struct sockaddr_in addr;
        socklen_t len = sizeof(struct sockaddr_in);
        int fd = accept(sockfd,(struct sockaddr*)&addr,&len);
        if(fd < 0){
            perror("accept() failed.\n");
            return false;
        }
        sock->sockfd = fd;
        if(ip != NULL){
            char* dst_str;
            //ipv4地址二进制格式转点分十进制格式
            inet_ntop(AF_INET, &addr.sin_addr.s_addr, dst_str, INET_ADDRSTRLEN);
            (*ip) = dst_str;
        }
        if(port != NULL){
            *port = ntohs(addr.sin_port);
        }
        return true;
    }
    ssize_t readn(void* buf, size_t n){
        ssize_t numread;
        size_t totread;
        char *p = static_cast<char*>(buf);
        for(totread = 0; totread < n;){
            numread = read(sockfd, p, n - totread);
            if(numread == -1){
                if(errno == EINTR)
                    continue;
                else
                    return -1;
            }
            totread += numread;
            p += numread;
        }
        return totread;
    }
    ssize_t writen(int fd, const void *buf, size_t n) {
        ssize_t numwriten;
        size_t totwriten;
        const char *p = static_cast<const char*>(buf);
        for (totwriten = 0; totwriten < n;) {
            numwriten = write(fd, p, n - totwriten);

            if (numwriten <= 0) {
            if (numwriten == -1 && errno == EINTR)
                continue;
            else
                return -1;
        }
        totwriten += numwriten;
        p += numwriten;
        }
        return totwriten;
    }
    int recvMsg(string& buf){
        int len = 0;
        readn((char*)&len, sizeof(int));
        len = ntohl(len);
        printf("接收到的 数据块大小 %d\n",len);
        int Len = readn(data,len);
        if(Len == 0)
        { 
            printf("对方断开链接\n");
            close(sockfd);    
        }
        else if(len != Len)
        {
            printf("数据接收失败\n");
        }
        data[len] = '\0';
        *msg = data;
        return Len;
    }




    bool Close(){
        if(sockfd > 0){
            close(sockfd);
            sockfd = -1;
            return true;
        }else{
            return false;
        }
    }
};
