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
    renderarea.cpp

HEADERS  += nielsenvis.h \
    renderarea.h

FORMS    += nielsenvis.ui

LIBS += -lglut -lGLU
