#ifndef COMMAND_H
#define COMMAND_H

#include "TCPSocket.hpp"
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using json = nlohmann::json;

struct Command{
public:
    Command() = default;
    Command(string uid, int cfd, int flag, string option)
            : m_uid(uid), m_cfd(cfd), m_flag(flag), m_option(option) {}
    string m_uid;       // 发送者的uid（没有的话为0）
    int m_cfd;          // 发送者的fd
    int m_flag = 0;     // 发送者的操作内容的类别
    string m_option;    // 命令的操作内容
    void From_Json(string command_string){
        json command_json = json::parse(command_string);
        command_json.at("uid").get_to(m_uid);
        command_json.at("cfd").get_to(m_cfd);
        command_json.at("flag").get_to(m_flag);
        command_json.at("option").get_to(m_option);
    }
    string To_Json(){
        json command_json = json{
            {"uid",m_uid},
            {"cfd",m_cfd},
            {"flag",m_flag},
            {"option",m_option},
        };
        return command_json.dump(); // json格式转为json字符串格式
    }
};

#endif