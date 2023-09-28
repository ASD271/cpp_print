This is an easy-to-use imitation of Python's print function in C++, simply copy the print.h file to use it

for example,
```c++
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
```

will output like

```
1,,2,,hello     [1,2,3,45],1

{1:0,15:xx,33:64} | hello | [1,15,18] |   | [1,2,3] | []

\\in test.txt
hello 1110000 13
```

