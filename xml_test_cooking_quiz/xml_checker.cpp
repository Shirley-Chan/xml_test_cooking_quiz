#include "xml_checker.h"
#include <algorithm>
#include <cstddef>
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
bool xml_checker(const size_t total_question_id_num, const nodelist_map& nodelist) {
	auto max = std::max({ nodelist.choices_num.size(), nodelist.explanation.size(), nodelist.IsNotForBiginner.size(), nodelist.question.size(), total_question_id_num });
	auto min = std::min({ nodelist.choices_num.size(), nodelist.explanation.size(), nodelist.IsNotForBiginner.size(), nodelist.question.size(), total_question_id_num });
	return max == min;
}
