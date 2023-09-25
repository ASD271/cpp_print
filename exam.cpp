#include <iostream>
#include <vector>
#include "print.h"
#include <bitset>
#include <queue>

using namespace std;

int main() {
    bitset<8> b(string("01110000"));
    auto c=std::make_pair(16,"xx");
    print("hello",b,13,c);

    print();

    map<int,string> x; x[1]="0"; x[15]="xx"; x[33]="64";
    stack<int> s; s.push(1); s.push(15); s.push(18);
    std::queue<int> q;    q.push(1);    q.push(2);    q.push(3);
    print(x,"hello",s,' ',q,vector<int>{7,8,9});


    return 0;
}