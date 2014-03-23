#-------------------------------------------------
#
# Project created by QtCreator 2014-03-17T22:50:48
#
#-------------------------------------------------

QT       += core gui

TARGET = EAN
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Interval/IntervalArithmetic.cpp \
    mathematics.cpp

HEADERS  += mainwindow.h \
    Interval/IntervalArithmetic.h \
    mathematics.h

FORMS    += mainwindow.ui

QMAKE_CXXFLAGS += -std=c++11
LIBS += -lmpfr
