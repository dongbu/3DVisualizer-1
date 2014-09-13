MODULE := visualizer

visualizer_TARGET := visualizer
visualizer_CXXFLAGS := -ITinyGL/src -ITopAnalyzer/src
visualizer_LIBS := -lglut -lGLEW -lGL
visualizer_LOCALLIBS := $(tinygl_TARGET) $(topanalyzer_TARGET)

include common-rules.mk
