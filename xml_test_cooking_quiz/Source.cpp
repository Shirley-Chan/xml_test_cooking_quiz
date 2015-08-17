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
			L"���̓~�X���V�т��ƐM���Ă��܂��B",
			L"���̓~�X���V�т��ƐM���Ă��܂��B",
			L"���p�e�B�ōĊm�F���Ă�����x���킵�Ă��������ˁB",
			L"���͊撣���Ă��������ˁB"
		),
		make_array(
			L"�ȒP�ł�����ˁH�ȒP�����Ă����܂���B",
			L"�ȒP�ł�����ˁH�ȒP�����Ă����܂���B",
			L"���Ȃ��͖{���ɖ��p�e�B�����Ă܂��ˁB",
			L"�N���A���߂łƂ��������܂��B"
		)
	);
	std::wcout << message[isAllcrear][judge_user_level] << std::endl;
	if (judge_correct && 4 == judge_user_level && !isAllcrear) std::wcout << L"���͑S�␳����ڎw���܂��傤�B" << std::endl;
	pause_console();
	clear_console();
	std::wcout
		<< L"���̃Q�[���̖��͈ȏ�ɂȂ�܂��B��������ȃQ�[������Ăق����A���𑝂₵�Ăق���������܂�����Areadme�ɏ�����܂������[���A�h���X�ցB"
		<< std::endl
		<< L"�V��Q�[����񓙂́Areadme�̃����N����A�N�Z�X�ł��܂��u���O�ɏ����L�ڂ��Ă����܂��B"
		<< std::endl;
}
bool ask(ans_v ans, const size_t q_id, const uint8_t user_level, const int correct_answer, const nodelist_map& nodelist) {
	answer re;
	//��蕶�\��, �񓚓���
	re.user_answer = input_num(rep_LF(nodelist.question[q_id].get_text()), std::stoi(nodelist.choices_num[q_id].get_text()), 1);
	re.jude_correct = (correct_answer == re.user_answer);
	bool judge_continue = re.jude_correct;
	std::wcout << ((re.jude_correct) ? L"����" : L"�s����") << L"�ł��B" << std::endl;
	if (!re.jude_correct && 4 == user_level) {
		std::wcout << "������" << correct_answer << "�ł��B" << std::endl
			<< nodelist.explanation[q_id].get_text() << std::endl;////����\��
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
		std::wcout << L"�N�C�Y�@�`���Ȃ��̓p�e�B�V�G�`" << std::endl << std::endl;
		if (2 == choose_one_from_two(L"�J�n�c�P�@�I���c�Q")) return 0;
		const auto correct_answer = make_array(3, 2, 2, 1, 3, 3, 3, 3, 1, 1, 4, 2, 1, 6, 4, 2, 2, 3, 1);
		//xml read
		auto xml = xmldoc_c(L"question.xml");
		const auto path_base = std::wstring(L"questionbook/questiondata/");
		auto nodelist = nodelist_map(xml[path_base + L"question"], xml[path_base + L"explanation"], xml[path_base + L"IsNotForBiginner"], xml[path_base + L"choices_num"]);
		if (!xml_checker(correct_answer.size(), nodelist)) return -1;//xml�̗v�f���Ɛ����̐����ׂĂ���v���Ȃ��ꍇ�v���O�����I��
		const auto user_level = prelude_question();//���O����
		const auto sequence_of_questions_number = decide_sequence_of_questions_number(user_level);//��菇���̌���
		std::wcout << L"�񓚂��肪�Ƃ��������܂��B����ł͎n�߂܂��B" << std::endl;
		pause_console();
		clear_console();

		//����
		bool judge_correct = true;
		ans_v answer;
		for (size_t i = 0; judge_correct == true && i < sequence_of_questions_number.size(); ++i) {
			const auto question_id = sequence_of_questions_number[i];
			judge_correct = ask(answer, question_id, user_level, correct_answer[question_id], nodelist);
		}
		bool isAllcrear = true;
		for (auto& i : answer) isAllcrear &= i.jude_correct;//�S�␳�������ׂ�
		show_last_message(isAllcrear, judge_correct, user_level);
	}
	catch (std::exception er) {
		std::cerr << er.what() << std::endl;
	}
	return 0;
}