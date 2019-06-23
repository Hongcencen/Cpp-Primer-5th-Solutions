//String类
#include <iostream>
using std::cin; using std::cout; using std::endl; using std::cerr;
#include <string>
using std::string;
#include <memory>
using std::allocator; using std::uninitialized_copy;
#include <utility>
using std::pair;
#include <initializer_list>
using std::initializer_list;

//类string类的实现，底层使用char
class String{
    friend bool operator==(const String &, const String &);
    friend bool operator!=(const String &, const String &);
    friend bool operator<(const String &, const String &);
    friend bool operator>(const String &, const String &);
public:
    String() :  //默认构造函数，allocator成员进行默认初始化
            elements(nullptr), first_free(nullptr), cap(nullptr){};
    String(const char *);       //接受C风格字符串指针的构造函数
    String(initializer_list<char>); //接受一个initializer_list<char>参数的构造函数
    String(const String &);     //拷贝构造函数
    String &operator=(const String &);  //拷贝赋值运算符
    ~String();                  //析构函数
    void push_back(const char &);   //拷贝元素
    size_t size() const { return first_free - elements;}
    size_t capacity() const { return cap - elements;}
    bool empty() const { return cbegin() == cend();}
    char *begin() const { return elements;}
    const char *cbegin() const { return elements;}
    char *end() const { return first_free;}
    const char *cend() const { return first_free;}
    void reserve(size_t);       //调整预留空间
    void resize(size_t);        //调整大小
    void resize(size_t, const char &);  //调整大小

private:
    static allocator<char> alloc;   //分配元素
    //被添加元素的函数所使用
    void chk_n_alloc() {if (size() == capacity()) reallocate();}
    //工具函数，被拷贝构造函数、拷贝赋值运算符和析构函数所使用
    pair<char *, char *> alloc_n_copy(const char *, const char *);
    void free();            //销毁元素并释放内存
    void reallocate(size_t);      //获得更多元素并拷贝已有元素(带参数)
    void reallocate();            //获得更多元素并拷贝已有元素(无参数)
    char *elements;         //指向数组首元素的指针
    char *first_free;       //指向数组第一个空闲元素的指针
    char *cap;              //指向数组尾后位置的指针
};

allocator<char> String::alloc;

inline
void String::push_back(const char &c) {
    chk_n_alloc();
    alloc.construct(first_free++, c);
}

pair<char *, char *> String::alloc_n_copy(const char *b, const char *e) {
    auto data = alloc.allocate(e - b);
    return {data, uninitialized_copy(b, e, data)};
}

void String::free(){
    if (elements){
        for (auto p = first_free; p != elements;)
            alloc.destroy(--p);
        alloc.deallocate(elements, cap - elements);
    }
}

String::String(const String &rhs) {
    auto newdata = alloc_n_copy(rhs.begin(), rhs.end());
    elements = newdata.first;
    first_free = cap = newdata.second;
}

String::~String() { free();}

String &
String::operator=(const String &rhs) {
    auto data = alloc_n_copy(rhs.begin(), rhs.end());
    free();
    elements = data.first;
    first_free = cap = data.second;
    return *this;
}

void String::reallocate(size_t n) {
    auto newcapacity = n;
    auto newdata = alloc.allocate(newcapacity);
    auto dest = newdata;
    auto elem = elements;
    for (size_t i = 0; i != size(); ++i)
        alloc.construct(dest++, std::move(*elem++));
    free();
    elements = newdata;
    first_free = dest;
    cap = elements + newcapacity;
}

void String::reallocate() {
    auto newcapacity = size() ? 2 * size() : 1;
    auto newdata = alloc.allocate(newcapacity);
    auto dest = newdata;
    auto elem = elements;
    for (size_t i = 0; i != size(); ++i)
        alloc.construct(dest++, std::move(*elem++));
    free();
    elements = newdata;
    first_free = dest;
    cap = elements + newcapacity;
}

String::String(const char *cc) : String() {
    while (cc && *cc != '\0')
        push_back(*cc++);
}

String::String(initializer_list<char> ilc) : String() {
    for (const auto &c : ilc)
        push_back(c);
}

void String::reserve(size_t n) {
    if (n > size()){
        reallocate(n);
    } else {
        return;
    }
}

void String::resize(size_t n) {
    if (n > size()){
        reallocate(n);
        for (int i = size() - n; i != 0; --i)
            push_back(char());
    } else if (n == size()){
        return;
    } else {
        for (int i = size() - n; i != 0; --i)
            alloc.destroy(--first_free);
        reallocate(n);
    }
}

void String::resize(size_t n, const char &c) {
    if (n > size()){
        reallocate(n);
        for (int i = size() - n; i != 0; --i)
            push_back(c);
    } else if (n == size()){
        return;
    } else {
        for (int i = size() - n; i != 0; --i)
            alloc.destroy(--first_free);
        reallocate(n);
    }
}

bool operator==(const String &lhs, const String &rhs){
    return lhs.elements == rhs.elements &&
           lhs.first_free == rhs.first_free &&
           lhs.cap == rhs.cap &&
           lhs.size() == rhs.size();
};

bool operator!=(const String &lhs, const String &rhs){
    return !(lhs == rhs);
};

bool operator<(const String &lhs, const String &rhs){
    auto it_lhs = lhs.elements, it_rhs = rhs.elements;
    for (;it_lhs != lhs.first_free && it_rhs != rhs.first_free;
          ++it_lhs, ++it_rhs){
        if (*it_lhs < *it_rhs){
            return true;
        } else {
            return false;
        }
    }
    return !(it_lhs == lhs.first_free && it_rhs == rhs.first_free);
};

bool operator>(const String &lhs, const String &rhs){
    auto it_lhs = lhs.elements, it_rhs = rhs.elements;
    for (;it_lhs != lhs.first_free && it_rhs != rhs.first_free;
          ++it_lhs, ++it_rhs){
        if (*it_lhs > *it_rhs){
            return true;
        } else {
            return false;
        }
    }
    return !(it_lhs == lhs.first_free && it_rhs == rhs.first_free);
};

int main(){
    String s;
    for (int i = 0; i != 10; ++i)
        s.push_back('a');
    cout << s.size() << endl;
    s.reserve(50);
    cout << s.size() << endl;
    s.resize(100, 'b');
    cout << s.size() << endl;
    s.resize(10);
    cout << s.size() << endl;
}