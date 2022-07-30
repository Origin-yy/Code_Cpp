#include <nlohmann/json.hpp>
#include <iostream>
using namespace std;
using json = nlohmann::json;
int main(){
    // string test = "1234qwer";
    // string test1 = "asef";

    // string test2 = test + "\n" + "7890" + test1;
    // cout << "test2:" << test2 << endl;

    // json test_json = test2;
    // cout << "test_json : " << test_json << endl;

    // string test_str = test_json.dump();
    // cout << "test_str :" << test_str << endl;

    // string to_str = json::parse(test_str);
    
    // cout << "to_str :" << to_str << endl;
    // store a string in a JSON value
    // json j_string = "this is a string";

    // string cpp_string = j_string.get<string>();
    // string cpp_string2;
    // j_string.get_to(cpp_string2);

    // string serialized_string = j_string.dump();

    // // output of original string
    // cout << cpp_string << " == " << cpp_string2 << " == " << j_string.get<string>() << '\n';
    // // output of serialized value
    // cout << j_string << " == " << serialized_string << endl;


    string test = "qwerqwer\nqwerqwer\nqwrqw";
    json jn = test;                 // json格式字符串和string格式字符互相赋值不改变
    string test_json = jn.dump();   // dump返回json格式的字符串
    string test_json1;
    jn.get_to(test_json1);       // go_to赋值stringn格式的字符串
    cout << "test:  " << test<< endl;
    cout << "jn : "  << jn << endl;
    cout << "test_json :  " << test_json <<endl;
    cout << "test_json1 :  " << test_json1 <<endl;

    return 0;
}