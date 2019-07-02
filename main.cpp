#include <iostream>
using std::cin; using std::cout; using std::endl; using std::cerr;
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <algorithm>
using std::find_if;
#include <functional>
using std::bind;
using std::modulus;

using namespace std::placeholders;

int main() {
    vector<int> vi1{2, 4, 6, 8, 10};
    vector<int> vi2{2, 4, 6, 8, 9};
    auto it_1 = find_if(vi1.begin(), vi1.end(), bind(modulus<int>(), _1, 2));
    auto it_2 = find_if(vi2.begin(), vi2.end(), bind(modulus<int>(), _1, 2));
    if (it_1 != vi1.end()){
        cout << "Can't modules by 2" << endl;
    } else {
        cout << "Could modules by 2" << endl;
    }
    if (it_2 != vi2.end()){
        cout << "Can't modules by 2" << endl;
    } else {
        cout << "Could modules by 2" << endl;
    }
}