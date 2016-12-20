#-------------------------------------------------
#
# Project created by QtCreator 2016-03-30T12:21:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EyeTracking
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    renderthread.cpp \
    eyetracking.cpp

HEADERS  += mainwindow.h \
    renderthread.h \
    eyetracking.h

FORMS    += mainwindow.ui

RC_ICONS = ETicon.ico


win32: LIBS += -L$$PWD/'../../../../../../../../Program Files (x86)/OptiTrack/Camera SDK/lib/' -lCameraLibrary2010x64S

INCLUDEPATH += $$PWD/'../../../../../../../../Program Files (x86)/OptiTrack/Camera SDK/include'
DEPENDPATH += $$PWD/'../../../../../../../../Program Files (x86)/OptiTrack/Camera SDK/include'

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/'../../../../../../../../Program Files (x86)/OptiTrack/Camera SDK/lib/CameraLibrary2010x64S.lib'
else:win32-g++: PRE_TARGETDEPS += $$PWD/'../../../../../../../../Program Files (x86)/OptiTrack/Camera SDK/lib/libCameraLibrary2010x64S.a'
