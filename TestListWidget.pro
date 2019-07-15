#-------------------------------------------------
#
# Project created by QtCreator 2016-09-27T14:08:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TestListWidget
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    customscene.cpp \
    exif-master/exif.c

HEADERS  += widget.h \
    customscene.h \
    exif-master/exif.h

FORMS    += widget.ui

RESOURCES += \
    resource.qrc

DISTFILES +=
