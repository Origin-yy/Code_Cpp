#include <nlohmann/json.hpp>
#include <iostream>
using namespace std;
using json = nlohmann::json;

int main(){
    // string test1;
    // cin >> test;
    // if(cin.eof()){
    //     cout << "jixu1" <<endl;
    //     cin.clear();
    //     cin.setstate(std::_S_goodbit);
    //     cin.sync(); 
    //     cout << "jixu2" <<endl;
    //     cin >> test1;
    //     cout << "jixu3" <<endl;
    // }
    string asdf = "add";
    if(asdf.find("add-friend") == 0){
        cout << "ok" << endl;
    }else{cout << "k" << endl;}





    return 0;
}
