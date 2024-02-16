// ReSharper disable CppClangTidyClangDiagnosticInvalidUtf8
#include "WindowUtils.h"

#include <vector>

void fix_channels(cv::Mat& cv_mat)
{
	// 如果通道数不等于 3，则进行修正
	if (cv_mat.channels() != 3)
	{
		// 如果是单通道或多通道但非 RGB 格式的图像，则转换为 3 通道 RGB 格式
		if (cv_mat.channels() == 1 || cv_mat.channels() == 4)
		{
			cv::Mat temp;
			// 如果是单通道图像，转换为 3 通道灰度图像
			cv::cvtColor(cv_mat, temp, cv::COLOR_GRAY2BGR);
			cv_mat = temp;
		}
		else
		{
			cv::Mat temp;
			// 如果是其他多通道图像，转换为 3 通道 RGB 图像
			cv::cvtColor(cv_mat, temp, cv::COLOR_BGR2RGB);
			cv_mat = temp;
		}
	}
}

void set_widget_image(const HWND image_control, cv::Mat cv_mat, const bool img_fix_control)
{
	if (img_fix_control)
	{
		RECT rect;
		GetClientRect(image_control, &rect);

		const int control_width = rect.right - rect.left;
		const int control_height = rect.bottom - rect.top;

		cv::resize(
			cv_mat,
			cv_mat,
			cv::Size(control_width, control_height)
		);
	}

	fix_channels(cv_mat);

	const auto h_bitmap = cv_mat_2_hbitmap(cv_mat);
	set_widget_image(image_control, h_bitmap);
}

void set_widget_image(const HWND image_control, HBITMAP h_bitmap)
{
	// 设置位图到静态控件
	SendMessage(image_control, STM_SETIMAGE, IMAGE_BITMAP, reinterpret_cast<LPARAM>(h_bitmap));
}

bool check_box_is_checked(const HWND hWnd)
{
	const BOOL is_checked =
		SendMessage(hWnd, BM_GETCHECK, 0, 0);
	return is_checked == BST_CHECKED;
}

// 获取静态控件的图像并保存到cv::Mat中
cv::Mat get_image_from_static_control(const HWND h_static_control)
{
	RECT rect;
	GetClientRect(h_static_control, &rect);

	// 创建内存DC
	const HDC hdc_screen = GetDC(h_static_control);
	HDC hdc_mem = CreateCompatibleDC(hdc_screen);

	// 创建位图对象
	const HBITMAP h_bitmap = CreateCompatibleBitmap(hdc_screen, rect.right - rect.left, rect.bottom - rect.top);

	// 将位图选入内存DC
	const auto h_bitmap_old = static_cast<HBITMAP>(SelectObject(hdc_mem, h_bitmap));

	// 设置拉伸模式，防止图像翻转
	SetStretchBltMode(hdc_mem, STRETCH_DELETESCANS);

	// 将静态控件的内容绘制到内存DC
	SendMessage(h_static_control, WM_PRINT, reinterpret_cast<WPARAM>(hdc_mem), PRF_CLIENT);

	// 将位图恢复到内存DC
	SelectObject(hdc_mem, h_bitmap_old);

	// 从内存DC获取位图信息
	BITMAPINFOHEADER bitmap_info_header = {};
	bitmap_info_header.biSize = sizeof(BITMAPINFOHEADER);
	bitmap_info_header.biWidth = rect.right - rect.left;
	bitmap_info_header.biHeight = rect.bottom - rect.top;
	bitmap_info_header.biPlanes = 1;
	// 32位色彩，每像素4字节
	bitmap_info_header.biBitCount = 32;
	bitmap_info_header.biCompression = BI_RGB;
	// 计算图像数据大小
	bitmap_info_header.biSizeImage = (
		(bitmap_info_header.biWidth * bitmap_info_header.biBitCount + 31) / 32
	) * 4 * bitmap_info_header.biHeight;

	// 分配足够的缓冲区
	std::vector<uchar> vec_image_buffer(bitmap_info_header.biSizeImage);
	GetDIBits(
		hdc_mem, h_bitmap,
		0, bitmap_info_header.biHeight,
		vec_image_buffer.data(),
		reinterpret_cast<BITMAPINFO*>(&bitmap_info_header), DIB_RGB_COLORS
	);

	// 释放资源
	DeleteObject(h_bitmap);
	DeleteDC(hdc_mem);
	ReleaseDC(h_static_control, hdc_screen);

	// 创建OpenCV的cv::Mat对象
	cv::Mat matImage(bitmap_info_header.biHeight, bitmap_info_header.biWidth, CV_8UC4, vec_image_buffer.data());
	// 将颜色通道顺序改为BGR
	cv::cvtColor(matImage, matImage, cv::COLOR_BGRA2BGR);

	// 垂直翻转图像
	cv::flip(matImage, matImage, 0);

	return matImage;
}
