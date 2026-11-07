
TEMPLATE = app
TARGET = symplectic
DESTDIR = .

DEPENDPATH += .
INCLUDEPATH += . ..

mac {
    CONFIG -= app_bundle
}

CONFIG += qt release c++17
CONFIG += link_pkgconfig

# Input source files
SOURCES += test.cpp

HEADERS += symplectic.cpp

# macOS
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
