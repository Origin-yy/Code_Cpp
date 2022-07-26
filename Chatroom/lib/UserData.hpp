#include <string>
#include <queue>
#include <map>
#include <unistd.h>
#include <string>

using namespace std;

string GetNowTime();

struct Message{  // 消息类，包含发送者uid，发送时间，内容
public:
    Message() = default;
    Message(string senderUid, string time ,string msg): senderUid(senderUid), time(time), msg(msg) {};
    ~Message();
private:
    string senderUid;
    string time = GetNowTime();
    string msg;
};

class UserData{  // 用户数据类，存储一个用户的所有数据
public:
    UserData() = default;
    ~UserData();
private:
    string uid;     // 注册时用户输入uid
    string password = 0; // 密码
    vector<int> list_friend;   // 好友列表
    vector<int> list_group;    // 群聊列表

    // 消息队列，key为和用户聊天的群聊/好友的uid,value为两者间的消息队列
    map <string, queue<Message> > map_uid_queue;

public:
    bool online = false;   // 是否在线
    string sex;      // 性别
    string nickname; // 昵称
    string other_information;  // 其他信息
    string getUid() const { return uid; }
    void add_message_queue(string& uid);    // 添加一个当前用户与参数所给uid代表的好友/群聊的消息队列进map
    void add_message(string& senderUid, string msg, string uid_queue);  // 发送者的uid，添加一条消息进uid指定的消息队列
};

void UserData::add_message_queue(string& uid){
    queue<Message> *newQueue = new queue<Message>;
    map_uid_queue.insert(make_pair(uid,*newQueue));
}
void UserData::add_message(string& senderUid, string msg, string uid_queue){
    Message newMessage(senderUid, GetNowTime(), msg);
    map_uid_queue.find(uid_queue)->second.push(newMessage);
}


string GetNowTime(){
    time_t nowtime;
    struct tm* p;
    time(&nowtime);
    p = localtime(&nowtime);
    string time = to_string(p->tm_mon) + "-" + to_string(p->tm_mday) + "-" + 
                  to_string(p->tm_hour) + "-" + to_string(p->tm_min);

    return time;
}