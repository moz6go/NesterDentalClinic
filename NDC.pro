#-------------------------------------------------
#
# Project created by QtCreator 2019-05-29T11:24:55
#
#-------------------------------------------------

QT       += core gui sql
CONFIG += c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NDC
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    database.cpp \
    addpatientdialog.cpp \
    addeventdialog.cpp \
    mycalendarwidget.cpp

HEADERS  += mainwindow.h \
    general.h \
    database.h \
    addpatientdialog.h \
    addeventdialog.h \
    mycalendarwidget.h

FORMS    += mainwindow.ui \
    addpatientdialog.ui \
    addeventdialog.ui

RESOURCES += \
    res/res.qrc

RC_ICONS = main_icon.ico
