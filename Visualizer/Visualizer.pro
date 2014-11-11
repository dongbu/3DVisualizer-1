TARGET = vis
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11 -MMD

CONFIG(release, debug|release) {
  QMAKE_CXXFLAGS += -g0 -O2
}

CONFIG(debug, debug|release) {
  QMAKE_CXXFLAGS += -g3 -O0 -pg
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

LIBS +=-ltbb -ltbbmalloc
LIBS += -lGLEW -lGL
LIBS += -ltourtre -ltinyxml
LIBS += -liup -liupgl -lXmu -lXt -lX11
LIBS += -lm
LIBS += -lgtk-x11-2.0 -lgdk-x11-2.0 -lgdk_pixbuf-2.0 -lpango-1.0 -lgobject-2.0
LIBS += -lglib-2.0

HEADERS += \
    src/config.h

SOURCES += \
    src/main.cpp
