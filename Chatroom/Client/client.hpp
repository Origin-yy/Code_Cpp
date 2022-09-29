#include "../lib/TCPSocket.hpp"
#include "Display.hpp"
#include "Input.hpp"
#include <cstdio>
#include <cstring>
#include <iostream>
#include <pthread.h>
#include <sys/types.h>
#include <string>
#include <signal.h>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <sys/sendfile.h>

void my_error(const char* errorMsg);
void *recvfunc(void* arg);
void Quit();
string Login(TcpSocket cfd_class);
bool Register(TcpSocket cfd_class);
void Quit(TcpSocket cfd_class);
bool AddFriend(TcpSocket cfd_class, Command command);
bool AddGroup(TcpSocket cfd_class, Command command);
bool AgreeAddFriend(TcpSocket cfd_class, Command command);
bool ListFirHistory(TcpSocket cfd_class, Command command);
bool ChatFriend(TcpSocket cfd_class, Command command);
bool ChatGroup(TcpSocket cfd_class, Command command);
bool ExitChatFriend(TcpSocket cfd_class, Command command);
bool ExitChatGroup(TcpSocket cfd_class, Command command);
bool ShieldFriend(TcpSocket cfd_class, Command command);
bool DeleteFriend(TcpSocket cfd_class, Command command);
bool Restorefriend(TcpSocket cfd_class, Command command);
bool NewMessage(TcpSocket cfd_class, Command command);
bool LookSystem(TcpSocket cfd_class, Command command);
bool LookNotice(TcpSocket cfd_class, Command command);
bool RefuseAddFriend(TcpSocket cfd_class, Command command);
bool CreateGroup(TcpSocket cfd_class, Command command);
bool ListGroup(TcpSocket cfd_class, Command command);
bool AboutGroup(TcpSocket cfd_class, Command command);
bool RequestList(TcpSocket cfd_class, Command command);
bool PassApply(TcpSocket cfd_class, Command command);
bool DenyApply(TcpSocket cfd_class, Command command);
bool SetMember(TcpSocket cfd_class, Command command);
bool ExitGroup(TcpSocket cfd_class, Command command);
bool DisplyMember(TcpSocket cfd_class, Command command);
bool RemoveMember(TcpSocket cfd_class, Command command);
bool InfoXXXX(TcpSocket cfd_class, Command command);

struct RecvArg{
    string myuid;
    int recv_fd = -1;
    RecvArg(string Myuid, int Recv_uid) : myuid(Myuid),recv_fd(Recv_uid) {} 
};
void my_error(const char* errorMsg){
    cout << errorMsg << endl;
    strerror(errno);
    exit(1);
}
void *recvfunc(void* arg){
    RecvArg *recv_arg = static_cast<RecvArg*>(arg);
    TcpSocket recv_class(recv_arg->recv_fd);
    recv_class.connectToHost("127.0.0.1", 6666);
    Command command(recv_arg->myuid, SETRECVFD, {"空"});
    int ret = recv_class.sendMsg(command.To_Json());
    if(ret == 0 || ret == -1){
        delete recv_arg;
        cout << "服务器已关闭" << endl;
        exit(0);
    }
    while(true){
        string message = recv_class.recvMsg();
        if(message == "close" || message == "-1"){
            cout << "通知套接字已关闭" << endl;
            delete recv_arg;
            exit(0);
        }
        cout << message << endl;
    }
    return nullptr;
}
void Quit(TcpSocket cfd_class){
    cfd_class.sendMsg("quit");
}
string Login(TcpSocket cfd_class){
    string input_uid;
    string pwd;
    input_uid = get_uid();  // 读入账号
    pwd = get_pwd();        // 读入密码
   
    // 命令包装成command类
    Command command(input_uid, LOGHIN_CHECK, {pwd});
    int ret = cfd_class.sendMsg(command.To_Json());  // 命令类将类转josn格式，再转json字符串格式，套接字类发送json字符串格式
    if(ret == 0 || ret == -1){
        cout << "服务器已关闭" << endl;
        exit(0);
    }
    // 收到操作结果
    // cout << "准备收到" << endl;
    string check = cfd_class.recvMsg();
    // cout << "收到" << endl;
    // cout << check << endl;
    if (check == "close" || check == "-1"){
        exit(0);
    }else if(check == "incorrect"){
        cout << "账号或密码错误." << endl;
        return "false";
    }else if (check == "online"){
        cout << "该账号正在登录中" << endl;
        return "false";
    }else if(check == "ok"){
        // 登录成功就新建一个线程等回信
        pthread_t tid;
        RecvArg *recv_arg = new RecvArg(input_uid,cfd_class.getrecvfd());
        ret = pthread_create(&tid, NULL, &recvfunc, static_cast<void*>(recv_arg));
        if(ret != 0){
            my_error("pthread_create()");
            exit(0);
        }
        ret = pthread_detach(tid);
        if(ret != 0){
            my_error("pthread_detach()");
            exit(0);
        }
         // cout << "登录成功" << endl;
        return input_uid;
    }else
        return "false";
}
bool Register(TcpSocket cfd_class){
    string pwd;
    while(true){
        cout << "请设置" << endl;
        pwd = get_pwd();        // 读入密码
        cout << "请确认";
        string pwd2 = get_pwd();
        if(pwd != pwd2){
            cout << "前后两次密码输入不一致，请重新选择操作·" << endl;
            return false;
        }else{
            break;
        }
    }
    Command command("NULL", REGISTER_CHECK,{pwd});
    int ret = cfd_class.sendMsg(command.To_Json());  // 命令类将类转josn格式，再转json字符串格式，套接字类发送json字符串格式
    if(ret == 0 || ret == -1){
        cout << "服务器已关闭." << endl;
        exit(0);
    }
    // 收到生成的密码
    string new_uid = cfd_class.recvMsg();
    if(new_uid == "close"){
        cout << "服务器已关闭." << endl;
        exit(0);
    }
    cout << "您注册的uid为: " << new_uid << endl << "忘记后无法找回，请牢记." << endl;
    return true;
}
bool AddFriend(TcpSocket cfd_class, Command command){
    int ret = cfd_class.sendMsg(command.To_Json());
    if(ret == 0 || ret == -1){
        cout << "服务器已关闭." << endl;
        exit (0);
    }
    string check = cfd_class.recvMsg();
    if(check == "close"){
        cout << "服务器已关闭." << endl;
        exit (0);
    }else if(check == "ok"){
        cout << "好友添加申请已发送,等待通过." << endl;
        return true;
    }else if(check == "had"){
        cout << "该用户已经是您的好友" << endl;
        return false;
    }else if(check == "cannot1"){
        cout << "您的系统消息中存在对方发来的好友申请，请先回复." << endl;
        return false;
    }else if(check == "cannot2"){
        cout << "您之前已向该用户发送好友申请,请等待回复." << endl;
        return false;
    }else{
        cout << "未找到该用户." << endl;
        return false;
    }
}
bool AddGroup(TcpSocket cfd_class, Command command){
    int ret = cfd_class.sendMsg(command.To_Json());
    if(ret == 0 || ret == -1){
        cout << "服务器已关闭." << endl;
        exit (0);
    }
    string check = cfd_class.recvMsg();
    if(check == "close"){
        cout << "服务器已关闭." << endl;
        exit (0);
    }else if(check == "ok"){
        cout << "群聊添加申请已发送,等待通过." << endl;
        return true;
    }else if(check == "had"){
        cout << "您已在该群聊中" << endl;
        return false;
    }else if(check == "cannot"){
        cout << "该群聊的申请列表中已有您的未处理的申请,请先处理." << endl;
        return false;
    }else{
        cout << "未找到该群聊." << endl;
        return false;
    }
}
bool AgreeAddFriend(TcpSocket cfd_class, Command command){
    int ret = cfd_class.sendMsg(command.To_Json());
    if(ret == 0 || ret == -1){
        cout << "服务器已关闭." << endl;
        exit (0);
    }
    string check = cfd_class.recvMsg();
    if(check == "close"){
        cout << "服务器已关闭." << endl;
        exit (0);
    }else if(check == "ok"){
        cout << "已通过" << command.m_option[0] << "的好友申请." << endl;
        return true;
    }else if(check == "had"){
        cout << "该用户已经是您的好友." << endl;
        return false;
    }else if(check == "haddeal"){
        cout << "您已经处理了该用户的申请." << endl;
        return false;
    }else if(check == "nofind"){
        cout << "未找到该用户的好友申请." << endl;
        return false;
    }
    else{
        cout << "其他错误" << endl;
        return false;
    }
}
bool ListFriend(TcpSocket cfd_class, Command command){
    int ret = cfd_class.sendMsg(command.To_Json());
    if(ret == 0 || ret == -1){
        cout << "服务器已关闭." << endl;
        exit (0);
    }
    while(true){
        string Friend = cfd_class.recvMsg();
        if(Friend == "end"){
            cout << "好友展示完毕" << endl;
            break;
        }else if(Friend == "none"){
            cout << "您当前还没有好友" << endl;
            return false;
        }else if(Friend == "close"){
            cout << "服务器已关闭." << endl;
            exit (0);
        }else {
            cout << Friend << endl;
        }
    }
    return true;
}
bool ChatFriend(TcpSocket cfd_class, Command command){
    // 进入与好友的聊天界面
    int ret = cfd_class.sendMsg(command.To_Json());  // 发送聊天请求
    if(ret == 0 || ret == -1){
        cout << "服务器已关闭." << endl; 
        exit(0);
    }            
    string check = cfd_class.recvMsg();        // 检查回复
    if(check == "close"){
        cout << "服务器已关闭." << endl; 
        exit(0);
    }else if(check == "none"){
        cout << "您还没有好友." << endl;
        return false;
    }else if(check == "nofind"){
        cout << "未找到该好友." << endl;
        return false;
    }

    // 有这个好友就打印历史聊天记录
    else if(check == "have"){
        string HistoryMsg;
        while(true){
            HistoryMsg = cfd_class.recvMsg();
            if(HistoryMsg == "close" || HistoryMsg == "-1"){
                cout << "服务器已关闭." << endl;
                exit(0);
            } 
            cout << HistoryMsg << endl;
            if(HistoryMsg == "以上为历史聊天记录"){
                break;
            }
        }

        // 循环获取用户想进行的操作（发消息或者发文件），'#'退出聊天，并更改自己的聊天对象
        string msg;
        while(true){
            //cout << "请输入您想发送的消息：" << endl; 
            cin.sync();
            getline(cin,msg);
            // 用户想退出聊天界面，送请求并等待服务器处理完毕
            if(msg == "#"){
                Command command_exit(command.m_uid,EXITCHAT,{"空"});
                ExitChatFriend(cfd_class, command_exit);
                break;
            }

            // 如果是发文件
            if(msg == "$"){
                // 获得要发送的文件路径并告诉服务器去创建文件保存目录
                string FilePath;
                string filepath;
                int file_fd;
                char buf[4096];
                memset(buf, '\0', sizeof(buf));
                cout << L_WHITE << "请输入您想发送的文件绝对路径(#取消)：" << NONE << endl;
                cin.sync();
                getline(cin, FilePath);
                if(FilePath == "#"){
                    cout << "已取消发送." << endl;
                    continue;
                }
                bool ok = true;
                for(auto c : msg){
                    if(isspace(c)){
                        ok = false;
                        break;
                    }
                }
                if(ok == false){
                    cout << UP << "文件路径中不能含有空白" << endl;
                    continue;
                }
                string filename(FilePath, FilePath.rfind("/") + 1);
                Command comamnd_filename(command.m_uid, SENDFILE, {command.m_option[0], filename, "begin"});
                int ret = cfd_class.sendMsg(comamnd_filename.To_Json());
                if(ret == 0 || ret == -1){
                    cout << "服务器已关闭." << endl; 
                    exit(0);
                }
                string check_create = cfd_class.recvMsg();
                if(check == "close"){
                    cout << "服务器已关闭." << endl; 
                    exit(0);
                }else if(check_create == "no"){
                    cout << "服务端创建文件存储目录出现错误." << endl;
                    continue;
                }
                // 否则目录创建成功，返回的是文件在服务器端的存储目录，保存文件在服务器端的完整路径
                else{
                    filepath = check_create + "/" + filename;
                }

                // 直到服务器端成功创建文件服务器打开文件，然后循环发送文件内容
                if((file_fd = open(FilePath.c_str(), O_RDONLY)) < 0){
                    cout << "文件打开失败或不存在该文件." << endl;
                    continue;
                }
                bool success = true;
                cout << L_WHITE << "文件上传中..." << NONE << endl;
                while(true){
                    string rar(512, '#');
                    string head = filepath + rar;
                    char* buffer_t = new char[4097];
                    memset(buffer_t, 0, 4097);
                    int count = read(file_fd, buffer_t, 4096);
                    if(count == 0){
                        break;
                    }
                    string buffer(buffer_t, 0, count);
                    int n = cfd_class.sendMsg((string(head, 0, 512) + buffer).c_str());
                    if(n < 0){
                        success = false;
                        break;
                    }else if(n == 0){
                        cout << "对端已关闭" << endl;
                        exit(0);
                    }else if(check == "close"){
                        cout << "服务器已关闭." << endl; 
                        exit(0);
                    }
                    delete[] buffer_t;
                    // 确认服务器端已经写入文件内容
                    string check = cfd_class.recvMsg();
                    if(check == "no"){
                        success = false;
                        break; 
                    }else if(check == "close"){
                        cout << "服务器已关闭." << endl;
                        exit(0);
                    }
                }
                if(success == true)
                    cout << L_GREEN << "文件上传完毕." << NONE << endl;
                else
                    cout << L_RED << "文件上传失败." << NONE << endl;
                close(file_fd);
                // 所有文件内容已经传输完毕，告诉服务器端进行收尾工作
                cout << endl;
                Command command_file_end(command.m_uid, SENDFILE, {command.m_option[0], filename, "end"});
                ret = cfd_class.sendMsg(command_file_end.To_Json()); 
                if(ret == 0 || ret == -1){
                    cout << "服务器已关闭." << endl; 
                    exit(0);
                } 
                string check_end = cfd_class.recvMsg();
                if(check_end == "close"){
                    cout << "服务器已关闭." << endl;
                    exit(0);
                }else if(check == "ok"){
                }
                continue;
            }
            // 不是文件，就是消息，消息中不能含有空白
            bool ok = true;
            for(auto c : msg){
                if(isspace(c)){
                    ok = false;
                    break;
                }
            }
            if(ok == false){
                cout << UP << "消息中不能含有空白" << endl;
                continue;
            }
            // 把消息包装好，让服务器转发
            Command command_msg(command.m_uid, FRIENDMSG, {command.m_option[0], msg});
            int ret = cfd_class.sendMsg(command_msg.To_Json());
            if(ret == 0 || ret == -1){
                cout << "服务器已关闭." << endl; 
                exit(0);
            }
            string check = cfd_class.recvMsg();
            if(check == "close"){
                cout << "服务器已关闭." << endl;
                exit(0);
            }else if(check == "ok"){
                continue;
            }else if(check == "nohave"){
                cout << "很遗憾,该用户已经和您解除了好友关系(输入#退出)." << endl;
                cout << "莫愁前路无知己，天下谁人不识君." << endl;
            }
        }
    }
    return true;
}
bool ChatGroup(TcpSocket cfd_class, Command command){
    int ret = cfd_class.sendMsg(command.To_Json());  // 发送聊天请求
    if(ret == 0 || ret == -1){
        cout << "服务器已关闭." << endl; 
        exit(0);
    }            
    string check = cfd_class.recvMsg();        // 检查回复
    if(check == "close"){
        cout << "服务器已关闭." << endl; 
        exit(0);
    }else if(check == "none"){
        cout << "您还没有加入任何群聊." << endl;
        return false;
    }else if(check == "nofind"){
        cout << "未找到该群聊." << endl;
        return false;
    }
    // 有这个群聊就打印历史聊天记录
    else if(check == "have"){
        string HistoryMsg;
        while(true){
            HistoryMsg = cfd_class.recvMsg();
            if(HistoryMsg == "close" || HistoryMsg == "-1"){
                cout << "服务器已关闭." << endl;
                exit(0);
            } 
            cout << HistoryMsg << endl;
            if(HistoryMsg == "以上为历史聊天记录"){
                break;
            }
        }
        // 给好友发送消息,‘#‘退出聊天，并更改自己的聊天对象
        string msg;
        while(true){
            // cout << "请输入您想发送的消息：" << endl; 
            cin.sync();
            getline(cin,msg);
            // 用户想退出聊天界面，送请求并等待服务器处理完毕
            if(msg == "#"){
                Command command_exit(command.m_uid,EXITGROUPCHAT,{"空"});
                ExitChatGroup(cfd_class, command_exit);
                break;
            }
            // 消息中不能含有空白
            bool ok = true;
            for(auto c : msg){
                if(isspace(c)){
                    ok = false;
                    break;
                }
            }
            if(ok == false){
                cout << "消息中不能含有空白" << endl;
                continue;
            }
            // 把消息包装好，让服务器转发
            Command command_msg(command.m_uid, GROUPMSG, {command.m_option[0], msg});
            int ret = cfd_class.sendMsg(command_msg.To_Json());
            if(ret == 0 || ret == -1){
                cout << "服务器已关闭." << endl; 
                exit(0);
            }
            string check = cfd_class.recvMsg();
            if(check == "close"){
                cout << "服务器已关闭." << endl;
                exit(0);
            }else if(check == "ok"){
                continue;
            }else if(check == "nohave"){
                cout << "很遗憾,您已被群主移出了该群聊(输入#退出)." << endl;
                cout << "道不同，不相为谋." << endl;
            }
        }
    }
    return true;
}
bool ExitChatFriend(TcpSocket cfd_class, Command command){
    int ret = cfd_class.sendMsg(command.To_Json());  // 发送退出聊天请求
    if(ret == 0 || ret == -1){
        cout << "服务器已关闭." << endl; 
        exit(0);
    }
    string check = cfd_class.recvMsg();
    if(check == "close"){
        cout << "服务器已关闭." << endl;
        exit (0);
    }else if(check == "ok"){
        system("clear");
        cout << "已退出聊天" << endl;
        return true;
    }else if (check == "no"){
        cout << L_WHITE << "无效的操作，请重新输入." << NONE << endl;
        return false;
    }
    return false;
}
bool ExitChatGroup(TcpSocket cfd_class, Command command){
    int ret = cfd_class.sendMsg(command.To_Json());  // 发送退出聊天请求
    if(ret == 0 || ret == -1){
        cout << "服务器已关闭." << endl; 
        exit(0);
    }
    string check = cfd_class.recvMsg();
    if(check == "close"){
        cout << "服务器已关闭." << endl;
        exit (0);
    }else if(check == "ok"){
        system("clear");
        cout << "已退出聊天" << endl;
        return true;
    }else if (check == "no"){
        cout << L_WHITE << "无效的操作，请重新输入." << NONE << endl;
        return false;
    }
    return false;
}
bool ShieldFriend(TcpSocket cfd_class, Command command){
    int ret = cfd_class.sendMsg(command.To_Json());
    if(ret == 0 || ret == -1){
        cout << "服务器已关闭." << endl;
        exit(0);
    }
    string check = cfd_class.recvMsg();
    if(check == "close"){
        cout << "服务器已关闭." << endl;
        exit (0);
    }else if(check == "no"){
        cout << "未找到该好友." << endl;
        return false;
    }else if(check == "ok"){
        cout << "已屏蔽该好友的会话." << endl;
        return true;
    }else if(check == "had") {
        cout << "该好友已被屏蔽." << endl;
    }else{
        cout << "错误" << endl;
    }
    return false;
}
bool DeleteFriend(TcpSocket cfd_class, Command command){
    int ret = cfd_class.sendMsg(command.To_Json());
    if(ret == 0 || ret == -1){
        cout << "服务器已关闭." << endl;
        exit(0);
    }
    string check = cfd_class.recvMsg();
    if(check == "close"){
        cout << "服务器已关闭." << endl;
        exit (0);
    }else if(check == "nofind"){
        cout << "未找到该好友." << endl;
        return false;
    }else if(check == "ok"){
        cout << "已删除该好友." << endl;
        return true;
    }
    return false;
}
bool Restorefriend(TcpSocket cfd_class, Command command){
    int ret = cfd_class.sendMsg(command.To_Json());
    if(ret == 0 || ret == -1){
        cout << "服务器已关闭." << endl;
        exit(0);
    }
    string check = cfd_class.recvMsg();
    if(check == "close"){
        cout << "服务器已关闭." << endl;
        exit (0);
    }else if(check == "nohave"){
        cout << "未找到该好友." << endl;
        return false;
    }else if(check == "nofind"){
        cout << "该好友未被屏蔽." << endl;
        return false;
    }else if(check == "ok"){
        cout << "已恢复该好友的会话." << endl;
        return true;
    }else{
        cout << "该好友已被屏蔽." << endl;
    }
    return false;
}
bool NewMessage(TcpSocket cfd_class, Command command){
    int ret = cfd_class.sendMsg(command.To_Json());
    if(ret == 0 || ret == -1){
        cout << "服务器已关闭." << endl;
        exit (0);
    }
    while(true){
        string oneline = cfd_class.recvMsg();
        if(oneline == "end"){
            break;
        }else if(oneline == "none"){
            cout << "您当前没有未读消息" << endl;
            break;
        }else if(oneline == "close"){
            cout << "服务器已关闭." << endl;
            exit (0);
        }else {
            cout << oneline << endl;
        }
    }
    return true;
}
bool LookSystem(TcpSocket cfd_class, Command command){
    int ret = cfd_class.sendMsg(command.To_Json());
    if(ret == 0 || ret == -1){
        cout << "服务器已关闭." << endl;
        exit (0);
    }
    while(true){
        string SysMsg = cfd_class.recvMsg();
        if(SysMsg == "end"){
            cout << "以上为系统消息" << endl;
            break;
        }else if(SysMsg == "none"){
            cout << "系统消息为空" << endl;
            break;
        }else if(SysMsg == "close"){
            cout << "服务器已关闭." << endl;
            exit (0);
        }else {
            cout << SysMsg << endl;
        }
    }
    return true;
}
bool LookNotice(TcpSocket cfd_class, Command command){
    int ret = cfd_class.sendMsg(command.To_Json());
    if(ret == 0 || ret == -1){
        cout << "服务器已关闭." << endl;
        exit (0);
    }
    while(true){
        string notice = cfd_class.recvMsg();
        if(notice == "end"){
            cout << "以上为通知消息" << endl;
            break;
        }else if(notice == "none"){
            cout << "通知消息为空" << endl;
            break;
        }else if(notice == "close"){
            cout << "服务器已关闭." << endl;
            exit (0);
        }else {
            cout << notice << endl;
        }
    }
    return true;
}
bool RefuseAddFriend(TcpSocket cfd_class, Command command){
    int ret = cfd_class.sendMsg(command.To_Json());
    if(ret == 0 || ret == -1){
        cout << "服务器已关闭." << endl;
        exit (0);
    }
    string check = cfd_class.recvMsg();
    if(check == "close"){
        cout << "服务器已关闭." << endl;
        exit (0);
    }else if(check == "ok"){
        cout << "已拒绝" << command.m_option[0] << "的好友申请" << endl;
        return true;
    }else if(check == "had"){
        cout << "该用户已经是您的好友" << endl;
        return false;
    }else if(check == "haddeal"){
        cout << "您已经处理了该用户的申请." << endl;
        return false;
    }else if(check == "nofind"){
        cout << "未找到该用户的好友申请." << endl;
        return false;
    }
    else{
        cout << UP  << "其他错误" << endl;
        return false;
    }
}
bool CreateGroup(TcpSocket cfd_class, Command command){
    // 调用展示好友列表函数展示好友列表
    Command command0(command.m_uid, LISTFRIEND, {"无"});
    if (!ListFriend(cfd_class, command0)){
        cout << "无法创建群聊." << endl;
        return false;
    }
    // 获得群聊初始成员列表
    cout << "请从以上好友中选择群聊初始成员." << endl;
    cout << "请输入好友的uid,以一个空格分割：" << endl;
    string f_list;
    getline(cin, f_list);
    Command command1(command.m_uid, CREATEGROUP, {f_list});
    // 给服务器发送创建群聊的请求
    int ret = cfd_class.sendMsg(command1.To_Json());
    if(ret == 0 || ret == -1){
        cout << "服务器已关闭." << endl;
        exit (0);
    }
    // 检查回复
    string check = cfd_class.recvMsg();        
    if(check == "close"){
        cout << "服务器已关闭." << endl; 
        exit(0);
    }else if(check.find("nofind") == 0 ){
        string nofriend(check.begin() + 6, check.end());
        cout << "应输入好友的uid并以空格分割." << endl;
        cout << "没有找到好友：." << nofriend << endl;
        return false;
    }else{
        cout << "群聊创建成功,群号为：" << check << endl;
        return true;
    }
}
bool ListGroup(TcpSocket cfd_class, Command command){
    int ret = cfd_class.sendMsg(command.To_Json());
    if(ret == 0 || ret == -1){
        cout << "服务器已关闭." << endl;
        exit (0);
    }
    while(true){
        string Group = cfd_class.recvMsg();
        if(Group == "end"){
            cout << "群聊展示完毕" << endl;
            break;
        }else if(Group == "none"){
            cout << "您当前还没有加入群聊" << endl;
            return false;
        }else if(Group == "close"){
            cout << "服务器已关闭." << endl;
            exit (0);
        }else {
            cout << Group << endl;
        }
    }
    return true;
}
bool AboutGroup(TcpSocket cfd_class, Command command){
    int ret = cfd_class.sendMsg(command.To_Json());
    if(ret == 0 || ret == -1){
        cout << "服务器已关闭." << endl;
        exit (0);
    }
    string check = cfd_class.recvMsg();
    if(check == "close"){
        cout << "服务器已关闭." << endl;
        exit (0);
    }else if(check == "ok"){
        cout << "关于群聊" << command.m_option[0] << "：" << endl;
    }else if(check == "nohave"){
        cout << "该群聊不存在" << endl;
        return false;
    }else if(check == "no"){
        cout << "你不在该群聊中." << endl;
        return false;
    }
    string option0 = command.m_option[0];   // 选项第一个为想要操作的群聊
    // while循环不断展示选项和获取操作
    while(true){
        display_menu2();
        string input;
        cout << L_WHITE << "就决定是你了：" << NONE << endl;
        cin.sync();
        getline(cin, input);

        if(input == "return"){
            break;
        }
        else if(input == "request"){
            string option0 = command.m_option[0];
            Command command1(command.m_uid, REQUSTLIST, {option0});
            RequestList(cfd_class, command1);
        }
        else if(input.find("pass-") == 0 && input.size() == 9){
            string option0 = command.m_option[0];
            string option1(input.begin() + 5, input.end());
            Command command1(command.m_uid, PASSAPPLY, {option0, option1});
            PassApply(cfd_class, command1);
        }
        else if(input.find("deny-") == 0 && input.size() == 9){
            string option0 = command.m_option[0];
            string option1(input.begin() + 5, input.end());
            Command command1(command.m_uid, DENYAPPLY, {option0, option1});
            DenyApply(cfd_class, command1);
        }
        else if(input.find("set-") == 0 && input.size() >= 15 && input.size() <= 16){
            string option0 = command.m_option[0];
            string option1(input.begin() + 4, input.begin() + 8);
            string option2(input.begin() + 9, input.end());
            if(option2 == "leader"){
                cout << L_WHITE << "此操作成功的话会使您失去群主身份，是否继续？(y/n)" << NONE << endl;
                string state;
                getline(cin, state);
                if(state != "y"){
                    continue;
                }
            }
            Command command1(command.m_uid, SETMEMBER, {option0, option1, option2});
            SetMember(cfd_class, command1);
        }
        else if(input == "exit"){
            string option0 = command.m_option[0];
            Command command1(command.m_uid, EXITGROUP, {option0});
            if(ExitGroup(cfd_class, command1) == true){
                cout << L_WHITE << "您确定要退出群聊吗？(y/n)" << NONE << endl;
                string state;
                getline(cin, state);
                if(state != "y"){
                    continue;
                }
                cout << "您已退出该群聊." << endl;
                break;
            }
        }
        else if(input == "members"){
            string option0 = command.m_option[0];
            Command command1(command.m_uid, DISPLAYMEMBER , {option0});
            DisplyMember(cfd_class, command1);
        }
        else if(input.find("remove-") == 0 && input.size() == 11){
            string option0 = command.m_option[0];
            string option1(input.begin() + 7, input.end());
            Command command1(command.m_uid, REMOVEMEMBER, {option0, option1});
            RemoveMember(cfd_class, command1);
        }
        else if(input == "info"){
            string option0 = command.m_option[0];
            Command command1(command.m_uid, INFOXXXX, {option0});
            InfoXXXX(cfd_class, command1);
        }else{
            cout << "无效的操作，请重新输入." << endl;
            continue;
        }
    }
    return true;
}
bool RequestList(TcpSocket cfd_class, Command command){
    int ret = cfd_class.sendMsg(command.To_Json());
    if(ret == 0 || ret == -1){
        cout << "服务器已关闭." << endl;
        exit (0);
    }
    while(true){
        string apply = cfd_class.recvMsg();
        if(apply == "close"){
            cout << "服务器已关闭." << endl;
            exit (0);
        }else if(apply == "none"){
            cout << "当前还没有入群申请" << endl;
            return false;
        }else if(apply == "cannot"){
            cout << "您在该群聊中没有此权限." << endl;
            return false;
        }else if(apply == "end"){
            cout << "入群申请展示完毕." << endl;
            return true;
        }else {
            cout << apply << endl;
        }
    }
    return true;
}
bool PassApply(TcpSocket cfd_class, Command command){
    int ret = cfd_class.sendMsg(command.To_Json());
    if(ret == 0 || ret == -1){
        cout << "服务器已关闭." << endl;
        exit (0);
    }
    string check = cfd_class.recvMsg();
    if(check == "close"){
        cout << "服务器已关闭." << endl;
        exit(0);
    }else if(check == "cannot"){
        cout << "您在此群聊中无此权限." << endl;
        return false;
    }else if(check == "nofind"){
        cout << "没有该用户的申请." << endl;
        return false;
    }else if(check == "haddeal"){
        cout << "该用户的申请已被处理." << endl;
        return false;
    }else if(check == "had"){
        cout << "该用户已经在群里." << endl;
        return false;
    }else if(check == "ok"){
        cout << "已通过该用户的入群申请." << endl;
        return true;
    }else{
        cout << "其他错误." << endl;
        return false;
    }
}
bool DenyApply(TcpSocket cfd_class, Command command){
    int ret = cfd_class.sendMsg(command.To_Json());
    if(ret == 0 || ret == -1){
        cout << "服务器已关闭." << endl;
        exit (0);
    }
    string check = cfd_class.recvMsg();
    if(check == "close"){
        cout << "服务器已关闭." << endl;
        exit (0);
    }else if(check == "cannot"){
        cout << "您在此群聊中无此权限." << endl;
        return false;
    }else if(check == "nofind"){
        cout << "没有该用户的申请." << endl;
        return false;
    }else if(check == "haddeal"){
        cout << "该用户的申请已被处理." << endl;
        return false;
    }else if(check == "had"){
        cout << "该用户已经在群里." << endl;
        return false;
    }else if(check == "ok"){
        cout << "已拒绝该用户的入群申请." << endl;
        return true;
    }else{
        cout << "其他错误." << endl;
        return false;
    }
}
bool SetMember(TcpSocket cfd_class, Command command){
    int ret = cfd_class.sendMsg(command.To_Json());
    if(ret == 0 || ret == -1){
        cout << "服务器已关闭." << endl;
        exit (0);
    }
    string check = cfd_class.recvMsg();
    if(check == "close"){
        cout << "服务器已关闭." << endl;
        exit (0);
    }else if(check == "cannot"){
        cout << "您在此群聊中无此权限." << endl;
        return false;
    }else if(check == "cannot1"){
        cout << "您不能修改自己的权限，只能转让群,." << endl;
        return false;
    }else if(check == "nohave"){
        cout << "该用户不是群聊成员." << endl;
        return false;
    }else if(check == "cannot2"){
        cout << "只能设置管理员,群成员和转让群." << endl;
        return false;
    }else if(check == "hadis"){
        cout << "该用户已经是此职位." << endl;
        return false;
    }else if(check == "ok"){
        cout << "已修改该用户的权限." << endl;
        return true;
    }else{
        cout << "其他错误." << endl;
        return false;
    }
}
bool ExitGroup(TcpSocket cfd_class, Command command){
    int ret = cfd_class.sendMsg(command.To_Json());
    if(ret == 0 || ret == -1){
        cout << "服务器已关闭." << endl;
        exit (0);
    }
    string check = cfd_class.recvMsg();
    if(check == "close"){
        cout << "服务器已关闭." << endl;
        exit (0);
    }else if(check == "cannot"){
        cout << "身为群主，您无法退群，只能转让群或解散群." << endl;
        return false;
    }else if(check == "ok"){
        cout << "您已退出该群聊." << endl;
        return true;
    }else{
        cout << "其他错误." << endl;
        return false;
    }
}
bool DisplyMember(TcpSocket cfd_class, Command command){
    int ret = cfd_class.sendMsg(command.To_Json());
    if(ret == 0 || ret == -1){
        cout << "服务器已关闭." << endl;
        exit (0);
    }
    while(true){
        string memeber = cfd_class.recvMsg();
        if(memeber == "end"){
            cout << "群成员展示完毕" << endl;
            break;
        }else if(memeber == "close"){
            cout << "服务器已关闭." << endl;
            exit (0);
        }else {
            cout << memeber << endl;
        }
    }
    return true;
}
bool RemoveMember(TcpSocket cfd_class, Command command){
    int ret = cfd_class.sendMsg(command.To_Json());
    if(ret == 0 || ret == -1){
        cout << "服务器已关闭." << endl;
        exit (0);
    }
    string check = cfd_class.recvMsg();
    if(check == "close"){
        cout << "服务器已关闭." << endl;
        exit (0);
    }else if(check == "ok"){
        cout << "已将其移出群聊." << endl;
        return false;
    }else if(check == "no"){
        cout << "该用户不在此群聊." << endl;
        return false;
    }else if(check == "cannot"){
        cout << "您没有此权限." << endl;
        return false;
    }else if(check == "cannot0"){
        cout << "您不能移出管理员或群主." << endl;
        return false;
    }else{
        cout << "其他错误." << endl;
        return false;
    }
}
bool InfoXXXX(TcpSocket cfd_class, Command command){

    return true;
}