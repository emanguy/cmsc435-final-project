#-------------------------------------------------
#
# Project created by QtCreator 2016-05-04T00:02:50
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FinalProject
TEMPLATE = app


SOURCES += main.cpp\
        nielsenvis.cpp \
    renderarea.cpp \
    BarChart.cpp \
    Chart.cpp \
    DDIMatrix.cpp \
    Triangle.cpp

HEADERS  += nielsenvis.h \
    renderarea.h \
    BarChart.h \
    Chart.h \
    DDIMatrix.h \
    Triangle.h

FORMS    += nielsenvis.ui

LIBS += -lglut -lGLU
