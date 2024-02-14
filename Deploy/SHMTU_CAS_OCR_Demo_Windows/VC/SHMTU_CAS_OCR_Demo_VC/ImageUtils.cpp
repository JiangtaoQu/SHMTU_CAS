// Image Utils using OpenCV
// Author: Haomin Kong
// Date: 2024-02-09

#include "ImageUtils.h"

HBITMAP cv_mat_2_hbitmap(const cv::Mat& image)
{
	if (image.empty())
	{
		return nullptr;
	}

	const int width = image.cols;
	const int height = image.rows;
	const int channels = image.channels();

	cv::Mat bmpImage(height, width, CV_8UC4);
	cv::cvtColor(image, bmpImage, cv::COLOR_BGR2BGRA);

	return CreateBitmap(width, height, 1, 32, bmpImage.data);
}
