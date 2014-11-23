TARGET = contourtree
TEMPLATE = lib
CONFIG += static c++11

INCLUDEPATH += ../Kernel/src
INCLUDEPATH += ../TinyGL/src
INCLUDEPATH += ../../libtourtre/include

DEPENDPATH += ../Kernel/src
DEPENDPATH += ../TinyGL/src
DEPENDPATH += ../../libtourtre/include

DESTDIR = $$OUT_PWD/../

win32 {
    CONFIG(release, debug|release) {
      QMAKE_CXXFLAGS += /O2
    }

    CONFIG(debug, debug|release) {
      QMAKE_CXXFLAGS += /Od
    }

    INCLUDEPATH += ../../glew/include
    INCLUDEPATH += ../../glm
    INCLUDEPATH += ../../tinyxml
    INCLUDEPATH += ../../tbb/include

    DEPENDPATH += ../../glew/include
    DEPENDPATH += ../../glm
    DEPENDPATH += ../../tinyxml
    DEPENDPATH += ../../tbb/include
}

unix {
    QMAKE_CXXFLAGS += -MMD

    CONFIG(release, debug|release) {
      QMAKE_CXXFLAGS += -g0 -O2
    }

    CONFIG(debug, debug|release) {
      QMAKE_CXXFLAGS += -g3 -O0 -pg
    }
}

HEADERS += \
    src/alphamanager.h \
    src/ctfunc.h \
    src/featureset.h \
    src/simplification.h \
    src/topanalyzer.h \
    src/topdata.h \
    src/topmesh.h \
    src/topsearch.hpp

SOURCES += \
    src/alphamanager.cpp \
    src/ctfunc.cpp \
    src/simplification.cpp \
    src/topanalyzer.cpp \
    src/topdata.cpp \
    src/topmesh.cpp
