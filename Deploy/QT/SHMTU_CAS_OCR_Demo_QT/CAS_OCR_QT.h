//
// Created by konghaomin on 2024/2/12.
//

#ifndef SHMTU_CAS_OCR_DEMO_QT_CAS_OCR_QT_H
#define SHMTU_CAS_OCR_DEMO_QT_CAS_OCR_QT_H

#include "QtOpenCV.hpp"
#include "CAS_OCR.h"

namespace CAS_OCR {
    std::tuple<int, std::string, int, int, int, int>
    predict_validate_code(const QPixmap &image_input, bool use_gpu = true);
}

#endif //SHMTU_CAS_OCR_DEMO_QT_CAS_OCR_QT_H
