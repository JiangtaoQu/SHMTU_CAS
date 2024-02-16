#pragma once
#include "ImageUtils.h"

void fix_channels(cv::Mat& cv_mat);

void set_widget_image(HWND image_control, cv::Mat cv_mat, bool img_fix_control = true);

void set_widget_image(HWND image_control, HBITMAP h_bitmap);

bool check_box_is_checked(HWND hWnd);

cv::Mat get_image_from_static_control(HWND h_static_control);
