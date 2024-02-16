// ReSharper disable CppClangTidyClangDiagnosticInvalidUtf8

#include "SelectUtils.h"
#include "VCUtils.h"

#include <string>

#include <Objbase.h>
// 包含 Common Dialog Box API 相关头文件
#include <Commdlg.h>
// 链接到 Ole32.lib 库
#pragma comment(lib, "Ole32.lib")

LPCWSTR select_file(const LPCWSTR filter)
{
	// 初始化 COM
	CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

	// 打开文件对话框
	OPENFILENAME ofn;
	TCHAR szFile[MAX_PATH] = {0};
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_EXPLORER;

	LPCWSTR result = nullptr;

	if (GetOpenFileName(&ofn) == TRUE)
	{
		// 获取选择的文件路径长度
		int len = lstrlen(ofn.lpstrFile);

		// 动态分配内存来存储选择的文件路径
		result = new WCHAR[len + 1];
		wcscpy_s(const_cast<LPWSTR>(result), len + 1, ofn.lpstrFile);
	}

	// 释放 COM
	CoUninitialize();

	return result;
}

std::string select_file_path_str(const LPCWSTR filter)
{
	const auto file_path = select_file(filter);
	if (file_path == nullptr)
	{
		return "";
	}
	// OutputDebugString(file_path);
	return ConvertLPCWSTRToStdString(file_path);
}

LPCWSTR select_pic_str()
{
	return select_file(L"Image Files(*.jpg; *.png)\0*.jpg; *.png\0");
}
