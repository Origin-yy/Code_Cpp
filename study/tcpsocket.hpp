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
 
class TcpSocket{
private:
    int _sockfd;
public:
    TcpSocket()
        :_sockfd(-1)
    {}
    bool Socket(){
        _sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
        if(_sockfd < 0){
            perror("socket error");
            return false;
        }
        return true;
    }
    int GetFd(){
        return _sockfd;
    }
    void SetFd(int fd){
        _sockfd = fd;
    }
    void SetNoBlock(){
        int flag = fcntl(_sockfd,F_GETFL,0);
        fcntl(_sockfd,F_SETFL,flag|O_NONBLOCK);
    }
    bool Bind(const string& ip,const int& port){
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(ip.c_str());
        socklen_t len = sizeof(struct sockaddr_in);
        int ret = bind(_sockfd,(struct sockaddr*)&addr,len);
        if(ret < 0){
            perror("bind error");
            return false;
        }
        return true;
    }
    bool Listen(int backlog = 5){
        int ret = listen(_sockfd,backlog);
        if(ret < 0){
            perror("listen error");
            return false;
        }
        return true;
    }
    bool Connect(const string& ip,const int& port){
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(ip.c_str());
        socklen_t len = sizeof(struct sockaddr_in);
        int ret = connect(_sockfd,(struct sockaddr*)&addr,len);
        if(ret < 0){
            perror("connect error");
            return false;
        }
        return true;
    }
 
    bool Accept(TcpSocket* sock,string* ip = NULL,int* port = NULL){
        struct sockaddr_in addr;
        socklen_t len = sizeof(struct sockaddr_in);
        int fd = accept(_sockfd,(struct sockaddr*)&addr,&len);
        if(fd < 0){
            perror("accept error");
            return false;
        }
        sock->_sockfd = fd;
        if(ip != NULL){
            *ip = inet_ntoa(addr.sin_addr);
        }
        if(port != NULL){
            *port = ntohs(addr.sin_port);
        }
        return true;
    }
    bool Recv(string* buf){
        char tmp[4096]={0};
        int ret = recv(_sockfd,tmp,4096,0);
        if(ret < 0){
            perror("recv error");
            return false;
        }else if(ret ==0){
            printf("connect shutdown");
            return false;
        }
        buf->assign(tmp,ret);
        return true;
    }
    bool Send(const string& data){
        int total_len = 0; //实际发送的数据长度
        while(total_len < data.size()){
            int ret = send(_sockfd,data.c_str()+total_len,data.size()-total_len,0);
            if(ret < 0){
                perror("send error");
                return false;
            }
            total_len+=ret;
        }
        return true;
    }
    bool Close(){
        if(_sockfd > 0){
            close(_sockfd);
            _sockfd = -1;
            return true;
        }else{
            return false;
        }
    }
};