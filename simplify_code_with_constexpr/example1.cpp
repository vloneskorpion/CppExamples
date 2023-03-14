// https://www.cppstories.com/2018/03/ifconstexpr/

#include <type_traits>
#include <concepts>

// C++11/14 version:
/*
template <class T>
constexpr T absolute(T arg) {
   return arg < 0 ? -arg : arg;
}

template <class T>
constexpr std::enable_if_t<std::is_floating_point<T>::value, bool> close_enough(T a, T b) {
   return absolute(a - b) < static_cast<T>(0.000001);
}
template <class T>
constexpr std::enable_if_t<!std::is_floating_point<T>::value, bool> close_enough(T a, T b) {
   return a == b;
}*/

// C++17:

template <class T>
constexpr T absolute(T arg) {
   return arg < 0 ? -arg : arg;
}

template <class T>
constexpr auto precision_threshold = T(0.000001);

template <class T>
constexpr bool close_enough(T a, T b) {
   if constexpr (std::is_floating_point_v<T>)
      return absolute(a - b) < precision_threshold<T>;
   else
      return a == b;
}

// C++20
constexpr bool close_enough20(std::floating_point auto a, std::floating_point auto b) {
   return absolute(a - b) < precision_threshold<std::common_type_t<decltype(a), decltype(b)>>;
}
constexpr bool close_enough20(std::integral auto a, std::integral auto b) {
   return a == b;
}

// alternate version:
template <std::floating_point T>
constexpr bool close_enough201(T a, T b) {
   return absolute(a - b) < precision_threshold<T>;
}

constexpr bool close_enough201(std::integral auto a, std::integral auto b) {
   return a == b;
}

// alternate version 2:
template <typename T>
requires std::is_floating_point_v<T> // requires clause
constexpr bool close_enough202(T a, T b) {
   return absolute(a - b) < precision_threshold<T>;
}

constexpr bool close_enough202(std::integral auto a, std::integral auto b) {
   return a == b;
}

int main()
{
    static_assert(close_enough20(10.0f, 10.0f));
    static_assert(close_enough20(10, 10));

    static_assert(close_enough201(10.0f, 10.0f));
    static_assert(close_enough201(10, 10));

    static_assert(close_enough202(10.0f, 10.0f));
    static_assert(close_enough202(10, 10));
}