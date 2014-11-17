TARGET = renderer
TEMPLATE = lib
CONFIG += staticlib c++11

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

INCLUDEPATH += ../Kernel/src
INCLUDEPATH += ../TinyGL/src
INCLUDEPATH += ../TopAnalyzer/src
INCLUDEPATH += ../../libtourtre/include

DESTDIR = $$OUT_PWD/../

HEADERS += \
    src/arcball.h \
    src/config.h \
    src/glslrenderer.h \
    src/renderer.h \
    src/renderermanager.h

SOURCES += \
    src/arcball.cpp \
    src/glslrenderer.cpp \
    src/renderermanager.cpp

OTHER_FILES += \
    ../Resources/shaders/FPass.vs \
    ../Resources/shaders/SPass.vs \
    ../Resources/shaders/FPass.fs \
    ../Resources/shaders/SPass.fs \
    ../Resources/shaders/SPassMultiOp.fs

shader.files = $$OTHER_FILES
shader.path = $$DESTDIR/shaders

INSTALLS += shader
