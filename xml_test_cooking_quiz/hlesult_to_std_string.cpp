#ifdef _UNICODE
#undef _UNICODE
#endif
#ifdef UNICODE
#undef UNICODE
#endif
#include "hresult_to_string.h"
std::string hresult_to_string(HRESULT err) {
	_com_error error(err);
	auto re = error.ErrorMessage();
	static_assert(sizeof(*re) == sizeof(char), "type of 'TCHAR' is not defined as 'wchar_t'");
	return std::string(re);
}
