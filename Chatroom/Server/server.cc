#include "server.hpp"

using namespace std;
using json = nlohmann::json;

int main(){
    int ret;  // 检测返回值
    ThreadPool<Command> pool(2,10);   // 创建一个线程池类
    map<int, int> uid_cfd;    // 一个uid对应一个cfd的表
    TcpServer sfd_class;  // 创建服务器的socket

    ret = sfd_class.setListen(6666);  //设置监听返回监听符.内部报错
    if(ret == -1) {exit(1);}

    // 创建epoll实例，并把listenfd加进去，监视可读事件
    int epfd = epoll_create(5);
    if(epfd == -1) { exit(1); }
    struct epoll_event temp,ep[1024];
    temp.data.fd = sfd_class.getfd();
    temp.events = EPOLLIN;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, sfd_class.getfd(), &temp);
    if(ret == -1) {
        my_error("epoll_ctl() failed.");
    }
    // 循环监听自己的符看是否有连接请求，监听客户端的符看是否有消息需要处理
    while(true) {
        int readyNum = epoll_wait(epfd, ep, 1024, -1);  // 有几个符就绪了
        for (int i = 0; i < readyNum; i++){  // 对于ep中每个就绪的符
             // 如果是服务器的符，说明新客户端连接，接入连接并把客户端的符扔进epoll
            if(ep[i].data.fd == sfd_class.getfd()) { 
                TcpSocket* cfd_class = sfd_class.acceptConn(NULL);
                temp.data.fd = cfd_class->getfd();
                temp.events = EPOLLIN;
                epoll_ctl(epfd,EPOLL_CTL_ADD,cfd_class->getfd(),&temp);
            }// 如果是客户端的符，就运行任务函数
            else {
                Command* command;
                pool.addTask(Task<Command>(&taskfunc,static_cast<void*>(command)));
            }
        }
        
    }
    return 0;
}

void my_error(const char* errorMsg) {
    cout << errorMsg << endl;
    strerror(errno);
    exit(1);
}
//任务函数，获取客户端发来的命令，解析命令进入不同模块，并进行回复
void taskfunc(void * arg){

}