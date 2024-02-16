#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

QT_BEGIN_NAMESPACE

namespace Ui {
    class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

private slots:
    void on_pushButton_DownloadUrl_clicked();

    void on_pushButton_ocr_clicked();

    void on_pushButton_OpenLocal_clicked();

    void on_actionReleaseModel_triggered();

    void on_actionDoOCR_triggered();

    void on_actionExit_triggered();

    void on_actionDownload_From_URL_triggered();

    void on_actionOpenLocal_triggered();

    void on_actionAbout_triggered();

    void on_pushButton_ReleaseModel_clicked();

private:
    Ui::MainWindow *ui;
    QPixmap _innerBitmap;

    void SetPixelMapToLabel(QLabel *label, const QPixmap &pixmap);
};

#endif // MAINWINDOW_H
