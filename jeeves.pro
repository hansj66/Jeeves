#-------------------------------------------------
#
# Project created by QtCreator 2013-03-19T11:27:34
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = jeeves
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += $$(BOOSTPATH)

SOURCES += main.cpp \
    locator.cpp \
    log.cpp \
    jeeves.cpp

HEADERS += \
    errors.h \
    network.h \
    locator.h \
    log.h \
    jeeves.h

macx {
    #Use clang for C++11
    QMAKE_CXXFLAGS += -std=c++11
}

!mac:unix {
}

win32 {
    win32-msvc:QMAKE_CXXFLAGS += /MP
}
