#-------------------------------------------------
#
# Project created by QtCreator 2015-08-17T10:31:34
#
#-------------------------------------------------

QT             +=  core gui
QMAKE_CXXFLAGS -= -O1
QMAKE_CXXFLAGS -= -O2
QMAKE_CXXFLAGS -= -O3
QMAKE_CXXFLAGS += -O0

include(third_party/common/common.pri)
include(third_party/svgbutton/svgbutton.pri)
include(third_party/svgtoggleswitch/svgtoggleswitch.pri)
include (third_party/svgdialgauge/svgdialgauge.pri)
include (third_party/multislider/multislider.pri)

RESOURCES +=  third_party/skins/beryl_multislider.qrc          \
             third_party/skins/beryl_svgslideswitch.qrc       \
             third_party/skins/beryl_scrolldial.qrc           \
             third_party/skins/beryl_5waybutton.qrc           \
             third_party/skins/beryl_scrollwheel.qrc          \
             third_party/skins/beryl_svgtoggleswitch.qrc      \
             third_party/skins/beryl_svgbutton.qrc            \
             third_party/skins/berylsquare_svgbutton.qrc      \
             third_party/skins/berylsquare_svgtoggleswitch.qrc\
             third_party/skins/amperemeter_svgdialgauge.qrc   \
             third_party/skins/thermometer_svgdialgauge.qrc   \
             third_party/skins/tachometer_svgdialgauge.qrc \
             third_party/skins/tachometer_svgdialgauge-45.qrc \
             third_party/skins/tachometer_svgdialgauge-360.qrc \
             leds.qrc

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
LIBS += "-lrt"

TARGET = 3umant
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    moon2.cpp \
    gpio.cpp \
    qencoder.cpp \
    qencreader.cpp \
    umencoder.cpp \
    trackerElev.cpp \
    trackerazim.cpp

HEADERS  += mainwindow.h \
    moon2.h \
    gpio.h \
    qencoder.h \
    qencreader.h \
    umencoder.h \
    trackerElev.h \
    trackerazim.h

FORMS    += mainwindow.ui

target.path = /usr/local/bin

INSTALLS += target
