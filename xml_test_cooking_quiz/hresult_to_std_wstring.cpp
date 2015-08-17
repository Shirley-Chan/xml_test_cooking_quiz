#include "hresult_to_string.h"
#ifndef _UNICODE
#define _UNICODE
#endif
#ifndef UNICODE
#define UNICODE
#endif
#include <comdef.h>
std::wstring hresult_to_wstring(HRESULT err) {
	_com_error error(err);
	return std::wstring(error.ErrorMessage());
}
