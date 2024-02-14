// ReSharper disable CppClangTidyClangDiagnosticInvalidUtf8

#include "SelectUtils.h"
#include "VCUtils.h"

#include <string>

#include <Objbase.h>
// ���� Common Dialog Box API ���ͷ�ļ�
#include <Commdlg.h>
// ���ӵ� Ole32.lib ��
#pragma comment(lib, "Ole32.lib")

LPCWSTR select_file(const LPCWSTR filter)
{
	// ��ʼ�� COM
	CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

	// ���ļ��Ի���
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
		result = ofn.lpstrFile;
	}

	// �ͷ� COM
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
	return LPCWSTRToString(file_path);
}

LPCWSTR select_pic_str()
{
	return select_file(L"Image Files(*.jpg; *.png)\0*.jpg; *.png\0");
}
