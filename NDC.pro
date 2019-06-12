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
    appointmentdialog.cpp \
        mainwindow.cpp \
    database.cpp \
    addpatientdialog.cpp \
    addeventdialog.cpp \
    mycalendarwidget.cpp \
    alleventsdialog.cpp \
    mysortfilterproxymodel.cpp

HEADERS  += mainwindow.h \
    appointmentdialog.h \
    general.h \
    database.h \
    addpatientdialog.h \
    addeventdialog.h \
    mycalendarwidget.h \
    alleventsdialog.h \
    mysortfilterproxymodel.h

FORMS    += mainwindow.ui \
    addpatientdialog.ui \
    addeventdialog.ui \
    alleventsdialog.ui \
    appointmentdialog.ui

RESOURCES += \
    res/res.qrc

RC_ICONS = main_icon.ico
