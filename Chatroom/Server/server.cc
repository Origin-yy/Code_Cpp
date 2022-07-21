#include "server.hpp"
#include <cerrno>
#include <cstdio>
#include <netinet/in.h>
#include <sys/epoll.h>

using namespace std;

void my_error(const char* errorMsg);

int main(){
    int ret;  //检测返回值
    TcpServer sfd;  //创建服务器的socket
    int listenfd = sfd.setListen(6666);  //设置监听返回监听符.内部报错
    //创建epoll实例，并把sfd加进去，监视可读事件
    int epfd = epoll_create(5);
    if(epfd == -1){
        my_error("epoll() failed.");
    }
    struct epoll_event temp,ep[1024];
    temp.data.fd = listenfd;
    temp.events = EPOLLIN;
    epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &temp);

    while(true){
        int readyNum = epoll_wait(epfd, ep, 1024, -1);
        for (int i = 0; i < readyNum; i++){
            if(ep[i].data.fd == listenfd){

            }
        }
        
    }
    return 0;
}

void my_error(const char* errorMsg){
    cout << errorMsg << endl;
    strerror(errno);
    exit(1);
}