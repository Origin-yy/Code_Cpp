#include <nlohmann/json.hpp>
#include <iostream>
using namespace std;
using json = nlohmann::json;

int main(){
    // string test = "qwerqwer\nqwerqwer\nqwrqw";
    // string test_json1;

    // json jn = test;                 // json格式字符串和string格式字符互相赋值不改变
    // string test_json = jn.dump();   // dump返回json格式的字符串,与上等价（.....）

    // jn.get_to(test_json1);       // go_to赋值string格式的字符串    得到原字符串
    // string test_json2 = jn.get<string>();    // 返回string格式的字符串，与上等价

    // cout << "test:  " << test<< endl;
    // cout << "jn : "  << jn << endl;
    // cout << "test_json :  " << test_json <<endl;
    // cout << "test_json1 :  " << test_json1 <<endl;
    // cout << "test_json2 : "  << test_json2 <<endl;

    string test;
    string test1;
    cin >> test;
    if(cin.eof()){
        cout << "jixu1" <<endl;
        cin.clear();
        cin.setstate(std::_S_goodbit);
        cin.sync(); 
        cout << "jixu2" <<endl;
        cin >> test1;
        cout << "jixu3" <<endl;
    }
    return 0;
}

// 判断是不是通知套接字，是就加到对应的用户信息里，后边不执行
                int isRecvFd = 0;
                if(command_string.size() == 4){
                    for(auto c :command_string){
                        if(isdigit(c)){
                            isRecvFd++;
                        }
                    }
                }
                if(isRecvFd == 4){
                    redis.hsetValue(command_string, "通知套接字", to_string(ep[i].data.fd));
                    redis.hsetValue("fd-uid对应表", to_string(ep[i].data.fd), command_string);
                    continue;
                }                