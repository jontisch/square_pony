#-------------------------------------------------
#
# Project created by QtCreator 2017-12-12T21:35:23
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = square_pony
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
        mainwindow.cpp \
    player.cpp \
    globals.cpp \
    map.cpp \
    item.cpp \
    editor.cpp \
    movingobject.cpp \
    wrappingobject.cpp \
    selectablepolygonf.cpp \
    editorwidget.cpp \
    enemy.cpp \
    boxcollider.cpp

HEADERS  += mainwindow.h \
    player.h \
    globals.h \
    map.h \
    item.h \
    editor.h \
    movingobject.h \
    wrappingobject.h \
    selectablepolygonf.h \
    editorwidget.h \
    enemy.h \
    boxcollider.h

FORMS    += mainwindow.ui

RESOURCES += \
    graphics.qrc

DISTFILES +=
