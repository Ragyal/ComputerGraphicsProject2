#-------------------------------------------------
#
# Project created by QtCreator 2016-03-04T23:07:51
#
#-------------------------------------------------

QT     += core gui opengl

win32 {
    LIBS   += -lOpengl32
}

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = opengl1
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
    oglwidget.cpp \
    vertex.cpp \
    quad.cpp \
    mesh.cpp \
    surface.cpp \
    vectormatrix.cpp \
    matrix.cpp \
    curve.cpp

HEADERS  += mainwindow.h \
    oglwidget.h \
    vertex.h \
    quad.h \
    mesh.h \
    surface.h \
    vectormatrix.h \
    matrix.h \
    curve.h

FORMS += mainwindow.ui

DISTFILES +=
