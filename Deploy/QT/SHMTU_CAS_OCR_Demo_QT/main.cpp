#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <iostream>

// #ifndef _DEBUG
// #ifdef DEBUG_MODE
// #define _DEBUG
// #endif
// #endif

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale: uiLanguages) {
        const QString baseName = "SHMTU_CAS_OCR_Demo_QT_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            QApplication::installTranslator(&translator);
            break;
        }
    }

#ifndef NDEBUG
    std::cout << "Hello, World! Debug Mode" << std::endl;
#else
    std::cout << "Hello, World! Release Mode" << std::endl;
#endif


    MainWindow w;
    w.show();

    return QApplication::exec();
}
