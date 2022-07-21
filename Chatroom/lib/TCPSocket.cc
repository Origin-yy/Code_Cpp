#include "TCPSocket.hpp"


bool TcpSocket::CreatSockfd(){
    sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP); 
    if(sockfd < 0){
        perror("scoket() failed.\n");
        return false;
    }
    return true;
}
bool TcpSocket::Bind(const string& ip, const int& port){
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

bool TcpSocket::Listen(int backlog){
        int ret = listen(sockfd, BACKLOG);
        if(ret < 0){
            perror("bind() failed.\n");
            return false;
        }
        return true;
}

bool TcpSocket::Connect(const string& ip, const int& port){
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

bool TcpSocket::Accept(TcpSocket* sock, string* ip, int* port){
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

int TcpSocket::write_timeout(string& msg, size_t n) {
    //设置超时时间
    fd_set wSet;
    FD_ZERO(&wSet);
    FD_SET(sockfd, &wSet);

    struct timeval timeout;
    memset(&timeout, 0, sizeof(timeout));
    timeout.tv_sec = TIME;
    timeout.tv_usec = 0;
    // select加超时，阻塞并等待写就绪
    int r;
    while (1) {
        r = select(sockfd + 1, NULL, &wSet, NULL, &timeout);
        if (r < 0) {
            if (errno == EINTR){
                continue;
            }
            return r;
        } else if (r == 0) {
            errno = ETIMEDOUT; //设置errno为超时
            return -1;
        } else {
            break;
        }
    }
    //开写
    int writeNum;
    void* buf = malloc(n+1);
    writeNum = write(sockfd, buf, n);
    msg = static_cast<string>(static_cast<const char*>(buf));
    return writeNum;
}

int TcpSocket::read_timeout(string& msg, size_t n) {
    //设置超时时间
    fd_set rSet;
    FD_ZERO(&rSet);
    FD_SET(sockfd, &rSet);

    struct timeval timeout;
    memset(&timeout, 0, sizeof(timeout));
    timeout.tv_sec = TIME;
    timeout.tv_usec = 0;
    // select加上超时,并阻塞等待读就绪
    int r;
    while (1) {
         r = select(sockfd + 1, &rSet, NULL, NULL, &timeout);
        if (r < 0) {
            if (errno == EINTR){
                continue;
            }
            return r;
        } else if (r == 0) {
            errno = ETIMEDOUT;
            return -1;
        } else {
            break;
        }
    }
    //开读
    int readnum;
    void* buf = malloc(n+1);
    readnum = read(sockfd, buf, n);
    msg = static_cast<string>(static_cast<const char*>(buf));
    return readnum;
}
int TcpSocket::sendmsg(string& msg){
    string buf = msg + to_string(htonl(msg.size()));
    int ret;
    ret = write_timeout(buf, buf.size());
    if(ret < 0){
        if(errno == ETIMEDOUT){
            cout << "Connection is broken" << endl;
        }else{
            perror("sendmsg() error.\n");
            Close();
        }
    }
    return ret;
}
int TcpSocket::recvmsg(string& msg){
    string len;
    int ret = read_timeout(len, sizeof(int));
    int lenth = stoi(len);
    
    ret = read_timeout(msg, lenth);
        if(ret < 0){
        if(errno == ETIMEDOUT){
            cout << "Connection is broken" << endl;
        }else{
            perror("recvmsg() error.\n");
            Close();
        }
    }
    return ret;
}
bool TcpSocket::Close(){
    if(sockfd > 0){
        close(sockfd);
        sockfd = -1;
        return true;
    }else{
        return false;
    }
}