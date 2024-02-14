// Image Utils using OpenCV
// Author: Haomin Kong
// Date: 2024-02-09

#pragma once

#include <opencv2/opencv.hpp>
#include "framework.h"

HBITMAP cv_mat_2_hbitmap(const cv::Mat& image);
