#-------------------------------------------------
#
# Project created by QtCreator 2014-05-25T15:08:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VTK_Loader
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
		vtk_loader.cpp \
    loader_widget.cpp \
    vtk_filter.cpp \
	helpers.cpp \
    datafield/cdatafieldint.cpp \
    datafield/cadatafield.cpp \
    datafield/cdatafielduint.cpp \
    datafield/cdatafieldfloat.cpp \
    datafield/cdatafielddouble.cpp \
    datafield/cdatafielduchar.cpp \
	datafield/cdatafieldvtkpoint.cpp \
    datafield/cdatafieldt.cpp \
    render_widget.cpp \
    ccamera.cpp \
    cfiledata.cpp


HEADERS  += mainwindow.h \
    vtk_loader.h \
    loader_widget.h \
    datastruct.h \
    vtk_filter.h \
	helpers.h \
    datafield/cdatafieldint.h \
    datafield/cadatafield.h \
    datafield/cdatafielduint.h \
    datafield/datafieldincludes.h \
    datafield/cdatafieldfloat.h \
    datafield/cdatafielddouble.h \
    datafield/cdatafielduchar.h \
    vtkpoint.h \
	datafield/cdatafieldvtkpoint.h \
    datafield/cdatafieldt.h \
    render_widget.h \
    ccamera.h \
    cfiledata.h

FORMS    += mainwindow.ui \
    loaderwidget.ui

QT           += opengl


INCLUDEPATH += C:/boostLib

OTHER_FILES += \
	texture/earth_8k.jpg \
	texture/earthmap1k.png
