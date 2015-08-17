#pragma once
#include <cstddef>
#include <limits>
#include <cstdint>
#include <string>
#ifdef max
#undef max
#endif

template<typename T_> using limit = std::numeric_limits<T_>;//create new type. C++11:alias declaration
size_t choose_one_from_two(const wchar_t* echo_str) noexcept;
int input_num(std::wstring&& echo_str, const int max, const int min) noexcept;
uint8_t prelude_question();
