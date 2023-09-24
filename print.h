#include <iostream>
#include <tuple>
#include <vector>
#include <stack>
#include <map>
#include <bitset>
#include <queue>
#include <unordered_map>

namespace pstyle {
    template<class T>
    void _print(const std::stack<T> &x) {
        std::stack<T> s1(x), s2;
        while (!s1.empty()) {
            s2.push(s1.top());
            s1.pop();
        }
        while (!s2.empty()) {
            std::cout << s2.top() << ' ';
            s2.pop();
        }
    }

    template<class T>
    void _print(const std::queue<T> &x) {
        auto q2 = x;
        while (!q2.empty()) {
            std::cout << q2.front() << " ";
            q2.pop();
        }
    }

    template<class Ta, class Tb>
    void _print(const std::map<Ta, Tb> &m) {
        for (const auto it: m) {
            std::cout << it.first << ':' << it.second << ' ';
        }
    }

    template<class Ta, class Tb>
    void _print(const std::unordered_map<Ta, Tb> &m) {
        for (const auto it: m) {
            std::cout << it.first << ':' << it.second << ' ';
        }
    }

    template<size_t s>
    void _print(const std::bitset<s> &b) {
        bool hasOne = false;
        for (int i = s; i >= 0; i--) {
            if (b[i]) hasOne = true;
            if (hasOne) {
                std::cout << b[i];
            }
        }
        std::cout << ' ';
    }

    template<std::size_t I = 0, typename... Ts>
    void _print(const std::tuple<Ts...> &tuple) {
        if constexpr (I < sizeof...(Ts)) {
            std::cout << std::get<I>(tuple) << " ";
            _print<I + 1>(tuple);
        }
    }

    template<typename T, typename=void>
    struct has_begin_end : std::false_type {
    };
    template<typename T>
    struct has_begin_end<T, std::void_t<decltype(std::declval<T>().begin()), decltype(std::declval<T>().end())>>
            : std::true_type {
    };

    template<typename Ts>
    void _print(const Ts &args) {
        if constexpr (std::is_same_v<Ts, std::string> || !has_begin_end<Ts>::value) {
            std::cout << args << ' ';
        } else {
            for (const auto &i: args) { std::cout << i << ' '; }
        }
    }
}


template<typename T, typename... Ts>
void print(const T &arg, const Ts &... args) {
    pstyle::_print(arg);
    if constexpr (sizeof...(args) == 0) { std::cout << '\n'; } else { print(args...); }
}

template<typename ...T>
typename std::enable_if<sizeof...(T) == 0>::type print() {
    std::cout << "\n";
}