#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<QtCore>
#include<QMessageBox>
#include <qfiledialog.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    capwebcam.open(0);

    capwebcam.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    capwebcam.set(CV_CAP_PROP_FRAME_HEIGHT,480);

    frameTimer = new QTimer(this);

    connect(frameTimer, SIGNAL(timeout()), this, SLOT(processFrameAndUpdateGUI()));
    frameTimer->start(30);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::processFrameAndUpdateGUI()
{
    capwebcam.read(img);

    //cv::cvtColor(matOrg, matOrg,CV_BGR2RGB);
    cv::cvtColor(img, hsv, CV_BGR2HSV);

    cv::inRange(hsv, cv::Scalar(110, 50, 50), cv::Scalar(130, 255, 255), binary);

    //cv::Canny(matOrg,matCanny,50,100);
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15, 15));
    cv::erode(binary, binary, element);
    cv::dilate(binary, binary, element);

    std::vector< std::vector<cv::Point> > contours;
    findContours(binary, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

    drawContours(binary, contours, -1, cv::Scalar(255), CV_FILLED);

    for (std::vector<cv::Point> contour : contours) {
              // dibujar rectangulo
              cv::Rect r = boundingRect(contour);
              rectangle(img, r, CV_RGB(255, 0, 0), 2, CV_AA, 0);

              cv::Point center(r.x + (r.width / 2), r.y + (r.height / 2));

              std::ostringstream str;
              str << center.x << "," << center.y;

              // mostrar texto (x, y)
              cv::putText(img, str.str(), center, cv::FONT_HERSHEY_COMPLEX_SMALL, 0.60, CV_RGB(0, 255, 0), 1, CV_AA);
          }

    imageOrg=QImage((uchar*)img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
    imageCanny=QImage((uchar*)binary.data, binary.cols, binary.rows, binary.step, QImage::Format_Indexed8);


  ui -> lblCapture -> setPixmap(QPixmap::fromImage(imageOrg));
  ui -> lblCanny -> setPixmap(QPixmap::fromImage(imageCanny));

}


void MainWindow::closeEvent (QCloseEvent *event)
{

    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "OPENCV","¿Quieres salir?",QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes)
    {

        event->ignore();
    }
    else
    {

        exitProgram();
        event->accept();
    }
}

void MainWindow::exitProgram()
{

    if (capwebcam.isOpened())
        {
            capwebcam.release();
        }


    if(frameTimer->isActive()) frameTimer->stop();


    QApplication::quit();
}
