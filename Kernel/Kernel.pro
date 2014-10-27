TARGET = viskernel
TEMPLATE = lib
CONFIG += staticlib

QMAKE_CXXFLAGS += -std=c++11 -MMD

CONFIG(release, debug|release) {
  QMAKE_CXXFLAGS += -g0 -O2
}

CONFIG(debug, debug|release) {
  QMAKE_CXXFLAGS += -g3 -O0 -pg
}

HEADERS += \
    src/datahandler.h \
    src/dataset.h \
    src/datasetmanager.h \
    src/datatransfer.h \
    src/knlconfig.h \
    src/logger.h \
    src/perlin.h \
    src/singleton.h \
    src/tfmanager.h \
    src/tfunction.h

SOURCES += \
    src/datahandler.cpp \
    src/dataset.cpp \
    src/datasetmanager.cpp \
    src/datatransfer.cpp \
    src/logger.cpp \
    src/perlin.cpp \
    src/tfmanager.cpp \
    src/tfunction.cpp

