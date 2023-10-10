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
#include <sstream>

struct Params {
    std::string sep = " ";
    std::string end = "\n";
    std::ostream *out = &std::cout;
};
namespace cpp_print{
// Checks magic_enum compiler compatibility.
#if defined(__clang__) && __clang_major__ >= 5 || defined(__GNUC__) && __GNUC__ >= 9 || defined(_MSC_VER) && _MSC_VER >= 1910 || defined(__RESHARPER__)
#  undef  MAGIC_ENUM_SUPPORTED
#  define MAGIC_ENUM_SUPPORTED 1
#endif

    // Enum value must be greater or equals than MAGIC_ENUM_RANGE_MIN. By default MAGIC_ENUM_RANGE_MIN = -128.
// If need another min range for all enum types by default, redefine the macro MAGIC_ENUM_RANGE_MIN.
#if !defined(MAGIC_ENUM_RANGE_MIN)
#  define MAGIC_ENUM_RANGE_MIN -128
#endif

// Enum value must be less or equals than MAGIC_ENUM_RANGE_MAX. By default MAGIC_ENUM_RANGE_MAX = 128.
// If need another max range for all enum types by default, redefine the macro MAGIC_ENUM_RANGE_MAX.
#if !defined(MAGIC_ENUM_RANGE_MAX)
#  define MAGIC_ENUM_RANGE_MAX 127
#endif

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
    constexpr bool is_special_unit = std::is_same_v<std::decay_t<T>, bool> || std::is_enum_v<std::decay_t<T>>;

    template<typename T>
    constexpr bool is_simple_unit = (std::is_integral_v<std::decay_t<T>> || std::is_floating_point_v<std::decay_t<T>> ||
                                     is_one_of<std::decay_t<T>, std::string, char *, std::string_view>::value) &&
                                    (!is_special_unit<T>);

    template<typename T>
    constexpr bool is_simple_container = is_one_instantiation_of_v<std::decay_t<T>,
            std::vector, std::deque, std::list, std::forward_list,
            std::set, std::multiset, std::unordered_set, std::unordered_multiset>;

    template<typename T>
    constexpr bool is_kv_container = is_one_instantiation_of_v<std::decay_t<T>,
            std::map, std::unordered_map, std::multimap, std::unordered_multimap>;

    template<typename T>
    const bool is_ready_type = is_simple_unit<std::decay_t<T>> || is_simple_container<std::decay_t<T>>
                               || is_kv_container<std::decay_t<T>> || is_special_unit<T>;

    constexpr std::string_view pretty_name(std::string_view name) noexcept {
        // guide from https://zhuanlan.zhihu.com/p/464758750
        for (std::size_t i = name.size(); i > 0; --i) {
            if (!((name[i - 1] >= '0' && name[i - 1] <= '9') ||
                  (name[i - 1] >= 'a' && name[i - 1] <= 'z') ||
                  (name[i - 1] >= 'A' && name[i - 1] <= 'Z') ||
                  #if defined(MAGIC_ENUM_ENABLE_NONASCII)
                  (name[i - 1] & 0x80) ||
                  #endif
                  (name[i - 1] == '_'))) {
                name.remove_prefix(i);
                break;
            }
        }

        // 这里检查名称是否有效，注意到这里和前面稍有不同，这里不允许出现数字
        if (name.size() > 0 && ((name.front() >= 'a' && name.front() <= 'z') ||
                                (name.front() >= 'A' && name.front() <= 'Z') ||
                                #if defined(MAGIC_ENUM_ENABLE_NONASCII)
                                (name.front() & 0x80) ||
                                #endif
                                (name.front() == '_'))) {
            return name;
        }

        return {}; // Invalid name.
    }

    template<typename E, E V>
    constexpr auto name() noexcept {
        static_assert(std::is_enum_v<E>, "magic_enum::detail::n requires enum type.");

#if defined(MAGIC_ENUM_SUPPORTED) && MAGIC_ENUM_SUPPORTED
#  if defined(__clang__) || defined(__GNUC__)
        constexpr auto name = pretty_name({__PRETTY_FUNCTION__, sizeof(__PRETTY_FUNCTION__) - 2});
#  elif defined(_MSC_VER)
        constexpr auto name = pretty_name({__FUNCSIG__, sizeof(__FUNCSIG__) - 17});
#  endif
        return name;
#else
        return std::string_view{}; // Unsupported compiler.
#endif
    }

    template<typename E, typename U, U x, typename=void>
    struct could_v_static_cast : std::false_type {
    };
    template<typename E, typename U, U x>
    struct could_v_static_cast<E, U, x, std::void_t<
            decltype(static_cast<E>(x))
    >> : std::true_type {
    };

    template<typename E, typename U, typename=void>
    struct could_static_cast : std::false_type {
    };
    template<typename E, typename U>
    struct could_static_cast<E, U, std::void_t<
            decltype(static_cast<E>(std::declval<U>()))
    >> : std::true_type {
    };

    using enum_map=std::unordered_map<int, std::string_view>;

    template<typename T, int i>
    auto check_enum(enum_map & m)
    -> std::enable_if_t<could_v_static_cast<T, int, i>::value, std::string_view> {
        std::string_view temp=name<T, static_cast<T>(i)>();
        if(!temp.empty()) m[i]=temp;
        return temp;
    }

    template<typename T, int i>
    auto check_enum(...) -> std::enable_if_t<!could_v_static_cast<T, int, i>::value, std::string_view> {
        return {};
    }

    template<typename T,int MIN, int... ids>
    [[nodiscard]] auto names_impl(std::integer_sequence<int,ids...>) {
        enum_map m{};
        ((check_enum<T,MIN+ids>(m)),...);
        return m;
    }


    template<typename T>
    [[nodiscard]]  auto names() {
        static const auto result = names_impl<T,MAGIC_ENUM_RANGE_MIN>
                (std::make_integer_sequence<int,MAGIC_ENUM_RANGE_MAX-MAGIC_ENUM_RANGE_MIN>{});
        return result;
    }


    template<typename T>
    void _print(T *const &x, const Params &params = {});

    template<typename T>
    std::enable_if_t<is_simple_unit<T>>
    _print(const T &x, const Params &params = {}) {
        *(params.out) << x;
    }

    template<typename T>
    std::enable_if_t<std::is_enum_v<std::decay_t<T>>>
    _print(const T &t, const Params &params = {}) {
        std::stringstream ss;
        if constexpr (could_static_cast<int,T>::value){
            auto x=static_cast<int>(t);
            if(names<T>().count(x)){
                ss<<names<T>()[x];
            }else{
                ss<<"[warning: this enum "<<x<<" is not recognized]";
            }
        }else{
            ss<<"[warning: this enum can not cast to size_t]";
        }

        _print(ss.str());
    }

    template<typename T>
    std::enable_if_t<std::is_same_v<std::decay_t<T>, bool>>
    _print(const T &v, const Params &params = {}) {
        *(params.out) << (v ? "true" : "false");
    }

    template<typename Ts>
    std::enable_if_t<is_simple_container<Ts>>
    _print(const Ts &args, const Params &params = {}) {
        bool first = true;
        *(params.out) << '[';
        for (const auto &i: args) {
            if (!first) *(params.out) << ',';
            first = false;
            _print(i, params);
        }
        *(params.out) << ']';
    }

    template<typename T>
    std::enable_if_t<is_kv_container<T>>
    _print(const T &m, const Params &params = {}) {
        *(params.out) << '{';
        bool first = true;
        for (const auto &it: m) {
            if (!first) *(params.out) << ',';
            first = false;
            _print(it.first, params);
            *(params.out) << ':';
            _print(it.second, params);
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
            _print(s2.top(), params);
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
            _print(q2.front(), params);
            q2.pop();
        }
        *(params.out) << ']';
    }

    template<typename T, size_t I>
    void _print(const std::array<T, I> &a, const Params &params = {}) {
        *(params.out) << '[';
        bool first = true;
        for (int i = 0; i < a.size(); i++) {
            if (!first) *(params.out) << ',';
            first = false;
            _print(a[i], params);
        }
        *(params.out) << ']';
    }


    template<typename T>
    std::enable_if_t<!is_ready_type<T>>
    _print(const T &x, const Params &params = {}) {
        *(params.out) << x;
    }

    template<typename T>
    void
    _print(T *const &x, const Params &params) {
        _print(*x, params);
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
            _print(std::get<I>(tuple), params);
            if constexpr (I != sizeof...(Ts) - 1) *(params.out) << ',';
            _print<I + 1>(tuple, params);
        }
    }

    template<typename Ta, typename Tb>
    void _print(const std::pair<Ta, Tb> &x, const Params &params = {}) {
        _print(x.first, params);
        _print(',', params);
        _print(x.second, params);
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
            cpp_print::_print(arg, last);
            if constexpr (sizeof...(args) > 1) {
                sep(last);
            }
        } else {
            cpp_print::_print(arg);
            sep({});
        }
        print(args...);// no need to print sep if it is the last object to print
    } else {// this branch will be called only when params is not settle
        cpp_print::_print(arg);
        print();
    }
}


