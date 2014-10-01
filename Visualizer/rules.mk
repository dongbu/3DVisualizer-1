MODULE := visualizer

visualizer_TARGET := vis
visualizer_CXXFLAGS := -ITinyGL/src -ITopAnalyzer/src -IKernel/src
visualizer_LIBS := -lGLEW -lglfw -lGL -lm -ltourtre -ltbb -ltbbmalloc -ltinyxml
visualizer_LOCALLIBS := $(kernel_TARGET) $(tinygl_TARGET) $(topanalyzer_TARGET)

include common-rules.mk
