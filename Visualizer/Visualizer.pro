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
INCLUDEPATH += ../../libtourtre/include


LIBS += -L$$OUT_PWD/../kernel -lviskernel
LIBS += -L$$OUT_PWD/../tinygl -ltinygl
LIBS += -L$$OUT_PWD/../topanalyzer -lcontourtree

LIBS +=-ltbb -ltbbmalloc
LIBS += -lGLEW
LIBS += -lglfw
LIBS += -lGL
LIBS += -ltourtre
LIBS += -ltinyxml
LIBS += -liup
LIBS += -lXmu
LIBS += -lXt
LIBS += -lX11
LIBS += -lm
LIBS += -lgtk-x11-2.0
LIBS += -lgdk-x11-2.0
LIBS += -lgdk_pixbuf-2.0
LIBS += -lpango-1.0
LIBS += -lgobject-2.0
LIBS += -lglib-2.0

HEADERS += \
    src/arcball.h \
    src/config.h

SOURCES += \
    src/arcball.cpp \
    src/main.cpp
