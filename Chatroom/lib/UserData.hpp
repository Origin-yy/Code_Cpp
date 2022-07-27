#include <nlohmann/json.hpp>
#include <string>
#include <queue>
#include <unordered_map>
#include <unistd.h>
#include <string>

using namespace std;
using json = nlohmann::json;

class UserData{  // 用户数据类，存储一个用户的所有数据
public:
    UserData() = default;
    ~UserData();
private:
    string uid;                 // 注册时用户输入uid
    string password = 0;        // 密码
    int on_fd = -1;             // 不在线为-1.在线为此次登录的fd
    string sex;                 // 性别
    string nickname;            // 昵称
    string other;               // 其他信息
    vector<string> list_friend; // 好友列表
    vector<string> list_group;  // 群聊列表

public:
    string getUid() const { return uid; }
    bool setUid(string newUid) { uid = newUid; }
    string getpassword() const { return password; }
    bool setpassword(string newpassword) { password = newpassword; }
    int geton_fd() const { return on_fd; }
    bool setsex(string newsex) { sex = newsex; }
    string getnickname() const { return nickname; }
    bool setpnickname(string newnickname) { nickname = newnickname; }
    string getother() const { return other; }
    bool setother(string newother) { other = newother; }

    void From_Json(string& data_string);
    string To_Json();
};



void UserData::From_Json(string& data_string){
    json data_json = json::parse(data_string);    // json字符串格式转json格式
    // json格式转类
    data_json.at("uid").get_to(uid);
    data_json.at("password").get_to(password);
    data_json.at("on_fd").get_to(on_fd);
    data_json.at("sex").get_to(sex);
    data_json.at("nickname").get_to(nickname);
    data_json.at("other").get_to(other);
    data_json.at("list_friend").get_to(list_friend);
    data_json.at("list_group").get_to(list_group);
}
string UserData::To_Json(){
    // 类转json格式
    json data_json = {   
        {"uid",uid},
        {"password",password},
        {"on_fd",on_fd},
        {"sex",sex},
        {"nickname",nickname},
        {"other",other},
        {"list_friend",list_friend},
        {"list_group",list_group},
    };
    return data_json.dump();  // json格式转为json字符串格式

}