#-------------------------------------------------
#
# Project created by QtCreator 2013-03-19T11:27:34
#
#-------------------------------------------------

QT       += core network xml gui

TARGET = jeeves
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += $$(BOOSTPATH)

SOURCES += main.cpp \
    locator.cpp \
    log.cpp \
    jeeves.cpp \
    interrogator.cpp \
    build.cpp \
    statusscreen.cpp

HEADERS += \
    errors.h \
    network.h \
    locator.h \
    log.h \
    jeeves.h \
    interrogator.h \
    build.h \
    statusscreen.h

macx {
    # Mac-specific if you are compiling, using gcc. Update these two lines to match your target Mac OS SDK
    QMAKE_MACOSX_DEPLOYMENT_TARGET=10.7
    QMAKE_MAC_SDK = /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.7.sdk
}

macx-clang {
    QMAKE_CXXFLAGS += -std=c++11
}

win32-msvc {
    QMAKE_CXXFLAGS += /MP
}

OTHER_FILES += \
    LICENSE.TXT \
    README.md
