#ifndef COMMAND_H
#define COMMAND_H

#include <nlohmann/json.hpp>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using json = nlohmann::json;

struct Command{
public:
    Command() = default;
    ~Command() = default;
    Command(string uid, int flag, vector<string> option)
            : m_uid(uid), m_flag(flag), m_option(option) {}
    string m_uid;       // 发送者的uid（没有的话为0）
    int m_flag = 0;     // 发送者的操作内容的类别
    vector<string> m_option;    // 命令的操作内容

    void From_Json(string command_js){
        json jn = json::parse(command_js);
        jn.at("uid").get_to(m_uid);
        jn.at("flag").get_to(m_flag);
        jn.at("option").get_to(m_option);
    }
    string To_Json(){
        json jn = json{
            {"uid",m_uid},
            {"flag",m_flag},
            {"option",m_option},
        };
        return jn.dump(); // json格式转为json字符串格式
    }
};

#endif