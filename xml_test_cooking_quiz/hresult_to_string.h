#pragma once
#include <cstddef>
#include <string>
#include <winerror.h>
std::wstring hresult_to_wstring(HRESULT err);
std::string hresult_to_string(HRESULT err);
