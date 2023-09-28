#pragma once

#include <iostream>
#include <tuple>
#include <vector>
#include <stack>
#include <map>
#include <bitset>
#include <queue>
#include <unordered_map>

struct Params {
    std::string sep = " ";
    std::string end = "\n";
    std::ostream *out = &std::cout;
};
namespace pstyle {
    template<class T>
    void _print(const std::stack<T> &x, const Params &params = {}) {
        std::stack<T> s1(x), s2;
        while (!s1.empty()) {
            s2.push(s1.top());
            s1.pop();
        }
        *(params.out) << '[';
        bool first= true;
        while (!s2.empty()) {
            if (!first) *(params.out) << ',';
            first = false;
            *(params.out) << s2.top() ;
            s2.pop();
        }
        *(params.out) << ']';
    }

    template<class T>
    void _print(const std::queue<T> &x, const Params &params = {}) {
        auto q2 = x;
        *(params.out) << '[';
        bool first= true;
        while (!q2.empty()) {
            if (!first) *(params.out) << ',';
            first = false;
            *(params.out) << q2.front();
            q2.pop();
        }
        *(params.out) << ']';
    }

    template<class Ta, class Tb>
    void _print(const std::map<Ta, Tb> &m, const Params &params = {}) {
        *(params.out) << '{';
        bool first= true;
        for (const auto &it: m) {
            if (!first) *(params.out) << ',';
            first = false;
            *(params.out) << it.first << ':' << it.second;
        }
        *(params.out) << '}';
    }

    template<class Ta, class Tb>
    void _print(const std::unordered_map<Ta, Tb> &m, const Params &params = {}) {
        *(params.out) << '{';
        bool first= true;
        for (const auto &it: m) {
            if (!first) *(params.out) << ',';
            first = false;
            *(params.out) << it.first << ':' << it.second;
        }
        *(params.out) << '}';
    }

    template<size_t s>
    void _print(const std::bitset<s> &b, const Params &params = {}) {
        bool hasOne = false;
        for (int i = s; i >= 0; i--) {
            if (b[i]) hasOne = true;
            if (hasOne) {
                *(params.out) << b[i];
            }
        }

    }

    template<std::size_t I = 0, typename... Ts>
    void _print(const std::tuple<Ts...> &tuple, const Params &params = {}) {
        if constexpr (I < sizeof...(Ts)) {
            *(params.out) << std::get<I>(tuple);
            if constexpr (I != sizeof...(Ts) - 1) *(params.out) << ',';
            _print<I + 1>(tuple);
        }
    }

    template<typename Ta, typename Tb>
    void _print(const std::pair<Ta, Tb> &x, const Params &params = {}) {
        *(params.out) << x.first << ',' << x.second;
    }

    template<typename T, typename=void>
    struct has_begin_end : std::false_type {
    };
    template<typename T>
    struct has_begin_end<T, std::void_t<decltype(std::declval<T>().begin()), decltype(std::declval<T>().end())>>
            : std::true_type {
    };

    template<typename Ts>
    void _print(const Ts &args, const Params &params = {}) {
        if constexpr (std::is_same_v<Ts, std::string> || !has_begin_end<Ts>::value) {
            *(params.out) << args;
        } else {
            bool first = true;
            *(params.out) << '[';
            for (const auto &i: args) {
                if (!first) *(params.out) << ',';
                first = false;
                *(params.out) << i;

            }
            *(params.out) << ']';
        }
    }
}

void sep(const Params &p) {
    *(p.out) << p.sep;
}
void print(const Params &p) {
    *(p.out) << p.end;
}

void print() {
    std::cout << "\n";
}

template<typename T, typename... Ts>
void print(const T &arg, const Ts &... args) {
    if constexpr (sizeof...(args) > 0) {
        const auto &last = (args, ...);
        if constexpr (std::is_same_v<decltype(last), const Params &>) {
            pstyle::_print(arg, last);
            if constexpr (sizeof...(args) > 1) {
                sep(last);
            }
        } else {
            pstyle::_print(arg);
            sep({});
        }
        print(args...);
    } else {// this function will be called only when params is not settle
        pstyle::_print(arg);
        print();
    }
}


