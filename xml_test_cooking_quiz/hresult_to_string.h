#pragma once
#include <string>
#include <winerror.h>
#include <comdef.h>
std::wstring hresult_to_wstring(HRESULT err);
std::string hresult_to_string(HRESULT err);
