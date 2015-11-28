#-------------------------------------------------
#
# Project created by QtCreator 2015-08-16T02:05:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ea3um
TEMPLATE = app

CONFIG += qwt

SOURCES += main.cpp\
        mainwindow.cpp \
    moon2.cpp

HEADERS  += mainwindow.h \
    moon2.h

FORMS    += mainwindow.ui

target.path = /home/root

INSTALLS += target
