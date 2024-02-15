#pragma once

#include <Windows.h>
#include <string>

std::string LPCWSTRToString(LPCWSTR str);

LPCWSTR StringToLPCWSTR(const std::string& str);
