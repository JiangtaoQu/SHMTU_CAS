#pragma once

#include <opencv2/core.hpp>
#include <QPixmap>

namespace QtOpenCV {
    // 将 cv::Mat 转换为 QPixmap
    QPixmap CvMatToQPixmap(const cv::Mat &mat);

    // 将 QPixmap 转换为 cv::Mat
    cv::Mat QPixmapToCvMat(const QPixmap &pixmap);
}