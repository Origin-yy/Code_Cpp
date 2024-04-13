#include <iostream>
#include <nlohmann/json.hpp>
#include <ostream>
using namespace std;
using json = nlohmann::json;

int main() {
  string test = "a d d fd ;";
  json jn = test;
  string test1 = jn.dump();
  json test2 = test2.parse(test1);

  string test3;
  jn.get_to(test3);

  string test4 = jn.get<string>();

  cout << "test: " << test << endl
       << "jn(json): " << jn << endl
       << "(jn.dump)test1(string): " << test1 << endl
       << "(test1的parse)test2(json): " << test2 << endl
       << "(jn.get_to)test3(string): " << test3 << endl
       << "(jn.get<>:)test4(string): " << test4 << endl;
  return 0;
}
