TARGET = vis
TEMPLATE = app
QT += core gui opengl widgets
CONFIG += qt c++11 app_bundle

win32 {
    INCLUDEPATH += $$PWD/../../glew/include
    INCLUDEPATH += $$PWD/../../glm

    LIBS += -L$$PWD/../../tbb/lib/ia32/vc12 -ltbb
    LIBS += -L$$PWD/../../tbb/lib/ia32/vc12 -ltbbmalloc
    LIBS += -lopengl32

    debug {
        LIBS += -L$$PWD/../../glew/lib -lglew32d
        LIBS += -L$$PWD/../../libtourtre/Debug -ltourtre
        LIBS += -L$$PWD/../../tinyxml/vc12/DebugtinyxmlSTL -ltinyxmlSTL
    }
    release {
        LIBS += -L$$PWD/../../glew/lib -lglew32
        LIBS += -L$$PWD/../../libtourtre/Release -ltourtre
        LIBS += -L$$PWD/../../tinyxml/vc12/ReleasetinyxmlSTL -ltinyxmlSTL
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

    LIBS += -lGLEW -lGL
    LIBS += -ltbb -ltbbmalloc
    LIBS += -ltourtre
    LIBS += -ltinyxml
    LIBS += -lm
}

INCLUDEPATH += ../Kernel/src
INCLUDEPATH += ../TinyGL/src
INCLUDEPATH += ../TopAnalyzer/src
INCLUDEPATH += ../Renderer/src
INCLUDEPATH += ../../libtourtre/include

DESTDIR = $$OUT_PWD/../

LIBS += -L$$OUT_PWD/.. -lviskernel
LIBS += -L$$OUT_PWD/.. -ltinygl
LIBS += -L$$OUT_PWD/.. -lcontourtree
LIBS += -L$$OUT_PWD/.. -lrenderer

#LIBS += -liup -liupgl -lXmu -lXt -lX11
#LIBS += -lgtk-x11-2.0 -lgdk-x11-2.0 -lgdk_pixbuf-2.0 -lpango-1.0 -lgobject-2.0
#LIBS += -lglib-2.0

HEADERS += \
    src/config.h \
    src/viswindow.h

SOURCES += \
    src/main.cpp \
    src/viswindow.cpp

FORMS +=
