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
INCLUDEPATH += ./code


SOURCES += code/main.cpp \
    code/locator.cpp \
    code/log.cpp \
    code/jeeves.cpp \
    code/interrogator.cpp \
    code/build.cpp \
    code/statusscreen.cpp \
    code/builds.cpp \
    code/builder.cpp

HEADERS += \
    code/errors.h \
    code/network.h \
    code/locator.h \
    code/log.h \
    code/jeeves.h \
    code/interrogator.h \
    code/build.h \
    code/statusscreen.h \
    code/builds.h \
    code/builder.h

RESOURCES += \
    jeeves.qrc \

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
