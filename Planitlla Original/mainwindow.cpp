#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<QtCore>
#include<QMessageBox>
#include <qfiledialog.h>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //webcam'den görüntü almak için gerekli satırlar
    //Eğer sistemde birden çok kamera varsa 0 yerine 1,2,..3 kullanabiliriz.
    capWebcam.open(0);

    //capture boyutlarımızı ayarlıyoruz. Frame'leri daha hızlı proses edebilmek için boyutu küçültmemiz gerekebilir.
    //Bir pc için bu gerekli olmayabilir ama daha yavaş, Raspberry pi gibi sistemlerde buna ihtiyaç duyarız.
    capWebcam.set(CV_CAP_PROP_FRAME_WIDTH,640);
    capWebcam.set(CV_CAP_PROP_FRAME_HEIGHT,480);


    //Bir Qt timer'ı ayarlıyoruz. Bu timer her 30ms'de bir processFrameAndUpdateGUI metodumuzu çağıracak.
    //Böylece görüntü tazeleme ve process etme işi bu kısımda yapılacak.
    //Bu iş için thread'de kullanabilirdik. Fakat QTimer sınıfı işimizi basitleştirecektir.
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

    //Kameradan sıradaki frame'i okuyoruz.
    capWebcam.read(matOrg);

    //Okunan frame'i BGR renk uzayından RGB uzayına dönüştürüyoruz.
    //Opencv webcam'den aldığı görüntü her zaman BGR uzayındadır.
    cv::cvtColor(matOrg, matOrg, CV_BGR2RGB);

    //Görüntüyü 50 ve 100 parametrelerini kullanarak "canny edges" metodu ile işiyoruz.
    cv::Canny(matOrg,matCanny,50,100);


    //Bu kısımda opencv matrislerini QImage'a dönüştürüyoruz.
    //RGB image için QImage::Format_RGB888 parametresini
    //Gray image içib QImage::Format_Indexed8 parametresini kullanacağız
    imageOrg=QImage((uchar*)matOrg.data, matOrg.cols, matOrg.rows, matOrg.step, QImage::Format_RGB888);
    imageCanny=QImage((uchar*)matCanny.data, matCanny.cols, matCanny.rows, matCanny.step, QImage::Format_Indexed8);

    //Image'larımızı UI üzerinde tanımlamış olduğumuz label'lara aktarıyoruz
    ui->lblCapture->setPixmap(QPixmap::fromImage(imageOrg));
    ui->lblCanny->setPixmap(QPixmap::fromImage(imageCanny));

}

void MainWindow::closeEvent (QCloseEvent *event)
{
    //Programdan çıkmak istediğimizde bu event çalıştırılıyor ve bize çıkmak isteyip istemediğimizi soruyor
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "OPENCV","Çıkmak istiyor musunu?",QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes)
    {
        //Hayır çıkmayalım
        event->ignore();
    }
    else
    {
        //Evet çıkalım
        exitProgram();
        event->accept();
    }
}

void MainWindow::exitProgram()
{
    //Çıkış için webcam'imizi kapatalım
    if (capWebcam.isOpened())
        {
            capWebcam.release();
        }

    //QTimer'ı sonlandıralım
    if(frameTimer->isActive()) frameTimer->stop();

    //Ve çıkalım
    QApplication::quit();
}
