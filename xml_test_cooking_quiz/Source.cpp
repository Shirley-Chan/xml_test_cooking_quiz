#include "xmlreader_simple.h"
#include "nodelist_map.h"
#include "prelude.h"
#include "xml_checker.h"
#include "make_array.h"
#include "Environment-dependent.h"
#include <iostream>
#include <vector>
#include <exception>
struct answer{
	int user_answer;
	bool jude_correct;
};
using ans_v = std::vector<answer>;
auto decide_sequence_of_questions_number(uint8_t user_level) {
	static const auto sequence_of_questions_number_table = make_array(
		make_array<size_t>(0U, 1U, 2U, 3U, 4U, 8U, 12U, 16U, 17U, 18U),
		make_array<size_t>(0U, 1U, 2U, 3U, 5U, 9U, 13U, 16U, 17U, 18U),
		make_array<size_t>(0U, 1U, 2U, 3U, 6U, 10U, 14U, 16U, 17U, 18U),
		make_array<size_t>(0U, 1U, 2U, 3U, 7U, 11U, 15U, 16U, 17U, 18U)
	);
	return sequence_of_questions_number_table[user_level - 1];
}
std::wstring rep_LF(std::wstring&& str) {
	if (L"empty" == str) return L"";
	const std::wstring rep_target = L"\\n";
	const std::wstring rep_after = L"\n";
	for (size_t pos = str.find(rep_target); std::string::npos != pos; pos = str.find(rep_target, pos + rep_after.length())) {
		str.replace(pos, rep_target.length(), rep_after);
	}
	std::wcout << str << std::endl;
	return str;
}
void show_last_message(const bool isAllcrear, const bool judge_correct, const uint8_t judge_user_level) {
	static const auto message = make_array(
		make_array(
			L"入力ミスか遊びだと信じています。",
			L"入力ミスか遊びだと信じています。",
			L"夢パティで再確認してもう一度挑戦してくださいね。",
			L"次は頑張ってくださいね。"
		),
		make_array(
			L"簡単でしたよね？簡単すぎてすいません。",
			L"簡単でしたよね？簡単すぎてすいません。",
			L"あなたは本当に夢パティ愛してますね。",
			L"クリアおめでとうございます。"
		)
	);
	std::wcout << message[isAllcrear][judge_user_level] << std::endl;
	if (judge_correct && 4 == judge_user_level && !isAllcrear) std::wcout << L"次は全問正解を目指しましょう。" << std::endl;
	pause_console();
	clear_console();
	std::wcout
		<< L"このゲームの問題は以上になります。もしこんなゲーム作ってほしい、問題を増やしてほしい等ありましたら、readmeに書かれましたメールアドレスへ。"
		<< std::endl
		<< L"新作ゲーム情報等は、readmeのリンクからアクセスできますブログに順次記載していきます。"
		<< std::endl;
}
bool ask(ans_v ans, const size_t q_id, const uint8_t user_level, const int correct_answer, const nodelist_map& nodelist) {
	answer re;
	//問題文表示, 回答入力
	re.user_answer = input_num(rep_LF(nodelist.question[q_id].get_text()), std::stoi(nodelist.choices_num[q_id].get_text()), 1);
	re.jude_correct = (correct_answer == re.user_answer);
	bool judge_continue = re.jude_correct;
	std::wcout << ((re.jude_correct) ? L"正解" : L"不正解") << L"です。" << std::endl;
	if (!re.jude_correct && 4 == user_level) {
		std::wcout << "正解は" << correct_answer << "です。" << std::endl
			<< nodelist.explanation[q_id].get_text() << std::endl;////解説表示
		if (std::stoi(nodelist.IsNotForBiginner[q_id].get_text())) judge_continue = true;
	}
	pause_console();
	clear_console();
	ans.push_back(re);
	return judge_continue;
}
int main() {
	try {
		std::wcout.imbue(std::locale("japanese"));
		std::wcout << L"クイズ　～あなたはパティシエ～" << std::endl << std::endl;
		if (2 == choose_one_from_two(L"開始…１　終了…２")) return 0;
		const auto correct_answer = make_array(3, 2, 2, 1, 3, 3, 3, 3, 1, 1, 4, 2, 1, 6, 4, 2, 2, 3, 1);
		//xml read
		auto xml = xmldoc_c(L"question.xml");
		const auto path_base = std::wstring(L"questionbook/questiondata/");
		auto nodelist = nodelist_map(xml[path_base + L"question"], xml[path_base + L"explanation"], xml[path_base + L"IsNotForBiginner"], xml[path_base + L"choices_num"]);
		if (!xml_checker(correct_answer.size(), nodelist)) return -1;//xmlの要素数と正解の数すべてが一致しない場合プログラム終了
		const auto user_level = prelude_question();//事前質問
		const auto sequence_of_questions_number = decide_sequence_of_questions_number(user_level);//問題順序の決定
		std::wcout << L"回答ありがとうございます。それでは始めます。" << std::endl;
		pause_console();
		clear_console();

		//質問
		bool judge_correct = true;
		ans_v answer;
		for (size_t i = 0; judge_correct == true && i < sequence_of_questions_number.size(); ++i) {
			const auto question_id = sequence_of_questions_number[i];
			judge_correct = ask(answer, question_id, user_level, correct_answer[question_id], nodelist);
		}
		bool isAllcrear = true;
		for (auto& i : answer) isAllcrear &= i.jude_correct;//全問正解か調べる
		show_last_message(isAllcrear, judge_correct, user_level);
	}
	catch (const std::exception& er) {
		std::cerr << er.what() << std::endl;
	}
	return 0;
}