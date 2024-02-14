//
// Created by konghaomin on 2024/2/13.
//

#include "CasNetwork.h"

QPixmap DownloadQPixmapFromUrl(const std::string &url) {
    QNetworkAccessManager manager;
    QEventLoop loop;
    QNetworkReply *reply = manager.get(
        QNetworkRequest(
            QUrl(
                QString::fromStdString(url)
            )
        )
    );
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    QByteArray data = reply->readAll();
    QPixmap pixmap;
    pixmap.loadFromData(data);
    return pixmap;
}
