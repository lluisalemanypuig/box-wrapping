#-------------------------------------------------
#
# Project created by QtCreator 2018-04-27T10:30:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = box-wrapper
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

SOURCES += \
    main.cpp \
    box_organizer.cpp \
    box_wrapper_mw.cpp \
    loadbutton.cpp \
    savebutton.cpp

HEADERS += \
    box_organizer.hpp \
    box_wrapper_mw.hpp \
    loadbutton.hpp \
    savebutton.hpp

FORMS += \
    boxwrappermainwindow.ui
