#-------------------------------------------------
#
# Project created by QtCreator 2021-01-08T17:54:23
#
#-------------------------------------------------

QT       += core gui widgets printsupport

RC_ICONS = logo.ico

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GyroCali
TEMPLATE = app


SOURCES += main.cpp\
	mainwindow.cpp \
	qcustomplot.cpp \
	tycan.cpp \


HEADERS  += mainwindow.h \
    qcustomplot.h \
    tycan.h \
    tygyro.h \
    ECanVci.h \


FORMS    += mainwindow.ui

LIBS += -L../ -lECanVci

DISTFILES += \
    logo.ico \
    CHUSBDLL.dll \
    ECanVci.dll

