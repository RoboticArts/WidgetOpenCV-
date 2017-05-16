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
    capwebcam.read(matOrg);

    cv::cvtColor(matOrg, matOrg,CV_BGR2RGB);

    cv::Canny(matOrg,matCanny,50,100);

    imageOrg=QImage((uchar*)matOrg.data, matOrg.cols, matOrg.rows, matOrg.step, QImage::Format_RGB888);
    imageCanny=QImage((uchar*)matCanny.data, matCanny.cols, matCanny.rows, matCanny.step, QImage::Format_Indexed8);


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
