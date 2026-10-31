TEMPLATE = app
TARGET = rainbow
DESTDIR = .
DEPENDPATH += .
INCLUDEPATH += . ..

mac {
    CONFIG -= app_bundle
}

CONFIG += qt release c++17

# Input
SOURCES += rainbow.cpp

QMAKE_DEL_FILE = rm -rf
QMAKE_DISTCLEAN += ../local

mac {
    PKG_CONFIG_PATH += /opt/homebrew/opt/qat/lib/pkgconfig
    PKG_CONFIG_PATH += /opt/homebrew/lib/pkgconfig
    PKG_CONFIG_PATH += /opt/homebrew/opt/gsl/lib/pkgconfig
    PKG_CONFIG_PATH += /opt/homebrew/share/pkgconfig
    PKG_CONFIG_PATH += /opt/homebrew/opt/open-mpi/lib/pkgconfig

    PKG_CONFIG = /opt/homebrew/bin/pkg-config
}

CONFIG += link_pkgconfig
PKGCONFIG += QatPlotWidgets ompi