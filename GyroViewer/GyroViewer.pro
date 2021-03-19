#-------------------------------------------------
#
# Project created by QtCreator 2021-01-08T17:54:23
#
#-------------------------------------------------

QT       += core gui widgets printsupport

RC_ICONS = logo.ico

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GyroViewer
TEMPLATE = app


SOURCES += main.cpp\
	mainwindow.cpp \
	qcustomplot.cpp \
	tycan.cpp \
    cali.cpp \
    matlab/eig.cpp \
    matlab/magmcali.cpp \
    matlab/magmcali_data.cpp \
    matlab/magmcali_initialize.cpp \
    matlab/magmcali_rtwutil.cpp \
    matlab/magmcali_terminate.cpp \
    matlab/minOrMax.cpp \
    matlab/recip.cpp \
    matlab/relop.cpp \
    matlab/rt_nonfinite.cpp \
    matlab/rtGetInf.cpp \
    matlab/rtGetNaN.cpp \
    matlab/schur.cpp \
    matlab/sqrt.cpp \
    matlab/xdlanv2.cpp \
    matlab/xdlapy3.cpp \
    matlab/xgerc.cpp \
    matlab/xhseqr.cpp \
    matlab/xnrm2.cpp \
    matlab/xzggev.cpp \
    matlab/xzhgeqz.cpp \
    matlab/xzlarf.cpp \
    matlab/xzlarfg.cpp \
    matlab/xzlartg.cpp \
    matlab/xztgevc.cpp \
    cali2.cpp


HEADERS  += mainwindow.h \
    qcustomplot.h \
    tycan.h \
    tygyro.h \
    ECanVci.h \
    matlab/coder_array.h \
    matlab/eig.h \
    matlab/magmcali.h \
    matlab/magmcali_data.h \
    matlab/magmcali_initialize.h \
    matlab/magmcali_rtwutil.h \
    matlab/magmcali_terminate.h \
    matlab/magmcali_types.h \
    matlab/minOrMax.h \
    matlab/recip.h \
    matlab/relop.h \
    matlab/rt_defines.h \
    matlab/rt_nonfinite.h \
    matlab/rtGetInf.h \
    matlab/rtGetNaN.h \
    matlab/rtwtypes.h \
    matlab/schur.h \
    matlab/sqrt.h \
    matlab/xdlanv2.h \
    matlab/xdlapy3.h \
    matlab/xgerc.h \
    matlab/xhseqr.h \
    matlab/xnrm2.h \
    matlab/xzggev.h \
    matlab/xzhgeqz.h \
    matlab/xzlarf.h \
    matlab/xzlarfg.h \
    matlab/xzlartg.h \
    matlab/xztgevc.h \
    matlab/tmwtypes.h


FORMS    += mainwindow.ui

LIBS += -L../ -lECanVci

DISTFILES += \
    logo.ico \
    CHUSBDLL.dll \
    ECanVci.dll

