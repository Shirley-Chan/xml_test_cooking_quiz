#include "prelude.h"
#include "make_array.h"
#include "Environment-dependent.h"
#include <iostream>
#include <exception>
#include <stdexcept>
#include <type_traits>
#include <utility>

template<typename T_> T_ input(const wchar_t* echo_str, const T_ max = limit<T_>::max(), const T_ min = limit<T_>::lowest()) noexcept {
	static_assert(std::is_arithmetic<T_>::value, "unexpected type T_");//T_が整数か浮動小数点型でないならばコンパイルエラーを出す
	T_ buf;
	try {
		if (nullptr == echo_str) throw std::invalid_argument("echo_str is unexpected input");//エラー対策
		if (L'\0' != echo_str[0]) std::wcout << echo_str << std::endl;//文字列が空じゃなければ出力
		std::cin >> buf;//入力を受ける
		if (max < buf || buf < min) throw std::out_of_range("input is iligal");//範囲チェック
	}
	catch (const std::exception& er) {
		std::cerr << er.what() << std::endl;//エラーメッセージ表示
		return input(L"再入力してください。", max, min);//エラー時は再帰する方向で
	}
	return buf;
}
size_t choose_one_from_two(const wchar_t* echo_str) noexcept {
	return input<size_t>(echo_str, 2, 1);
}
int input_num(std::wstring&& echo_str, const int max, const int min) noexcept {
	return input(echo_str.c_str(), max, min);
}
static inline void prelude_question_1() {
	std::wcout << L"事前確認１\nあなたはパティシエまたはパティシエールですか？" << std::endl;
}
static inline void prelude_question_2() {
	std::wcout << L"事前確認２\nあなたは夢色パティシエールを視聴してその内容やそこで出てきた知識を全て覚えていますか？" << std::endl;
}
uint8_t calc_game_level(std::pair<size_t, size_t> answer) noexcept {
	/*user_level
	はい、はい		：	1
	はい、いいえ	：	2
	いいえ、はい	：	3
	いいえ、いいえ	：	4
	*/
	if (1 == answer.first) --answer.first;
	return static_cast<uint8_t>(answer.first + answer.second);
}
uint8_t prelude_question() {
	static const auto ask_yes_or_no = L"はい…１　いいえ…２";
	static const auto yes_or_no = make_array(L"はい", L"いいえ");
	std::pair<size_t, size_t> answer;
	for (uint8_t roop = 1; roop;
		roop = static_cast<uint8_t>(choose_one_from_two(ask_yes_or_no) - 1)
	) {
		clear_console();
		//質問部
		prelude_question_1();
		answer.first = choose_one_from_two(ask_yes_or_no);
		clear_console();;
		prelude_question_2();
		answer.second = choose_one_from_two(ask_yes_or_no);
		clear_console();
		//確認画面
		printf("これでよろしいですか？\n");
		prelude_question_1();
		std::wcout << yes_or_no[answer.first - 1] << std::endl << std::endl;
		prelude_question_2();
		std::wcout << yes_or_no[answer.second - 1] << std::endl << std::endl;
	}
	clear_console();
	return calc_game_level(answer);
}
