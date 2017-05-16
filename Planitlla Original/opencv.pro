#-------------------------------------------------
#
# Project created by QtCreator 2015-12-15T17:39:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = opencv
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

INCLUDEPATH+=D:\\MESUT\\opencv\\opencv-2.4.10\\build\\include

LIBS+=-LD:\\MESUT\\opencv\\opencv-2.4.10\\build\\x86\\vc10\\lib \
    -lopencv_calib3d2410 \
    -lopencv_contrib2410 \
    -lopencv_core2410 \
    -lopencv_features2d2410 \
    -lopencv_flann2410 \
    -lopencv_gpu2410 \
    -lopencv_highgui2410 \
    -lopencv_imgproc2410 \
    -lopencv_legacy2410 \
    -lopencv_ml2410 \
    -lopencv_nonfree2410 \
    -lopencv_objdetect2410 \
    -lopencv_ocl2410 \
    -lopencv_photo2410 \
    -lopencv_stitching2410 \
    -lopencv_superres2410 \
    -lopencv_ts2410 \
    -lopencv_video2410 \
    -lopencv_videostab2410

