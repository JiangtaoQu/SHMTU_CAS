#include "QtOpenCV.hpp"

#include <QImage>

#include <opencv2/imgproc.hpp>

namespace QtOpenCV {
    // 将 cv::Mat 转换为 QPixmap
    QPixmap CvMatToQPixmap(const cv::Mat &mat) {
        const QImage image(
            mat.data,
            mat.cols, mat.rows,
            static_cast<int>(mat.step),
            QImage::Format_RGB888
        );
        return QPixmap::fromImage(image.rgbSwapped());
    }

    // 将 QPixmap 转换为 cv::Mat
    cv::Mat QPixmapToCvMat(const QPixmap &pixmap) {
        QImage image =
                pixmap.toImage().convertToFormat(QImage::Format_RGB888);
        cv::Mat mat(
            image.height(), image.width(), CV_8UC3,
            const_cast<uchar *>(image.bits()),
            static_cast<size_t>(image.bytesPerLine())
        );

        // OpenCV 默认使用 BGR 顺序
        cv::cvtColor(mat, mat, cv::COLOR_RGB2BGR);

        // 返回一个副本，避免潜在的内存共享问题
        return mat.clone();
    }
}
