TARGET = tinygl
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

DESTDIR = $$OUT_PWD/../

HEADERS += \
    src/axis.h \
    src/bufferobject.h \
    src/cube.h \
    src/framebufferobject.h \
    src/grid.h \
    src/light.h \
    src/mesh.h \
    src/quad.h \
    src/shader.h \
    src/singleton.h \
    src/sphere.h \
    src/tglconfig.h \
    src/tinygl.h

SOURCES += \
    src/axis.cpp \
    src/bufferobject.cpp \
    src/cube.cpp \
    src/framebufferobject.cpp \
    src/grid.cpp \
    src/light.cpp \
    src/mesh.cpp \
    src/quad.cpp \
    src/shader.cpp \
    src/sphere.cpp \
    src/tinygl.cpp
