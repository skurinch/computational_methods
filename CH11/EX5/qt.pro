TEMPLATE = app
TARGET = top
DESTDIR = .
DEPENDPATH += .
INCLUDEPATH += . ..

mac {
    CONFIG -= app_bundle
}

CONFIG += qt release c++17

# Input source files
SOURCES += top.cpp \
           plot.cpp

HEADERS += plot.h

# Clean
QMAKE_DEL_FILE = rm -rf
QMAKE_DISTCLEAN += ../local

mac {
    PKG_CONFIG_PATH += /opt/homebrew/opt/qat/lib/pkgconfig
    PKG_CONFIG_PATH += /opt/homebrew/lib/pkgconfig
    PKG_CONFIG_PATH += /opt/homebrew/opt/gsl/lib/pkgconfig
    PKG_CONFIG_PATH += /opt/homebrew/opt/open-mpi/lib/pkgconfig
    PKG_CONFIG_PATH += /opt/homebrew/share/pkgconfig

    PKG_CONFIG = /opt/homebrew/bin/pkg-config

    # macOS deployment target
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 14.4
}

CONFIG += link_pkgconfig

PKGCONFIG += QatPlotWidgets ompi