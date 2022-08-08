#ifndef MESSAGE_H
#define MESSAGE_H

#include <nlohmann/json.hpp>
#include <string>

using namespace std;
using json = nlohmann::json;

struct Message{
public:
    Message() = default;
    ~Message() = default;
    Message(string SendUid, string RecvUid, string content, string t_time) : 
            SendUid(SendUid), RecvUid(RecvUid), content(content), t_time(t_time){}

    string SendUid;
    string RecvUid;
    string content;
    string t_time;
    // 讲一个json字符串转为类
    void From_Json(string message_js){
        json jn = json::parse(message_js);
        jn.at("SendUid").get_to(SendUid);
        jn.at("RecvUid").get_to(RecvUid);
        jn.at("content").get_to(content);
        jn.at("t_time").get_to(t_time);
    }
    // 将类转为json字符串
    string To_Json(){
        json jn = json{
            {"SendUidd", SendUid},
            {"RecvUid", RecvUid},
            {"content", content},
            {"t_time", t_time},
        };
        return jn.dump(); 
    }
};

#endif