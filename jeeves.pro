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
    code/FileDownloader.cpp \
    code/build.cpp \
    code/statusscreen.cpp \
    code/builder.cpp \
    code/builders.cpp \
    code/HtmlGenerator.cpp

HEADERS += \
    code/errors.h \
    code/network.h \
    code/locator.h \
    code/log.h \
    code/jeeves.h \
    code/FileDownloader.h \
    code/build.h \
    code/statusscreen.h \
    code/builder.h \
    code/builders.h \
    code/HtmlGenerator.h

RESOURCES += \
    jeeves.qrc \

macx {
    # Mac-specific if you are compiling, using gcc. Update these two lines to match your target Mac OS SDK
    QMAKE_MACOSX_DEPLOYMENT_TARGET=10.8
    QMAKE_MAC_SDK = /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.8.sdk
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
