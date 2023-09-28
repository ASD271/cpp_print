#include "print.h"
#include <fstream>
using namespace std;

int main() {
    print(1, 2, "hello", Params{.sep=",,", .end="\t"});
    print(vector<int>{1, 2, 3, 45}, 1,Params{.sep=","});
    print();

    map<int,string> x; x[1]="0"; x[15]="xx"; x[33]="64";
    stack<int> s; s.push(1); s.push(15); s.push(18);
    std::queue<int> q;    q.push(1);    q.push(2);    q.push(3);
    print(x,"hello",s,' ',q,vector<int>{},Params{.sep=" | "});

    std::ofstream file("test.txt");
    bitset<8> b(string("01110000"));
    print("hello",b,13,Params{.out=&file});

    vector<vector<int>> vs{{1,2,3},{4,5,6},{7,8,9}};
    print(vs);
}