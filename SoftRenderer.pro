#-------------------------------------------------
#
# Project created by QtCreator 2021-07-07T13:24:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SoftRenderer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        widget.cpp \
    Scene/gscene.cpp \
    Raster/graster.cpp \
    Math/gmath.cpp \
    Model/gmodel.cpp \
    Scene/ggameobject.cpp \
    Scene/gcamera.cpp \
    Model/gmesh.cpp \
    Raster/gframebuffer.cpp \
    Raster/gdepthbuffer.cpp \
    Shader/gshader.cpp \
    Raster/gvertexattribute.cpp \
    Raster/gprimitive.cpp \
    Model/gmaterial.cpp \
    Shader/phongshader.cpp \
    Shader/skyboxshader.cpp \
    Scene/glight.cpp \
    Math/gsh.cpp \
    Math/gshrotate.cpp \
    Shader/shshader.cpp \
    Shader/brdfshader.cpp \
    Raster/gfragmentattribute.cpp

HEADERS  += widget.h \
    Scene/gscene.h \
    Raster/graster.h \
    Math/gmath.h \
    Model/gmodel.h \
    Scene/ggameobject.h \
    Scene/gcamera.h \
    Model/gmesh.h \
    Raster/gframebuffer.h \
    Raster/gdepthbuffer.h \
    Shader/gshader.h \
    Raster/gvertexattribute.h \
    Raster/gprimitive.h \
    Model/gmaterial.h \
    Shader/phongshader.h \
    Shader/skyboxshader.h \
    Scene/glight.h \
    Math/gsh.h \
    Math/gshrotate.h \
    Shader/shshader.h \
    Shader/brdfshader.h \
    Raster/gfragmentattribute.h

RESOURCES += \
    Resource/assets.qrc
