ifneq ($(MAKECMDGOALS), release)
    CONFIG := debug
    CXXFLAGS += -g3 -O0 -D_DEBUG
else
    CONFIG := release
    CXXFLAGS += -g0 -O2 -DNDEBUG
endif

JUNK_DIR := bin/$(CONFIG)
CXXFLAGS += -std=c++0x -MMD -Iinclude

MODULES := kernel tinygl topanalyzer #visualizer

kernel_PATH := Kernel
tinygl_PATH := TinyGL
topanalyzer_PATH := TopAnalyzer
visualizer_PATH := Visualizer

include $(foreach i,$(MODULES),$($(i)_PATH)/rules.mk)

.DEFAULT_GOAL :=
.PHONY : all
all : $(foreach i,$(MODULES),$($(i)_TARGET))

.PHONY : release
release : all

.PHONY : clean
clean :
	rm -rf bin
