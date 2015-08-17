#pragma once
#include <array>
#include <type_traits>

template<class T> constexpr std::array<T, 0> make_array() {
	return std::array<T, 0>{ {}};
}
template<class... T> constexpr std::array<typename std::common_type<T...>::type, sizeof...(T)> make_array(T... args) {
	return std::array<typename std::common_type<T...>::type, sizeof...(T)>{ {args...}};
}
