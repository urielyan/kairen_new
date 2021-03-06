#-------------------------------------------------
#
# Project created by QtCreator 2016-10-08T19:50:37
#
#-------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = kairen_new
TEMPLATE = app


SOURCES += main.cpp\
    common/wininforlistdialog.cpp \
    common/abstractfactory.cpp \
    windows/buttonwinmannager.cpp \
    windows/mainwindow.cpp \
    windows/statusbar.cpp \
    windows/winabstractframe.cpp \
    windows/wincalibratemeasure.cpp \
    windows/wincountmeasure.cpp \
    windows/winspectrumeasure.cpp \
    windows/winsystrmsettings.cpp \
    common/numberinput.cpp \
    communication/posix_qextserialport.cpp \
    communication/qextserialbase.cpp \
    communication/com.cpp \
    windows/winmainframe.cpp \
    common/datasave.cpp \
    windows/winquerydata.cpp \
    common/database.cpp

HEADERS  += mainwindow.h \
    common/wininforlistdialog.h \
    common/abstractfactory.h \
    windows/buttonwinmannager.h \
    common/global.h \
    windows/mainwindow.h \
    windows/statusbar.h \
    windows/winabstractframe.h \
    windows/wincalibratemeasure.h \
    windows/wincountmeasure.h \
    windows/winspectrumeasure.h \
    windows/winsystrmsettings.h \
    common/numberinput.h \
    communication/posix_qextserialport.h \
    communication/qextserialbase.h \
    communication/com.h \
    windows/winmainframe.h \
    common/datasave.h \
    windows/winquerydata.h \
    common/database.h
