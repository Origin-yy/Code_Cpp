#include <iostream>
#include <string>
// 使用 nlohmann_json 库只需包含即可, 无需链接
#include <nlohmann/json.hpp>
namespace OVL {
// 注意 From_Json 函数 To_Json 对象必须在同一 namespace 中
class User {
public:
  std::string name;
  uint64_t age;
  std::string number;
  std::string password; // JSON => class
  static void From_Json(const nlohmann::json &jn, User &user) {
    jn.at("name").get_to(user.name);
    jn.at("age").get_to(user.age);
    jn.at("number").get_to(user.number);
    jn.at("password").get_to(user.password);
  }
  // class => JSON
  static void To_Json(nlohmann::json &jn, const User &user) {
    jn = nlohmann::json{{"name", user.name},
                        {"age", user.age},
                        {"number", user.number},
                        {"password", user.password}};
  }
  void PrintInfo() {
    printf("name = %s\nage = %ld \nnumber = %s \npassword = %s\n", name.c_str(),
           age, number.c_str(), password.c_str());
  }
};
} // namespace OVL
int main(void) {
  // 2. 显示的调用 json::parse 推荐
  auto jn_2 = nlohmann::json::parse(R"(
{
"name": "xiao_ming",
"age": 12,
"number": "123",
"password": "234"
}
)");
  OVL::User xiao_ming;
  OVL::User::From_Json(jn_2, xiao_ming);
  xiao_ming.PrintInfo();
  nlohmann::json xiao_ming_json;
  OVL::User::To_Json(xiao_ming_json, xiao_ming);
  // nlohmann::json.dump 函数返回一个 json 字符串
  std::string xiao_ming_json_string = xiao_ming_json.dump();
  printf("%s\n", xiao_ming_json_string.c_str());
  return 0;
}
