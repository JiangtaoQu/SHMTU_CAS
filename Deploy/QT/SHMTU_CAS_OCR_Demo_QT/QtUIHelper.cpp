//
// Created by konghaomin on 2024/2/13.
//

#include "QtUIHelper.h"

void setQPushButtonFixedSize(QPushButton *button) {
    button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    button->setFixedSize(button->size());
}
