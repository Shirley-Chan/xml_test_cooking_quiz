#pragma once
#include "xmlreader_simple.h"
struct nodelist_map {
	xmllist_c question, explanation, IsNotForBiginner, choices_num;
	nodelist_map(xmllist_c&& question, xmllist_c&& explanation, xmllist_c&& IsNotForBiginner, xmllist_c&& choices_num)
		: question(question), explanation(explanation), IsNotForBiginner(IsNotForBiginner), choices_num(choices_num) {}
	nodelist_map(const nodelist_map& other) : question(other.question), explanation(other.explanation), IsNotForBiginner(other.IsNotForBiginner), choices_num(other.choices_num) {}
};
