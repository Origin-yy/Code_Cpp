#ifndef __REDIS_HANDLER_H__
#define __REDIS_HANDLER_H__

#include <cstring>
#include <hiredis/hiredis.h>
#include <iostream>
#include <string>

using namespace std;

class Redis {
public:
  Redis() = default;
  Redis(string addr, int port)
      : redis_addr(addr), redis_port(port) {} // 指定redis ip地址和端口
  ~Redis();
  bool connect();                       // 阻塞连接redis数据库
  bool connect(struct timeval timeout); // 超时连接redis
  bool disConnect();                    // 断开连接
  bool setValue(const string &key, const string &value); // 添加或修改键值对
  string getValue(const string &key); // 获取键对应的值
  bool delKey(const string &key);     // 删除键
  // 哈希表相关操作
  bool hsetValue(const string &key, const string &field,
                 const string &value); // 指定key哈希表field字段的值
  bool hashexists(const string &key,
                  const string &field); // 查看key的哈希表中是否存在field字段
  string gethash(const string &key,
                 const string &field); // 获取key哈希表中field对应的值
  bool delhash(const string &key,
               const string &field);     // 从哈希表删除指定的元素
  int hlen(const string &key);           // 返回哈希表中的元素个数
  redisReply **hkeys(const string &key); // 返回哈希表中所有字段
                                         // set相关操作
  int scard(const string &key);          // 返回set集合里的元素个数
  int saddvalue(const string &key, const string &value); // 插入到集合
  int sismember(const string &key, const string &value); // 查看数据是否存在
  int sremvalue(const string &key, const string &value); // 将数据从set中移出
  redisReply **smembers(const string &key); // 返回set中所有成员
  // list相关操作
  int lpush(const string &key, const string &value); // 插入一条消息
  int llen(const string &key);                       // 获取列表长度
  redisReply **lrange(const string &key);            // 返回列表所有元素
  redisReply **lrange(const string &key, string a,
                      string b); // 返回列表中指定的元素
  int ltrim(const string &key);  // 删除列表中的所有元素

private:
  string redis_addr = "127.0.0.1"; // redis IP地址，默认环回地址
  int redis_port = 6379;           // redis端口号，默认6379
  redisContext *redis_s = nullptr; // redis句柄
  redisReply *reply = nullptr;     // redisCommand返回的结构体
};

Redis::~Redis() {
  disConnect();
  redis_s = nullptr;
  reply = nullptr;
}
// 阻塞连接redis
bool Redis::connect() {
  redis_s = redisConnect(redis_addr.c_str(), redis_port);
  if (redis_s == nullptr || redis_s->err) {
    cerr << "redis连接失败" << endl;
    return false;
  }
  return true;
}
// 超时连接redis
bool Redis::connect(struct timeval timeout) {
  redis_s = redisConnect(redis_addr.c_str(), redis_port);
  if (redis_s == nullptr || redis_s->err) {
    cerr << "redis连接失败" << endl;
    return false;
  }
  return true;
}
// 断开链接
bool Redis::disConnect() {
  freeReplyObject(reply);
  redisFree(redis_s);
  return true;
}
// 设置键值
bool Redis::setValue(const string &key, const string &value) {
  string cmd = "set  " + key + "  " + value;
  reply = (redisReply *)redisCommand(redis_s, cmd.c_str());
  if (reply == nullptr) {
    cerr << "redis:" << cmd << "失败" << endl;
    return false;
  } else {
    return true;
  };
}
// 获取键对应的值
string Redis::getValue(const string &key) {
  string cmd = "get  " + key;
  reply = (redisReply *)redisCommand(redis_s, cmd.c_str());
  if (reply == nullptr) {
    cerr << "redis:" << cmd << "失败" << endl;
    return "false";
  } else {
    return reply->str;
  };
}
// 删除键值
bool Redis::delKey(const string &key) {
  string cmd = "del  " + key;
  reply = (redisReply *)redisCommand(redis_s, cmd.c_str());
  if (reply == nullptr) {
    cerr << "redis:" << cmd << "失败" << endl;
    return false;
  } else {
    return true;
  };
}
// 插入哈希表
bool Redis::hsetValue(const string &key, const string &field,
                      const string &value) {
  string cmd = "hset  " + key + " " + field + " " + value;
  reply = (redisReply *)redisCommand(redis_s, cmd.c_str());
  if (reply == nullptr) {
    cerr << "redis:" << cmd << "失败" << endl;
    return false;
  } else {
    return true;
  };
}
// 哈希表是否存在
bool Redis::hashexists(const string &key, const string &field) {
  string cmd = "hexists  " + key + "  " + field;
  reply = (redisReply *)redisCommand(redis_s, cmd.c_str());
  if (reply == nullptr) {
    cerr << "redis:" << cmd << "失败" << endl;
    return false;
  } else {
    if (reply->integer == 0) {
      return false;
    } else {
      return true;
    }
  };
}
// 获取对应的hash_value
string Redis::gethash(const string &key, const string &field) {
  string cmd = "hget  " + key + "  " + field;
  reply = (redisReply *)redisCommand(redis_s, cmd.c_str());
  if (reply == nullptr) {
    cerr << "redis:" << cmd << "失败" << endl;
    return "false";
  } else {
    return reply->str;
  };
}
// 从哈希表删除指定的元素
bool Redis::delhash(const string &key, const string &field) {
  string cmd = "hdel  " + key + "  " + field;
  reply = (redisReply *)redisCommand(redis_s, cmd.c_str());
  if (reply == nullptr) {
    cerr << "redis:" << cmd << "失败" << endl;
    return false;
  } else {
    return true;
  };
}

int Redis::hlen(const string &key) { // 返回哈希表中的元素个数
  string cmd = "hlen  " + key;
  reply = (redisReply *)redisCommand(redis_s, cmd.c_str());
  if (reply == nullptr) {
    cerr << "redis:" << cmd << "失败" << endl;
    return -1;
  } else {
    return reply->integer;
  };
}

redisReply **Redis::hkeys(const string &key) {
  string cmd = "hkeys  " + key;
  reply = (redisReply *)redisCommand(redis_s, cmd.c_str());
  if (reply == nullptr) {
    cerr << "redis:" << cmd << "失败" << endl;
    return nullptr;
  } else {
    return reply->element;
  };
}

int Redis::scard(const string &key) // 返回set集合里的元素个数
{
  string cmd = "scard  " + key;
  reply = (redisReply *)redisCommand(redis_s, cmd.c_str());
  if (reply == nullptr) {
    cerr << "redis:" << cmd << "失败" << endl;
    return -1;
  } else {
    return reply->integer;
  };
}
int Redis::saddvalue(const string &key, const string &value) // 插入到集合
{
  string cmd = "sadd  " + key + "  " + value;
  reply = (redisReply *)redisCommand(redis_s, cmd.c_str());
  if (reply == nullptr) {
    cerr << "redis:" << cmd << "失败" << endl;
    return -1;
  } else {
    return reply->type;
  };
}
int Redis::sismember(const string &key, const string &value) // 查看数据是否存在
{
  string cmd = "sismember  " + key + "  " + value;
  reply = (redisReply *)redisCommand(redis_s, cmd.c_str());
  if (reply == nullptr) {
    cerr << "redis:" << cmd << "失败" << endl;
    return -1;
  } else {
    if (reply->integer == 0) {
      return false;
    } else {
      return true;
    }
  };
}
int Redis::sremvalue(const string &key,
                     const string &value) // 将数据从set中移出
{
  string cmd = "srem  " + key + "  " + value;
  reply = (redisReply *)redisCommand(redis_s, cmd.c_str());
  if (reply == nullptr) {
    cerr << "redis:" << cmd << "失败" << endl;
    return -1;
  } else {
    return reply->integer;
  };
}
redisReply **Redis::smembers(const string &key) {
  string cmd = "smembers  " + key;
  reply = (redisReply *)redisCommand(redis_s, cmd.c_str());
  if (reply == nullptr) {
    cerr << "redis:" << cmd << "失败" << endl;
    return nullptr;
  } else {
    return reply->element;
  };
}

int Redis::lpush(const string &key, const string &value) {
  string cmd = "lpush  " + key + " " + value;
  reply = (redisReply *)redisCommand(redis_s, cmd.c_str());
  if (reply == nullptr) {
    cerr << "redis:" << cmd << "失败" << endl;
    return -1;
  } else {
    return reply->type;
  };
}
int Redis::llen(const string &key) {
  string cmd = "llen  " + key;
  reply = (redisReply *)redisCommand(redis_s, cmd.c_str());
  if (reply == nullptr) {
    cerr << "redis:" << cmd << "失败" << endl;
    return -1;
  } else {
    return reply->integer;
  };
}

redisReply **Redis::lrange(const string &key) // 返回所有消息
{
  string cmd = "lrange  " + key + "  0" + "  -1";
  reply = (redisReply *)redisCommand(redis_s, cmd.c_str());
  if (reply == nullptr) {
    cerr << "redis:" << cmd << "失败" << endl;
    return nullptr;
  } else {
    return reply->element;
  };
}

redisReply **Redis::lrange(const string &key, string a,
                           string b) // 返回指定的消息记录
{
  string cmd = "lrange  " + key + "  " + a + "  " + b;
  reply = (redisReply *)redisCommand(redis_s, cmd.c_str());
  if (reply == nullptr) {
    cerr << "redis:" << cmd << "失败" << endl;
    return nullptr;
  } else {
    return reply->element;
  };
}

int Redis::ltrim(const string &key) // 删除链表中的所有元素
{
  string cmd = "ltrim  " + key + " 1 " + " 0 ";
  reply = (redisReply *)redisCommand(redis_s, cmd.c_str());
  if (reply == nullptr) {
    cerr << "redis:" << cmd << "失败" << endl;
    return -1;
  } else {
    return reply->type;
  };
}

#endif
