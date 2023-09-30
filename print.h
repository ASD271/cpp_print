#pragma once

#include <iostream>
#include <tuple>
#include <vector>
#include <stack>
#include <map>
#include <bitset>
#include <queue>
#include <unordered_map>
#include <type_traits>
#include <list>
#include <forward_list>
#include <set>
#include <unordered_set>

struct Params {
    std::string sep = " ";
    std::string end = "\n";
    std::ostream *out = &std::cout;
};
namespace pstyle {

    template<typename Inst, template<typename...> typename Tmpl>
    struct is_instantiation_of : std::false_type {
    };

    template<template<typename...> typename Tmpl, typename... Args>
    struct is_instantiation_of<Tmpl<Args...>, Tmpl> : std::true_type {
    };

    template<typename T, typename U, typename... Rest>
    struct is_one_of : std::conditional_t<
            std::is_same_v<T, U>, std::true_type, is_one_of<T, Rest...>> {
    };

    template<typename T, typename U>
    struct is_one_of<T, U> : std::conditional_t<
            std::is_same_v<T, U>, std::true_type, std::false_type> {
    };

    template<typename Inst, template<typename...> typename Tmpl,
            template<typename...> typename... Rest>
    struct is_one_instantiation_of : std::conditional_t<
            is_instantiation_of<Inst, Tmpl>::value, std::true_type, is_one_instantiation_of<Inst, Rest...>> {
    };

    template<typename Inst, template<typename...> typename Tmpl>
    struct is_one_instantiation_of<Inst, Tmpl> : std::conditional_t<
            is_instantiation_of<Inst, Tmpl>::value, std::true_type, std::false_type> {
    };

    template<typename Inst, template<typename...> typename Tmpl,
            template<typename...> typename... Rest>
    const bool is_one_instantiation_of_v = is_one_instantiation_of<Inst, Tmpl, Rest...>::value;

    template<typename T>
    typename std::enable_if_t<is_one_of<std::decay_t<T>, int, long, long long, unsigned,
            unsigned long, unsigned long long,
            float, double, long double, std::string, char*, char>::value>
    _print(const T &x, const Params &params = {}) {
        *(params.out) << x;
    }

    template<typename T>
    std::enable_if_t<std::is_same_v<std::decay_t<T>,bool>>
    _print(const T& v, const Params &params = {}){
        *(params.out)<<(v?"true":"false");
    }

    template<typename Ts>
    typename std::enable_if_t<is_one_instantiation_of_v<std::decay_t<Ts>,
            std::vector, std::deque, std::list, std::forward_list,
            std::set, std::multiset, std::unordered_set, std::unordered_multiset>>
    _print(const Ts &args, const Params &params = {}) {
        bool first = true;
        *(params.out) << '[';
        for (const auto &i: args) {
            if (!first) *(params.out) << ',';
            first = false;
            _print(i,params);
        }
        *(params.out) << ']';
    }

    template<typename T>
    std::enable_if_t<
            is_one_instantiation_of_v<
                    std::decay_t<T>, std::map, std::unordered_map, std::multimap, std::unordered_multimap>>
    _print(const T &m, const Params &params = {}) {
        *(params.out) << '{';
        bool first = true;
        for (const auto &it: m) {
            if (!first) *(params.out) << ',';
            first = false;
            _print(it.first,params);
            *(params.out) << ':';
            _print(it.second,params);
        }
        *(params.out) << '}';
    }


    template<class T>
    void _print(const std::stack<T> &x, const Params &params = {}) {
        std::stack<T> s1(x), s2;
        while (!s1.empty()) {
            s2.push(s1.top());
            s1.pop();
        }
        *(params.out) << '[';
        bool first = true;
        while (!s2.empty()) {
            if (!first) *(params.out) << ',';
            first = false;
            _print(s2.top(),params);
            s2.pop();
        }
        *(params.out) << ']';
    }

    template<class T>
    void _print(const std::queue<T> &x, const Params &params = {}) {
        auto q2 = x;
        *(params.out) << '[';
        bool first = true;
        while (!q2.empty()) {
            if (!first) *(params.out) << ',';
            first = false;
            _print(q2.front(),params);
            q2.pop();
        }
        *(params.out) << ']';
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
            _print(std::get<I>(tuple),params);
            if constexpr (I != sizeof...(Ts) - 1) *(params.out) << ',';
            _print<I + 1>(tuple,params);
        }
    }

    template<typename Ta, typename Tb>
    void _print(const std::pair<Ta, Tb> &x, const Params &params = {}) {
        _print(x.first,params);
        _print(',',params);
        _print(x.second,params);
    }
}

void sep(const Params &p) {
    *(p.out) << p.sep;
}

template<typename T>
typename std::enable_if<std::is_same_v<T, Params>>::type
print(const T &p) {
    *(p.out) << p.end;
}

template<typename... Ts>
typename std::enable_if<sizeof...(Ts) == 0>::type
print() {
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
        print(args...);// no need to print sep if it is the last object to print
    } else {// this branch will be called only when params is not settle
        pstyle::_print(arg);
        print();
    }
}


