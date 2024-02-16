// WindowsProject1.cpp : 定义应用程序的入口点。
//

#define ADAPT_HIGH_DPI 1

#include "framework.h"
#include "WinMain1.h"
// #include <ShellScalingApi.h>

#include <opencv2/opencv.hpp>

#include <filesystem>
#include <string>

#include "SelectUtils.h"
#include "SHMTU_CAS_URL.h"
#include "WindowUtils.h"

#include "../NCNN_CLI/CAS_OCR.h"

const auto model_type = "fp16";

#define CREATE_DPI_AWARE_WINDOW( \
	lpClassName, lpWindowName, dwStyle, \
	x, y, nWidth, nHeight, \
	hWndParent, hMenu, hInstance, lpParam, \
	DPI_RATIO \
) { \
    CreateWindow( \
		lpClassName, lpWindowName, dwStyle, \
		static_cast<int>((x) * (DPI_RATIO)), \
		static_cast<int>((y) * (DPI_RATIO)), \
		static_cast<int>((nWidth) * (DPI_RATIO)), \
		static_cast<int>((nHeight) * (DPI_RATIO)), \
		hWndParent, hMenu, hInstance, lpParam \
	) \
}

constexpr auto window_main_size_width = 550;
constexpr auto window_main_size_height = 300;
constexpr auto size_font_base = 16;
const auto FONT_NAME = TEXT("Arial");

// 句柄
HWND hMainWindow,
     hButton1, hButton2, hButton3, hButton4,
     hImageControl,
     hLabel,
     hCheckbox;

float dpi_ratio = 1.0f;

// 按钮标识
#define ID_BUTTON_1 1001
#define ID_BUTTON_2 1002
#define ID_BUTTON_3 1003
#define ID_BUTTON_4 1004
// 复选框控件的标识符
#define ID_CHECKBOX_USE_GPU 1101

// ReSharper disable once IdentifierTypo
#define MAX_LOADSTRING 100  // NOLINT(modernize-macro-to-enum)

// 全局变量:
HINSTANCE hInst; // 当前实例
WCHAR szTitle[MAX_LOADSTRING] = L""; // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING] = L""; // 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 在此处放置代码。

#if ADAPT_HIGH_DPI
	// 告知系统应用程序是 DPI 感知的
	SetProcessDPIAware();
#endif

	// 初始化全局字符串
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_OCR_DEMO, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	const HACCEL h_accel_table =
		LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));

	MSG msg;

	// 主消息循环:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, h_accel_table, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return static_cast<int>(msg.wParam);
}

//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_SHMTU));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = reinterpret_cast<HBRUSH>((COLOR_WINDOW + 1));
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON_SHMTU));

	return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 将实例句柄存储在全局变量中

	// 设置 DPI 感知，但是操作系统至少Windows 8.1(NTDDI_VERSION >= NTDDI_WINBLUE)
	// SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);

	// 获取设备的 DPI
	const HDC screen = GetDC(nullptr);
	const int dpiX =
		GetDeviceCaps(screen, LOGPIXELSX);
	ReleaseDC(nullptr, screen);
#if ADAPT_HIGH_DPI
	dpi_ratio = static_cast<float>(dpiX) / static_cast<float>(USER_DEFAULT_SCREEN_DPI);
#endif

	const int target_window_width =
		static_cast<int>(window_main_size_width * dpi_ratio);
	const int target_window_height =
		static_cast<int>(window_main_size_height * dpi_ratio);

	const HWND hWnd = CreateWindowW(
		szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		target_window_width, target_window_height,
		nullptr, nullptr, hInstance, nullptr
	);

	hMainWindow = hWnd;

	const std::wstring dpi_str =
		L"DPI Scale Ratio:" + std::to_wstring(dpi_ratio);
	OutputDebugString(dpi_str.c_str());

	// 设置不可调整大小
	SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_THICKFRAME);

	// 移除最大化按钮和
	SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_MAXIMIZEBOX);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

// 设置控件字体大小的函数实现
BOOL SetControlFont(HWND hWndControl, int nHeight, LPCTSTR lpszFace)
{
	HFONT hFont = CreateFont(nHeight, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
	                         CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, lpszFace);
	if (hFont == nullptr)
		return FALSE;

	SendMessage(hWndControl, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));

	return TRUE;
}

BOOL SetWidgetFont(HWND hWndControl)
{
	return SetControlFont(
		hWndControl,
		static_cast<int>(size_font_base * dpi_ratio),
		FONT_NAME
	);
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	void on_click_btn1(HWND hWnd_Button);
	void on_click_btn2(HWND hWnd_Button);
	void on_click_btn3(HWND hWnd_Button);

	switch (message)
	{
	case WM_DPICHANGED:
		{
			// 获取新的 DPI 信息
			const UINT dpi = HIWORD(wParam);

			// 获取新的窗口矩形信息
			const RECT* prcNewWindow =
				reinterpret_cast<RECT*>(lParam);

			// 计算缩放比例
			const float scale_x =
				static_cast<float>(dpi) / static_cast<float>(USER_DEFAULT_SCREEN_DPI);
			const float scale_y = scale_x;

			// 计算新的宽度和高度
			const int new_width = static_cast<int>(window_main_size_width * scale_x);
			const int new_height = static_cast<int>(window_main_size_height * scale_y);

			// 设置新的窗口大小
			SetWindowPos(
				hWnd, nullptr,
				prcNewWindow->left, prcNewWindow->top,
				new_width, new_height,
				SWP_NOZORDER | SWP_NOACTIVATE
			);

			return 0;
		}
	case WM_CREATE:
		{
			// 创建按钮-从URL加载图片
			hButton1 = CREATE_DPI_AWARE_WINDOW(
				TEXT("BUTTON"), TEXT("Download URL"),
				WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
				410, 20, 120, 30,
				hWnd, reinterpret_cast<HMENU>(ID_BUTTON_1), NULL, NULL,
				dpi_ratio
			);
			SetWidgetFont(hButton1);

			// 创建按钮-OCR
			hButton2 = CREATE_DPI_AWARE_WINDOW(
				TEXT("BUTTON"), TEXT("OCR"),
				WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
				410, 60, 120, 30,
				hWnd, reinterpret_cast<HMENU>(ID_BUTTON_2), NULL, NULL,
				dpi_ratio
			);
			SetWidgetFont(hButton2);

			// 创建按钮-从本地打开
			hButton3 = CREATE_DPI_AWARE_WINDOW(
				TEXT("BUTTON"), TEXT("Local Image"),
				WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
				410, 100, 120, 30,
				hWnd, reinterpret_cast<HMENU>(ID_BUTTON_3), NULL, NULL,
				dpi_ratio
			);
			SetWidgetFont(hButton3);

			// 创建显示图片的控件
			hImageControl = CREATE_DPI_AWARE_WINDOW(
				TEXT("STATIC"), NULL,
				WS_VISIBLE | WS_CHILD | SS_BITMAP,
				0, 0, 400, 140,
				hWnd, NULL, NULL, NULL,
				dpi_ratio
			);
			SetWidgetFont(hImageControl);

			// 创建复选框控件(是否使用GPU)
			hCheckbox = CREATE_DPI_AWARE_WINDOW(
				TEXT("BUTTON"), TEXT("Use GPU"),
				WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
				410, 140, 120, 30,
				hWnd, reinterpret_cast<HMENU>(ID_CHECKBOX_USE_GPU), NULL, NULL,
				dpi_ratio
			);
			SetWidgetFont(hCheckbox);

			// 创建按钮-从释放模型资源
			hButton4 = CREATE_DPI_AWARE_WINDOW(
				TEXT("BUTTON"), TEXT("Release"),
				WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
				410, 180, 120, 30,
				hWnd, reinterpret_cast<HMENU>(ID_BUTTON_4), NULL, NULL,
				dpi_ratio
			);
			SetWidgetFont(hButton4);

			// 显示OCR结果
			hLabel = CREATE_DPI_AWARE_WINDOW(
				TEXT("STATIC"), TEXT("[Wait For OCR]"),
				WS_VISIBLE | WS_CHILD | SS_CENTER,
				10, 150, 380, 50,
				hWnd, NULL, NULL, NULL,
				dpi_ratio
			);
			SetControlFont(
				hLabel,
				static_cast<int>(size_font_base * 2.5 * dpi_ratio),
				FONT_NAME
			);

			// Author Info
			const HWND label_author =
				CREATE_DPI_AWARE_WINDOW(
					TEXT("STATIC"), TEXT("Author:Haomin Kong"),
					WS_VISIBLE | WS_CHILD | SS_CENTER,
					10, 210, 380, 30,
					hWnd, NULL, NULL, NULL,
					dpi_ratio
				);
			SetControlFont(
				label_author,
				static_cast<int>(size_font_base * 1.5 * dpi_ratio),
				FONT_NAME
			);
		}
		break;
	case WM_COMMAND:
		{
			// 检查按钮点击事件
			if (HIWORD(wParam) == BN_CLICKED)
			{
				// 检查是哪个按钮被点击
				if (LOWORD(wParam) == ID_BUTTON_1)
				{
					// 按钮1被点击
					// 从URL加载图片
					on_click_btn1(hWnd);
				}
				else if (LOWORD(wParam) == ID_BUTTON_2)
				{
					// 按钮2被点击
					// OCR识别图片
					on_click_btn2(hWnd);
				}
				else if (LOWORD(wParam) == ID_BUTTON_3)
				{
					// 按钮3被点击
					// 打开本地图片
					on_click_btn3(hWnd);
				}
				else if (LOWORD(wParam) == ID_BUTTON_4)
				{
					// 按钮4被点击
					// 释放模型
					void try_to_release_model();
					try_to_release_model();
				}
				else if (LOWORD(wParam) == ID_CHECKBOX_USE_GPU)
				{
					// 复选框被点击

					// 获取复选框的状态
					const LRESULT is_checked =
						SendMessage(hCheckbox, BM_GETCHECK, 0, 0);
					const bool is_checked_bool = is_checked == BST_CHECKED;

					// 切换复选框的状态
					SendMessage(
						hCheckbox,
						BM_SETCHECK, is_checked_bool ? BST_UNCHECKED : BST_CHECKED,
						0
					);
				}
			}

			//////////////////////////////////////////
			int wmId = LOWORD(wParam);
			// 分析菜单选择:
			switch (wmId)
			{
			case ID_MENU_RELEASE:
				void try_to_release_model();
				try_to_release_model();
				break;
			case IDM_EXIT:
				DestroyWindow(hWnd);
				break;
			case IDM_DOWNLOAD_URL:
				on_click_btn1(hWnd);
				break;
			case IDM_OPEN_LOCAL:
				on_click_btn3(hWnd);
				break;
			case IDM_DO_OCR:
				on_click_btn2(hWnd);
				break;
			case IDM_ABOUT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;
			default:
				break;
			}
		}
		return DefWindowProc(hWnd, message, wParam, lParam);
	case WM_PAINT:
		{
			// 虽然没用，但是必须保留
			// 否则在弹出信息框的时候，可能会有很大的延迟
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			// 在此处添加使用 hdc 的任何绘图代码...
			EndPaint(hWnd, &ps);
		}
		break;
	case WM_DESTROY:

		// 使用CPU推理操作系统会自动GC
		// 如果使用了Vulkan必须释放资源，否则会报错
		CAS_OCR::release_model();

		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	default:
		break;
	}
	return (INT_PTR)FALSE;
}

cv::Mat current_img;

void on_click_btn1(HWND hWnd_Button)
{
	// 按钮1被点击
	if (auto img = get_captcha_by_url(); !img.empty())
	{
		fix_channels(img);
		current_img = img;
		set_widget_image(hImageControl, current_img);
	}
	else
	{
		MessageBox(
			hWnd_Button,
			TEXT("Failed to load image!"),
			TEXT("Error"),
			MB_ICONERROR | MB_OK
		);
	}
}

void on_click_btn2(HWND hWnd_Button)
{
	const auto use_gpu =
		check_box_is_checked(hCheckbox);

#ifdef _DEBUG

	if (use_gpu)
	{
		OutputDebugStringW(L"GPU Mode!");
	}
	else
	{
		OutputDebugStringW(L"CPU Mode!");
	}

#endif

	if (current_img.empty())
	{
		MessageBox(
			hWnd_Button,
			TEXT("No image loaded!"),
			TEXT("Error"), MB_ICONERROR | MB_OK
		);
		return;
	}

	CAS_OCR::set_model_gpu_support(use_gpu);

	const auto checkpoint_dir_path = "../../Checkpoint";

	if (
		std::filesystem::exists(checkpoint_dir_path) &&
		std::filesystem::is_directory(checkpoint_dir_path)
	)
	{
		CAS_OCR::init_model(
			"../../Checkpoint",
			model_type
		);
	}
	else
	{
		CAS_OCR::init_model(
			"./",
			model_type
		);
	}

	const auto result =
		CAS_OCR::predict_validate_code(current_img);

	const auto text_str = std::get<1>(result);
	const auto text_lstr = StringToLPCWSTR(text_str);

	SetWindowText(hLabel, text_lstr);
	MessageBox(
		hWnd_Button,
		text_lstr,
		TEXT("OCR Result"), MB_ICONINFORMATION | MB_OK
	);

	// CAS_OCR::release_model();
}

void on_click_btn3(HWND hWnd_Button)
{
	const auto lp_output_string = select_pic_str();
	OutputDebugString(lp_output_string);

	const auto file_path =
		ConvertLPCWSTRToStdString(lp_output_string);

	if (file_path.empty())
	{
		MessageBox(
			hWnd_Button,
			TEXT("File Path is Empty!!!"),
			TEXT("Error"), MB_ICONERROR | MB_OK
		);
		return;
	}

	if (auto img = cv::imread(file_path); img.empty())
	{
		MessageBox(
			hWnd_Button,
			TEXT("Failed to load image!"),
			TEXT("Error"), MB_ICONERROR | MB_OK
		);
	}
	else
	{
		fix_channels(img);
		current_img = img;
		set_widget_image(hImageControl, img);
	}
}

void try_to_release_model()
{
	CAS_OCR::release_model();

	MessageBox(
		hMainWindow,
		TEXT("Model Released!"),
		TEXT("Info"), MB_ICONINFORMATION | MB_OK
	);
}
