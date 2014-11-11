MODULE := visualizer

visualizer_TARGET := vis
visualizer_CXXFLAGS := -ITinyGL/src -ITopAnalyzer/src -IRenderer/src -IKernel/src
visualizer_LIBS := -lGLEW -lglfw -lGL -ltourtre -ltbb -ltbbmalloc -ltinyxml -liup -lXmu -lXt -lX11 -lm -lgtk-x11-2.0 -lgdk-x11-2.0 -lgdk_pixbuf-2.0 -lpango-1.0 -lgobject-2.0 -lglib-2.0
visualizer_LOCALLIBS := $(kernel_TARGET) $(tinygl_TARGET) $(topanalyzer_TARGET) $(renderer_TARGET)

include common-rules.mk
