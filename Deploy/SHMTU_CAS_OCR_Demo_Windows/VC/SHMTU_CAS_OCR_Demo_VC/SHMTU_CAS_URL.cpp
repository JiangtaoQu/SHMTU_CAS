#include "SHMTU_CAS_URL.h"
#include "framework.h"

#include <vector>
#include <wininet.h>

// Link with wininet.lib
#pragma comment(lib, "wininet.lib")


cv::Mat get_captcha_by_url()
{
	const auto url = "https://cas.shmtu.edu.cn/cas/captcha";
	auto image = cv::Mat();

	const auto h_internet =
		InternetOpen(
			TEXT("HttpClient"),
			INTERNET_OPEN_TYPE_PRECONFIG,
			nullptr,
			nullptr,
			0
		);
	if (h_internet)
	{
		const auto h_url =
			InternetOpenUrlA(
				h_internet,
				url,
				nullptr, 0, INTERNET_FLAG_RELOAD, 0
			);
		if (h_url)
		{
			DWORD bytes_read, total_bytes_read = 0;
			constexpr int buffer_size = 1024;
			char buffer[buffer_size];
			std::vector<uchar> image_buffer;
			while (InternetReadFile(h_url, buffer, buffer_size, &bytes_read) && bytes_read != 0)
			{
				total_bytes_read += bytes_read;
				image_buffer.insert(image_buffer.end(), buffer, buffer + bytes_read);
			}
			if (!image_buffer.empty())
			{
				image = cv::imdecode(cv::Mat(image_buffer), cv::IMREAD_COLOR);
			}
			else
			{
				std::cerr << "Failed to read image from URL!" << '\n';
			}

			InternetCloseHandle(h_url);
		}
		else
		{
			std::cerr << "Failed to open URL!" << '\n';
		}
		InternetCloseHandle(h_internet);
	}
	else
	{
		std::cerr << "Failed to open internet connection!" << '\n';
	}

#ifdef _DEBUG
	if (!image.empty())
	{
		OutputDebugString(TEXT("Image loaded successfully!\n"));
		std::string debug_message =
			"Width: " + std::to_string(image.cols)
			+ ", Height: " + std::to_string(image.rows)
			+ ", Channels: " + std::to_string(image.channels()) + "\n";
		const std::wstring wide_string(debug_message.begin(), debug_message.end());
		OutputDebugString(wide_string.c_str());
	}
#endif

	return image;
}
