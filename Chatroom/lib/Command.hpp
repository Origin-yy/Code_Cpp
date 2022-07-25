#ifndef COMMAND_H
#define COMMAND_H

#include "TCPSocket.hpp"
#include "nlohmann/json.hpp"
#include <nlohmann/json_fwd.hpp>
#include <string>
#include <vector>
using namespace std;

struct Command{
public:
    string option;  // 命令的操作内容
    int uid;    // 发送者的uid（没有的话为0）
    int flag = 0;   // 发送者的操作内容的类别
    int cfd;  // 发送者的fd
    static void From_Json(nlohmann::json& jn, Command& command){
        jn.at("option").get_to(command.option);
        jn.at("uid").get_to(command.uid);
        jn.at("flag").get_to(command.flag);
        jn.at("cfd").get_to(command.cfd);
    }
    static void To_Json(nlohmann::json& jn, Command& command){
        jn = nlohmann::json{
            {"option",command.option},
            {"uid",command.uid},
            {"flag",command.flag},
            {"cfd",command.cfd},
        };
    }
};

#endif