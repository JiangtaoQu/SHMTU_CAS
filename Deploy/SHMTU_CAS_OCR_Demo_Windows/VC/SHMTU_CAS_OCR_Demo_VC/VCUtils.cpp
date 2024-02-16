// ReSharper disable CppClangTidyClangDiagnosticInvalidUtf8
#include "VCUtils.h"

std::string ConvertLPCWSTRToStdString(LPCWSTR wideString)
{
	// 获取宽字符字符串的长度
	const int wideStringLength =
		static_cast<int>(wcslen(wideString));

	// 计算需要的缓冲区大小
	const int bufferSize =
		WideCharToMultiByte(
			CP_ACP, 0,
			wideString, wideStringLength,
			nullptr, 0,
			nullptr, nullptr
		);

	// 分配缓冲区
	std::string result(bufferSize, '\0');

	// 将宽字符字符串转换为窄字符字符串
	WideCharToMultiByte(
		CP_ACP, 0,
		wideString, wideStringLength,
		result.data(), bufferSize,
		nullptr, nullptr
	);

	return result;
}

LPCWSTR StringToLPCWSTR(const std::string& str)
{
	static std::wstring wideStr;
	wideStr.resize(
		MultiByteToWideChar(
			CP_UTF8, 0,
			str.c_str(), -1,
			nullptr, 0
		)
	);
	MultiByteToWideChar(
		CP_UTF8, 0,
		str.c_str(), -1,
		wideStr.data(), static_cast<int>(wideStr.size())
	);
	return wideStr.c_str();
}
