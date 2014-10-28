TARGET = contourtree
TEMPLATE = lib
CONFIG += staticlib

QMAKE_CXXFLAGS += -std=c++11 -MMD

CONFIG(release, debug|release) {
  QMAKE_CXXFLAGS += -g0 -O2
}

CONFIG(debug, debug|release) {
  QMAKE_CXXFLAGS += -g3 -O0 -pg
}

INCLUDEPATH += ../Kernel/src
INCLUDEPATH += ../TinyGL/src
INCLUDEPATH += ../../libtourtre/include

DESTDIR = $$OUT_PWD/../

HEADERS += \
    src/alphamanager.h \
    src/ctfunc.h \
    src/featureset.h \
    src/simplification.h \
    src/topanalyzer.h \
    src/topdata.h \
    src/topmesh.h

SOURCES += \
    src/alphamanager.cpp \
    src/ctfunc.cpp \
    src/simplification.cpp \
    src/topanalyzer.cpp \
    src/topdata.cpp \
    src/topmesh.cpp
