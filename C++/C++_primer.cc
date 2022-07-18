#include <initializer_list>
#include <iostream>
#include <istream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <system_error>
#include <vector>
// #include <string>
// #include <cstdlib>
// #include <cctype>  // nullptr
// #include <cstddef> // size_t,ptrdiff_t
// #include <iterator> // begin(),end()
// #include <stdexcept> // 异常类，如runtime_error,p176
#include <cassert>  // assert,预处理宏

extern const int i = 123; // a文件里定义并初始化i，加extern使其能被其他文件使用

using namespace std; // 头文件不应包含using声明（名字冲突）

#ifdef MIN
#define MKSTR(x) #x       // "x"
#endif

#ifndef MIN
#define MIN(a, b) (a < b ? a : b)
#define concat(a, B) a##b // xy
#define NDEBUG // 定了他 assert什么都不做，没定义则指执行运行时检查
#endif

/*
 *第七章-类
 */
// 成员函数的声明必须在类的内部，定义可以在内部也可以在外部，接口组成部分的非成员函数的定义和声明都在类的外部
// 数据成员，成员函数，构造函数顺序无所谓，编译器先编译数据成员在编译其他
// 类的拷贝，赋值和析构（销毁）暂且使用合成的
class Sales_data {
  // 为非成员函数（接口函数）加上友元声明,不算函数声明，只是说明访问权限
  friend Sales_data add(const Sales_data& ,const Sales_data&);
  friend istream &read(istream &is, Sales_data &item);
  friend ostream &print(ostream &os, const Sales_data &item);

  private:  // 以下的成员可以被成员函数访问，但是不能被使用类的代码访问

  // 数据成员
  string bookNo;
  unsigned units_sold = 0;   // 类内初始值
  double revenue = 0.0;      // 类内初始值
  mutable int kebian = 0;    // 前加mutable后，即使在一个const的类里，也可以改变它的值
  static double interestrate;// 与类绑定的数据成员，与对象无关，所有类共享，一起变  
  static void initrate() { interestrate = 0; };    //  

  public:   // 以下成员可以在整个程序内使用
  static double rate() { return interestrate; };   // 与类绑定的成员函数，不包含this指针，不能声明为const
  static void change_rate(double a) {interestrate = a;}; // 与类绑定的成员函数，不包含this指针，不能声明为const
  // 成员函数，p231，额外的this隐式参数（调用函数的对象的地址）帮助函数得到调用对象的地址，从何访问其数据成员
  string isbn() const { return bookNo; }  // 调用函数的对象的地址被隐式的绑定到this上
  Sales_data& combine(const Sales_data&); // 声明在内，定义在外
  double avg_price() const { return units_sold ? revenue/units_sold : 0; }
  // const把this指针改为指向常量（原来不是指向常量）的常量指针
  // 如果不加const导致无法访问const的对象（不能用一个常量对象的地址（const的对象）初始化一个指向非常量的常量指针（this））
  // 这样使用const的成员函数称作常量成员函数

  // 构造函数a，不定义的话有一个默认构造函数，用类内初始值，没有则默认初始化
  Sales_data() = default;    // 需要一个默认构造函数，凡是()为空就是默认构造函数，无参数时用它，默认初始化
  // 需要一个string参数s，用它来初始化bookNo，剩下俩用默认构造函数初始化，有类内初始值，用类内初始值初始化
  Sales_data(const string &s) : bookNo(s) {} // 等价于Sales_data(const string &s) : bookNo(s),units_sold(0),revenue(0) {}
  // 需要三个参数，这三个参数分别用来初始化三个变量
  Sales_data(const string &s, unsigned n, double p) : bookNo(s), units_sold(n),revenue(p*n) {}
  // ：和{}中间的是构造函数初始值列表，用()内的值（由参数列表得到的）初始化前面的变量
  // 如果成员是const引用等没有默认构造函数的类类型，不能在{}里赋值初始化，只能用构造函数初始值列表
  Sales_data(istream &); // 以istream为参数的构造函数需要在{}内执行一些是自己操作来为成员赋值，可以在内声明，在外定义

  /*
    委托构造函数，p261
    Sales_data() : Sales_data("", 0, 0) {}
    Sales_data(std::string s): Sales_data(s, 0, 0) {}
    Sales_datra(std::istrea, &is): Sales_data() { read(is, *this); }  委托给默认（0参），默认给3参，再执行{}
  */
};

// 成员函数可以定义在函数外
Sales_data& Sales_data::combine(const Sales_data &rhs){  // ::说明在哪个类的作用域里
  units_sold += rhs.units_sold;   // 直接访问这个类对象的成员
  revenue += rhs.revenue;
  return *this;     // 返回调用该函数的对象（*this），返回*this的函数的返回值应为引用，一个const的成员函数返回的*this是一个常量引用
}

// 类的接口函数的声明应该与类声明（而非定义）在同一个头文件内
istream &read(istream &is, Sales_data &item){  // io类不能拷贝，所以只能用引用来传递他们
  double price = 0;
  is >> item.bookNo >> item.units_sold >> price;
  item.revenue = price * item.units_sold;
  return is;
}
ostream &print(ostream &os, const Sales_data &item){
  os << item.isbn() << " " << item.units_sold << " "
     << item.revenue << " " << item.avg_price();
  return os;
}
Sales_data add(const Sales_data &lhs, const Sales_data &rhs){
  Sales_data sum = lhs;
  sum.combine(rhs);
  return sum;
}

// 在类外定义构造函数 Sales_data:: 指明作用域（哪个类），成员名和类名相同，所以是一个构造函数
Sales_data :: Sales_data(istream &is){ // 构造函数初始值列表是空的，但执行了构造函数体，所以对象成员依旧初始化
  read(is, *this);  // read函数从is中读取一条交易信息然后存入this对象中
}

class Screen{
  friend class Sales_data;  // 把一个类设为友元，sales_data类可以访问Screen的私有数据成员
  friend string isbn() ; // 把一个类的成员函数设为友元,p252
  public:
  // 根据对象是否const重载display函数
  Screen display(ostream &os)
    { do_display(os); return *this; }
  const Screen display(ostream &os) const
    { do_display(os); return *this; }

  private:
  // 该函数负责显示Screen的内容
  void do_display(ostream &os) const { /*展示内容*/os << 'a'; }
  // 该成员函数展示后，根据参数的对象是否const进行调用，返回const引用或者非const引用
};






// 默认实参，前面有了，后边也得有，调用时省略即用默认实参，且只能省略后边的。
int screen(int ht = 10, int wid = 10, string backgrnd = "black"); 

void reset(int &i){
  i = 0;
}
void print(int (&arr)[10]){   // 数组引用形参
  for(auto elem : arr)
    cout << elem << endl;
}

// initialezer_list<T>    类似于vector，有多个T类型元素，但是是常量，用{}含多个元素传参
void error_msg(error_code e, initializer_list<string> il){
  cout << e.message().c_str() << ": ";
  for (const auto &elem : il)
    cout << elem << " ";
  cout << endl;
}

// auto func(int i) -> int(*)[10] 等同于下面的声明，接收一个int参数，返回一个指向含有10个int的数组的指针
int (*func(int i))[10]{
  int (*arr)[10];
  return arr;  // 为了举例子，不能返回一个局部变量的引用或者指针，函数结束后，他已经没了
}

// 实参必须是const string引用，普通的不行，
// inline,函数前加上inline即成为内联函数，在编译时展开，开销小，把内联和constexpr函数定义在头文件
// constexpr,函数加上constexpr即成为constexpr函数，编译时验证函数是否返回常量表达式
const string &shortString(const string &s1, const string &s2){
  return s1.size() <= s2.size() ? s1 : s2;
}
// 重载函数，普通的也可以，实参不是常量时调用，加const后调用const版本，再将返回值（一个const引用，但实际上这个const引用绑在了非const实参上，所以去const是安全的）去const
string &shortString(string &s1,string &s2) {
  auto &r = shortString(const_cast<const string&>(s1), const_cast<const string &>(s2));
  return const_cast<string&>(r);
}

// 函数类型，函数指针，函数指针形参
bool lengthCompare(const string &, const string &);

typedef bool Func(const string &,const string &);
typedef decltype(lengthCompare) Func;            // 与上等价

typedef bool (*FuncP)(const string &, const string &);
typedef decltype(lengthCompare) *FuncP2;         // 与上等价，decltype作用于函数，返回函数类型，需要显式加变指针

void useBigger(const string &s1, const string &s2, bool(*pf)(const string &, const string &));
void useBigger(const string &s1 ,const string &s2, FuncP2/*Func，Func2，FuncP等价*/); // 使用函数类型（指针）别名，与上面等价
// 函数（指针）类型包含两个要素，一是返回值，二是形参列表

// 返回指向函数的指针，不能返回一个函数类型，但可以返回一个函数指针
using F = int(int*, int);       // F是函数类型
using PF  = int (*)(int* ,int); // FP是函数指针类型
int (*f1(int))(int*,int);
PF f2(int);   // 与上等价
PF *f3(int);  // 与上等价
auto f4(int) ->int (*)(int* ,int); // 尾置返回类型，与上等价

int main() {
  extern const int i; // a文件里声明i而非定义i，extern又表示i并非本文件独有，他的定义将在别处出现
  int const &ref = i; // 引用必须初始化,用于初始化引用的必须是一个对象,且只能绑定一个。
  // 引用只是创建一个别名，没有创建对象，没有引用的引用。
  // const的值必须初始化，只在当前所在文件内有效，不同文件不共享
  // 在每个文件的每个const前加extern就可以共享一个，包括定义
  const int m = 0; // m是一个常量表达式，编译时已知且不变，const int n = get_size()不是，运行时才知道值。
  constexpr int mf = 10; // 声明为constexpr的变量一定是一个常量，必须用常量表达式（值不变，编译过程中就得到结果）初始化
  using zhengxing = int; 
  typedef char *pstring;
  const pstring cstr = 0; // 不要替换为原样，这里是声明一个指向char的指针常量（指针不可修改），而不是指向的char不可修改
  struct sale{
    string bookname;  // 默认初始化，类内初始值，不能用
    int id = 0;
  };
  sale a,c;

  // string 初始化
  string s;
  string *ptr = nullptr; // 不能把int类型变量赋给指针，即使值为0。
  string s1("this is a string");
  string s2(10, 'c');
  string s3; // 空字符串
  string s4 = s1;
  s3 = s2;
  // string s4(s2);
  string s5 = s1;
  // string操作
  if(s1 == s2 || s1.empty() || s2.size() == 0 || s2[4] == 'c')
  {
    getline(cin,s);
    string s7 = s1 + "asdf"; // 每个+号两侧的运算对象至少有一个是string
    cout << "string s1中的字符串是" << s1 << '\n' << "对s1做sizeof操作得到: " << sizeof(s1) << endl;
    cin >> s2;
    bool if_empty = s1.empty();
  }
  // 反复读取，直到遇到文件结尾
  // while (cin >> s)
    // cout << s << endl;
  // 读取一行输入,读取\n并丢弃，并且终止输入
  string line;
  // while (getline(cin, line))// 读取一行，包括空白符
  //  cout << line << endl;
  auto line_t = line.size();
  // auto类型，让编译器来推断类型，这里是string::size_type;
  // s.size()的返回值是一个无符号整数，不要与int相比
  int n = -1; // 有符号和无符号在一起，都转为无符号
  // int t = s.size() < n; // 几乎肯定为true，负值n会转为一个比较大的无符号值

  // 范围for语句， 想要修改string中字符的值，要用引用
  for(auto &c :s1){
    c = toupper(c);
  }
  string::size_type index = 0;// 下标类型，无符号整数

  // decltype关键字
  decltype(s.size()) cnt = 0; // 根据（）里变量的类型为类型

  // vector模板
  vector<int> ivec(10,2);
  vector<int> svec(10);  // 十个元素，每个初始化为0，值初始化，各种初始化详见p89
  vector<vector<string> > file{{"asdf"," qwer","qsfe"},{"esed"}};
  // .push_back()把元素放到vector尾端
  vector<string> words;
  for(int i = 0; i<10; i++){
    string buf = "asdf";
    words.push_back(buf);
  }
  // 迭代器指向容器中的某个元素
  vector<string>::iterator e =words.end(); // 指向容器的尾后元素,end返回的不能解引用或递增减
  for(auto it = words.begin();it != words.end() && !isspace((*it)[0]);++it)
    (*it)[0] = toupper((*it)[0]);

  // 迭代器二分搜索
  vector<int> text{1,2,3,4,5,6,7,8,9,10};
  int sought = 8;
  auto beg1 = text.begin(),end1 = text.end();
  auto mid = text.begin() + (end1 - beg1)/2;   // 中间点
  while (mid != end1 && *mid != sought){
    if(sought < *mid)
      end1 = mid;
    else 
      beg1 = mid + 1;
    mid = beg1 + (end1 - beg1)/2;
  }
  // 数组
  char ch[] = "C++";// 自动补‘\0’
  int ia[mf] = {0,1,2,3,4,5,6,7,8,9};  // 数组维度必须是常量表达式
  size_t nu = 0; // 数组下标类型
  int *beg = begin(ia); // 标准库函数begin和end返回数组头尾（下一个位置）的指针
  int *last = end(ia);  // 指向尾元素下一个位置的指针，不存在元素，但可以得到地址，不能解引用，或者递增
  // 内置（数组）的下标可以是负值，vector和string是无符号
  // const char *buf = s.c_str（）;  string的成员函数c_str把string类转为字符串（返回const char*）
  vector<int> wvec(begin(ia),end(ia)); // 用数组初始化vector，给他收尾指针
  // 范围for处理多维数组都用引用

  void* p = &s;
  string* q = static_cast<string*>(p);   // 强制类型转换,p145去底层const，const_cast
  // try和throw处理异常
  int t, u;
  while(cin >> t >> u ){
    try{
      int r = u + t;
      if(r != 0)
        throw runtime_error("输入必须是两个0\n");  // 出现异常会终止当前的代码，跳到能处理的地方
      cout << r << endl;
    }catch(runtime_error err){
      cout << err.what() << "再试一次？输入'y'or'n'\n";
      char o;  // err是一个runtime_error类，what()成员函数返回初始化这个具体对象时用的字符串
      cin >> o;
      if( !cin || o == 'n')
        break;
    }
  }

  static int v = 0;// 内置类型静态局部变量初始化为0
  reset(t);// ，只能传int类型对象，不能用字面值，const int等。尽量使用常量引用，可以传字面值和const int
  // 想要改变实参的值时，最好传引用参数(即形参是引用)，通过引用改变其值而不是指针,而且引用不拷贝,有些类型不支持拷贝，比较高效
  string expected = "qipan",actual(10,'c');
  error_code ec(1, system_category());
  if(expected != actual)
    error_msg(ec, {"functionX",expected, actual});
  // 不要返回局部变量的引用或者指针
  int y = 0;
  int (*arry)[10] = func(y);
  cout << actual.size() << endl;
  assert(actual.size() == 10);// 当()内表达式为假，输出信息并终止程序，为真则什么都不做

  // 函数类型等价于函数指针，有函数类型的形参，函数类型就是函数指针
  bool (*pf)(const string &, const string &); // pf是一个函数指针，指向一个有这样的形参列表，返回值是bool的函数，未初始化
  pf = lengthCompare; // 为pf赋值，等价于pf = &lengthCompare,函数名作为值使用时，自动转为指针
  bool b1 = pf("hello", "goodbye"); // 等价于bool b1 = (*pf)("hello","goodbye");
  

/*
 *第七章
 */
  Sales_data obj;  // 声明了一个对象；执行默认构造函数初始化，末尾不能加()
  Sales_data obj2("s"); // 声明了一个对象，用构造函数里的一个参数的构造函数初始化
  obj.combine(actual);// 发生了隐式类类型转换，这里actual是一个字符串，类内声明式一个参数的构造函数前加explicit避免隐式类类型转换，定义时不加
                          // 隐式的调用了一个参数的构造函数初始化了一个临时类，然后用调用combine，用这个临时类做参数
  //C中的结构体叫聚合类




  return 0;
}


/*
  #ifndef SALES_DATA_H
  #define SALES_DATA_H
  不包含using声明
  #include <string>
  #include <iostream>
  Sales_data.h：
  (1)类：
    友元的声明
    publc:
      构造函数声明或定义
      接口函数声明或定义
    private：
      数据成员
  (2)友元函数不带friend的声明
  (3)内联函数定义和constexpr函数定义
  (4)其他函数的声明
  #endif


  Sales_data.cc
  #include <iostream>
  #include "Sales_data.h"
  using namesp std;
  (1)类相关：
    构造函数定义（用户调用，根据给的信息，初始化一个类）
    成员函数定义（类调用，使用调用它的类的数据成员和给的参数（可以是类或空,可以访问参数的类和this的类的数据成员），进行一些操作，返回一个对象（需要的话））
    非成员函数定义（用户调用，使用用户给它的参数（可以是类或空），进行一些操作，返回一个对象（需要的话））
  (2)其他函数定义
 */