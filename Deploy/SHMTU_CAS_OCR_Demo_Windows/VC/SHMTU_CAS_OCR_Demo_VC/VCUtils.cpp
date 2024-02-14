#include "VCUtils.h"

std::string LPCWSTRToString(LPCWSTR str)
{
	if (str == nullptr)
		return "";

	int len = WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL);
	if (len == 0)
		return "";

	char* buffer = new char[len];
	WideCharToMultiByte(CP_UTF8, 0, str, -1, buffer, len, NULL, NULL);

	std::string result(buffer);

	delete[] buffer;

	return result;
}

// LPCWSTR StringToLPCWSTR(const std::string str)
// {
// 	int len = MultiByteToWideChar(
// 		CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
// 	if (len == 0)
// 		return nullptr;
//
// 	const auto buffer = new wchar_t[len];
// 	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, buffer, len);
//
// 	const LPCWSTR result = buffer;
//
// 	delete[] buffer;
//
// 	return result;
// }

LPCWSTR StringToLPCWSTR(const std::string& str) {
	static std::wstring wideStr;
	wideStr.resize(MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0));
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wideStr[0], wideStr.size());
	return wideStr.c_str();
}