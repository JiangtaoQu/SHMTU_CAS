// WindowsProject1.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "WinMain1.h"
// #include <ShellScalingApi.h>

#include <opencv2/opencv.hpp>

#include <string>

#include "SelectUtils.h"
#include "SHMTU_CAS_URL.h"
#include "WindowUtils.h"

#include "../NCNN_CLI/CAS_OCR.h"

constexpr auto window_main_size_width = 520;
constexpr auto window_main_size_height = 300;

// 全局变量
HWND hButton1, hButton2, hImageControl, hLabel;;

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

	HWND hWnd = CreateWindowW(
		szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		window_main_size_width, window_main_size_height,
		nullptr, nullptr, hInstance, nullptr
	);

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
	void on_click_btn1(HWND hWnd);
	void on_click_btn2(HWND hWnd);
	void on_click_btn3(HWND hWnd);

	switch (message)
	{
	case WM_CREATE:
		{
			// 创建按钮-从URL加载图片
			hButton1 = CreateWindow(TEXT("BUTTON"), TEXT("Download URL"),
			                        WS_VISIBLE | WS_CHILD,
			                        400, 20, 100, 30,
			                        hWnd, reinterpret_cast<HMENU>(1), NULL, NULL);

			// 创建按钮-OCR
			hButton2 = CreateWindow(TEXT("BUTTON"), TEXT("OCR"),
			                        WS_VISIBLE | WS_CHILD,
			                        400, 60, 100, 30,
			                        hWnd, reinterpret_cast<HMENU>(2), NULL, NULL);

			// 创建按钮-从本地打开
			hButton2 = CreateWindow(TEXT("BUTTON"), TEXT("Local Image"),
			                        WS_VISIBLE | WS_CHILD,
			                        400, 100, 100, 30,
			                        hWnd, reinterpret_cast<HMENU>(3), NULL, NULL);

			// 创建显示图片的控件
			hImageControl = CreateWindow(TEXT("STATIC"), NULL,
			                             WS_VISIBLE | WS_CHILD | SS_BITMAP,
			                             0, 0, 400, 140,
			                             hWnd, NULL, NULL, NULL);

			// 显示OCR结果
			hLabel = CreateWindow(TEXT("STATIC"), TEXT("[Wait For OCR]"),
			                      WS_VISIBLE | WS_CHILD | SS_CENTER,
			                      10, 150, 200, 30,
			                      hWnd, NULL, NULL, NULL);

			// Author Info
			CreateWindow(TEXT("STATIC"), TEXT("Author:Haomin Kong"),
			             WS_VISIBLE | WS_CHILD | SS_CENTER,
			             10, 180, 200, 30,
			             hWnd, NULL, NULL, NULL);
		}
		break;
	case WM_COMMAND:
		{
			// 检查按钮点击事件
			if (HIWORD(wParam) == BN_CLICKED)
			{
				// 检查是哪个按钮被点击
				if (LOWORD(wParam) == 1)
				{
					// 按钮1被点击
					on_click_btn1(hWnd);
				}
				else if (LOWORD(wParam) == 2)
				{
					// 按钮2被点击
					on_click_btn2(hWnd);
				}
				else if (LOWORD(wParam) == 3)
				{
					// 按钮3被点击
					on_click_btn3(hWnd);
				}
			}

			//////////////////////////////////////////
			int wmId = LOWORD(wParam);
			// 分析菜单选择:
			switch (wmId)
			{
			case IDM_DOWNLOAD_URL:
				on_click_btn1(hWnd);
				break;
			case IDM_OPEN_LOCAL:
				on_click_btn3(hWnd);
				break;
			case IDM_EXIT:
				DestroyWindow(hWnd);
				break;
			case IDM_DO_OCR:
				on_click_btn2(hWnd);
				break;
			case IDM_ABOUT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			// TODO: 在此处添加使用 hdc 的任何绘图代码...
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

void on_click_btn1(HWND hWnd)
{
	// 按钮1被点击
	const auto img = get_captcha_by_url();
	if (!img.empty())
	{
		current_img = img;
		set_widget_image(hImageControl, current_img);
	}
	else
	{
		MessageBox(hWnd, TEXT("Failed to load image!"), TEXT("Error"), MB_ICONERROR | MB_OK);
	}
}

void on_click_btn2(HWND hWnd)
{
	CAS_OCR::set_model_gpu_support(true);

	CAS_OCR::init_model(
		"",
		"fp16"
	);

	if (current_img.empty())
	{
		MessageBox(
			hWnd,
			TEXT("No image loaded!"),
			TEXT("Error"), MB_ICONERROR | MB_OK
		);
		return;
	}

	const auto result =
		CAS_OCR::predict_validate_code(current_img);

	const auto text_str = std::get<1>(result);
	const auto text_lstr = StringToLPCWSTR(text_str);

	SetWindowText(hLabel, text_lstr);
	MessageBox(
		hWnd,
		text_lstr,
		TEXT("OCR Result"), MB_ICONINFORMATION | MB_OK
	);

	// CAS_OCR::release_model();
}

void on_click_btn3(HWND hWnd)
{
	const auto lp_output_string = select_pic_str();
	OutputDebugString(lp_output_string);

	const auto file_path = LPCWSTRToString(lp_output_string);
	if (file_path.empty())
	{
		return;
	}

	const auto img = cv::imread(file_path);

	if (img.empty())
	{
		MessageBox(
			hWnd,
			TEXT("Failed to load image!"),
			TEXT("Error"), MB_ICONERROR | MB_OK
		);
	}
	else
	{
		cv::imshow("captcha", img);
		set_widget_image(hImageControl, img);
	}
}
