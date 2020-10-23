#-------------------------------------------------
#
# Project created by QtCreator 2020-10-21T09:52:40
#
#-------------------------------------------------

QT       += core gui
RC_ICONS = app.ico

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = example5_3
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    helpdialog.cpp

HEADERS  += mainwindow.h \
    helpdialog.h

FORMS    += mainwindow.ui

CONFIG += c++11

RESOURCES += \
    recicon.qrc
