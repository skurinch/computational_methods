TEMPLATE = app
TARGET = seir
DESTDIR = .

DEPENDPATH += .
INCLUDEPATH += . ..

mac {
    CONFIG -= app_bundle
}

QT += core gui widgets

CONFIG += release c++17
CONFIG += link_pkgconfig

SOURCES += seir.cpp

mac {
    PKG_CONFIG_PATH += /opt/homebrew/opt/qat/lib/pkgconfig
    PKG_CONFIG_PATH += /opt/homebrew/lib/pkgconfig
    PKG_CONFIG_PATH += /opt/homebrew/opt/gsl/lib/pkgconfig
    PKG_CONFIG_PATH += /opt/homebrew/opt/open-mpi/lib/pkgconfig
    PKG_CONFIG_PATH += /opt/homebrew/share/pkgconfig

    PKG_CONFIG = /opt/homebrew/bin/pkg-config

    QMAKE_MACOSX_DEPLOYMENT_TARGET = 14.4
}

PKGCONFIG += QatGenericFunctions
PKGCONFIG += QatPlotting
PKGCONFIG += QatPlotWidgets