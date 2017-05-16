#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qevent.h>
#include <QLabel>

#include <string>
#include <iostream>
#include <vector>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/video.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:

    void processFrameAndUpdateGUI();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTimer *frameTimer;
    QImage imageOrg, imageCanny;

    cv::Mat matOrg;
    cv::Mat matCanny;
    cv::VideoCapture capwebcam;

    void exitProgram();
    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
