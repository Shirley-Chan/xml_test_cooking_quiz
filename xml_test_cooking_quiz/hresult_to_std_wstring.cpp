#ifndef _UNICODE
#define _UNICODE
#endif
#ifndef UNICODE
#define UNICODE
#endif
#include "hresult_to_string.h"
std::wstring hresult_to_wstring(HRESULT err) {
	_com_error error(err);
	auto re = error.ErrorMessage();
	static_assert(sizeof(*re) == sizeof(wchar_t), "type of 'TCHAR' is not defined as 'wchar_t'");
	return std::wstring(error.ErrorMessage());
}
