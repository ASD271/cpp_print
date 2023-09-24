an easily used print function wrapped cout

```c++
bitset<8> b(string("01110000"));
print("hello",b,13);

print();

map<int,string> x; x[1]="0"; x[15]="xx"; x[33]="64";
stack<int> s; s.push(1); s.push(15); s.push(18);
std::queue<int> q;    q.push(1);    q.push(2);    q.push(3);
print(x,"hello",s,' ',q,vector<int>{7,8,9});
```

will output like

```
hello 1110000 13

1:0 15:xx 33:64 hello 1 15 18   1 2 3 7 8 9


```

