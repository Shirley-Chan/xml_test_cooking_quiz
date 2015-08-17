#include "hresult_to_string.h"
#ifdef _UNICODE
#undef _UNICODE
#endif
#ifdef UNICODE
#undef UNICODE
#endif
#include <comdef.h>
std::string hresult_to_string(HRESULT err) {
	_com_error error(err);
	return std::string(error.ErrorMessage());
}
