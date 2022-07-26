#ifndef __REDIS_HANDLER_H__
#define __REDIS_HANDLER_H__

#include <hiredis/hiredis.h>
#include <string>
#include <cstring>
#include <iostream>
using namespace std;



class Redis{
public:
    Redis() = default;
    Redis(string addr, int port) : redis_addr(addr),redis_port(port) {} //指定redis ip地址和端口
    ~Redis();
    bool connect();                                                     // 阻塞连接redis数据库
    bool connect(struct timeval timeout);                               // 超时连接redis
    bool disConnect();                                                  // 断开连接
    bool setValue(const string &key, const string &value);              // 添加或修改键值对
    string getValue(const string &key);                                 // 获取键对应的值
    bool delKey(const string &key);                                     // 删除键

    bool hsetValue(const string &key, const string &field, const string &value); // 插入哈希表
    bool hashexists(const string &key, const string &field);                     // 查看是否存在哈希表
    string gethash(const string &key, const string &field);                      // 获取对应的hash_value
    bool delhash(const string &key, const string &field);                        // 从哈希表删除指定的元素


    
private:
    string redis_addr = "127.0.0.1";   // redis IP地址，默认环回地址
    int redis_port = 6379;             // redis端口号，默认6379
    redisContext *redis_s = nullptr;   // redis句柄
    redisReply *reply = nullptr;       // redisCommand返回的结构体
};

Redis::~Redis(){
    disConnect();
    redis_s = nullptr;
    reply = nullptr;
}
// 阻塞连接redis
bool Redis::connect(){
    redis_s = redisConnect(redis_addr.c_str(), redis_port);
    if (redis_s == nullptr || redis_s->err) {
        cerr << "redis连接失败" << endl;
        return false;
    }
    return true;
}
// 超时连接redis
bool Redis::connect(struct timeval timeout){
    redis_s = redisConnect(redis_addr.c_str(), redis_port);
    if (redis_s == nullptr || redis_s->err) {
        cerr << "redis连接失败" << endl;
        return false;
    }
    return true;
}
// 断开链接
bool Redis::disConnect(){
    freeReplyObject(reply);
    redisFree(redis_s);
    return true;
}
// 设置键值
bool Redis::setValue(const string &key, const string &value){
    string cmd = "set  " + key + "  " + value;
    reply = (redisReply *)redisCommand(redis_s, cmd.c_str());
    if(reply == nullptr){
        cerr << "redis:" << cmd << "失败" << endl;
        return false;
    }else{
        return true;
    };
}
// 获取键对应的值
string Redis::getValue(const string &key){ 
    string cmd = "get  " + key;
    reply = (redisReply *)redisCommand(redis_s, cmd.c_str());
    return reply->str;
}
// 删除键值
bool Redis::delKey(const string &key){
    string cmd = "del  " + key;
    reply = (redisReply *)redisCommand(redis_s, cmd.c_str());
    if(reply == nullptr){
        cerr << "redis:" << cmd << "失败" << endl;
        return false;
    }else{
        return true;
    };
}
// 插入哈希表
bool Redis::hsetValue(const string &key, const string &field, const string &value){ 
    string cmd = "hset  " + key + " " + field + " " + value;
    reply = (redisReply *)redisCommand(redis_s, cmd.c_str());
    if(reply == nullptr){
        cerr << "redis:" << cmd << "失败" << endl;
        return false;
    }else{
        return true;
    };
}
// 哈希表是否存在
bool Redis::hashexists(const string &key, const string &field){ 
    string cmd = "hexists  " + key + "  " + field;
    reply = (redisReply *)redisCommand(redis_s, cmd.c_str());
    if(reply == nullptr){
        cerr << "redis:" << cmd << "失败" << endl;
        return false;
    }else{
        return true;
    };
}
// 获取对应的hash_value
string Redis::gethash(const string &key, const string &field){ 
    string cmd = "hget  " + key + "  " + field;
    reply = (redisReply *)redisCommand(redis_s, cmd.c_str());
    return reply->str;
}
// 从哈希表删除指定的元素
bool Redis::delhash(const string &key, const string &field){    
    string cmd = "hdel  " + key + "  " + field;
    reply = (redisReply *)redisCommand(redis_s, cmd.c_str());
    if(reply == nullptr){
        cerr << "redis:" << cmd << "失败" << endl;
        return false;
    }else{
        return true;
    };
}

#endif