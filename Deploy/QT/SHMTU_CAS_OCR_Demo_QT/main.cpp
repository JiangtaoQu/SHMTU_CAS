#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "cross_platform_macro.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    qDebug() << "Current Qt version:" << qVersion();

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale: uiLanguages) {
        const QString baseName = "SHMTU_CAS_OCR_Demo_QT_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            QApplication::installTranslator(&translator);
            break;
        }
    }

    MainWindow w;
    w.show();

    return QApplication::exec();
}
