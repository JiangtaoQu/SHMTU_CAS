#pragma once
#include "ImageUtils.h"

void set_widget_image(HWND image_control, cv::Mat cv_mat);
void set_widget_image(HWND image_control, HBITMAP h_bitmap);
cv::Mat get_image_from_static_control(HWND h_static_control);