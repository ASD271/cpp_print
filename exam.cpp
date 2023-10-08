#include "print.h"
#include <fstream>
using namespace std;
class A{
    int x=1,y=2;
    friend ostream& operator<<(ostream& os,const A& a){
        os<<"this is an A instance: "<<a.x<<' '<<a.y;
        return os;
    }
};
int main() {
    A a{};
    print(a);
    print(1, 2, "hello");
    print(1, false,2, "hello");
    print();

    map<int,string> x; x[1]="0"; x[15]="xx"; x[33]="64";
    stack<int> s; s.push(1); s.push(15); s.push(18);
    std::queue<int> q;    q.push(1);    q.push(2);    q.push(3);

    std::ofstream file("test.txt");

    print(x,"hello",s,' ',q,vector<vector<int>>{{}},Params{.sep=" | ",.out=&file});
}