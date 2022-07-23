#include "server.hpp"
#include <memory>
using namespace std;

int main(){
    ThreadPool<TcpSocket> pool(2,10);
    //int ret;  //检测返回值
    map<int, int> uid_cfd;    //一个uid对应一个cfd
    TcpServer sfd_class;  //创建服务器的socket
    int listenfd = sfd_class.setListen(6666);  //设置监听返回监听符.内部报错
    //创建epoll实例，并把listenfd加进去，监视可读事件
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
                sockaddr_in *addr = NULL;
                TcpSocket* cfd_class = sfd_class.acceptConn(addr);
                cout << "客户端连接成功,ip地址: " << inet_ntoa(addr->sin_addr) << "端口号: " << addr->sin_port << endl;
                pool.addTask(Task<TcpSocket>(loginTask, cfd_class));
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

void loginTask(void *arg){
    TcpSocket *cfd_class = static_cast<TcpSocket*>(arg);
    string option = cfd_class->recvMsg();


}