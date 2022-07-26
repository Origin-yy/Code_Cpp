#include <string>
#include <queue>
#include <map>
#include <unistd.h>

using namespace std;

string GetNowTime();

struct message{  // 消息类，包含发送者uid，发送时间，内容
public:
    message() = default;
    message(string senderUid, string time ,string msg): senderUid(senderUid), time(time), msg(msg) {};
    ~message();
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
    map <int, queue<message> > map_uid_queue;

public:
    bool online = false;   // 是否在线
    string sex;      // 性别
    string nickname; // 昵称
    string other_information;  // 其他信息
    string getUid() const { return uid; }
    void add_message_queue(int& uid);    // 添加一个当前用户与参数所给uid代表的好友/群聊的消息队列进map
    void add_message(int& senderUid, string msg, int uid_queue);  // 发送者的uid，添加一条消息进uid指定的消息队列
};