#pragma once

#include <Windows.h>
#include <string>

std::string ConvertLPCWSTRToStdString(LPCWSTR wideString);

LPCWSTR StringToLPCWSTR(const std::string& str);
