//
// Created by konghaomin on 2024/2/12.
//

#include "CAS_OCR_QT.h"

namespace CAS_OCR {
    std::tuple<int, std::string, int, int, int, int>
    predict_validate_code(const QPixmap &image_input, bool use_gpu) {
        cv::Mat image;
        image = QtOpenCV::QPixmapToCvMat(image_input);
        return CAS_OCR::predict_validate_code(image,use_gpu);
    }
}
