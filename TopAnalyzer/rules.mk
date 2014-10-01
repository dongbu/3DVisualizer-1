MODULE := topanalyzer

topanalyzer_TARGET := libtopanalyzer.a
topanalyzer_CXXFLAGS := -IKernel/src -I../libtourtre/include

include common-rules.mk
