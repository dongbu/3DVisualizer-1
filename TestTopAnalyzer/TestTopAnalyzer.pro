TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../Kernel/src
INCLUDEPATH += ../TinyGL/src
INCLUDEPATH += ../TopAnalyzer/src
INCLUDEPATH += ../Renderer/src
INCLUDEPATH += ../../libtourtre/include
INCLUDEPATH += ../../glfw/include

DESTDIR = $$OUT_PWD/../

LIBS += -L$$OUT_PWD/.. -lrenderer
LIBS += -L$$OUT_PWD/.. -lcontourtree
LIBS += -L$$OUT_PWD/.. -lviskernel
LIBS += -L$$OUT_PWD/.. -ltinygl

win32 {
    INCLUDEPATH += $$PWD/../../glew/include
    INCLUDEPATH += $$PWD/../../glm

    LIBS += -lopengl32

    CONFIG(release, debug|release) {
        QMAKE_CXXFLAGS += /O2
        LIBS += -L$$PWD/../../glew/lib -lglew32
        LIBS += -L$$PWD/../../libtourtre/Release -ltourtre
        LIBS += -L$$PWD/../../tinyxml/vc12/ReleasetinyxmlSTL -ltinyxmlSTL
        LIBS += -L$$PWD/../../tbb/lib/ia32/vc12 -ltbb
        LIBS += -L$$PWD/../../tbb/lib/ia32/vc12 -ltbbmalloc
    }

    CONFIG(debug, debug|release) {
        QMAKE_CXXFLAGS += /Od
        LIBS += -L$$PWD/../../glew/lib -lglew32d
        LIBS += -L$$PWD/../../libtourtre/Debug -ltourtre
        LIBS += -L$$PWD/../../tinyxml/vc12/DebugtinyxmlSTL -ltinyxmlSTL
        LIBS += -L$$PWD/../../tbb/lib/ia32/vc12 -ltbb_debug
        LIBS += -L$$PWD/../../tbb/lib/ia32/vc12 -ltbbmalloc_debug
    }
}

unix {
    QMAKE_CXXFLAGS += -MMD

    CONFIG(release, debug|release) {
      QMAKE_CXXFLAGS += -g0 -O2
    }

    CONFIG(debug, debug|release) {
      QMAKE_CXXFLAGS += -g3 -O0 -pg
    }

    LIBS += -lglfw3 -lGL -lGLU -lX11 -lXxf86vm -lXinerama -lXcursor -lXrandr -lpthread -lXi -ltinyxml -ltourtre -lGLEW  -ltbb -ltbbmalloc -lm
}

SOURCES += main.cpp

