MODULE := renderer

renderer_TARGET := librender.a
renderer_CXXFLAGS := -ITinyGL/src -ITopAnalyzer/src -IKernel/src

include common-rules.mk
