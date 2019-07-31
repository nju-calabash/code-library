SRC_CODES = $(shell find src -mindepth 2 -maxdepth 2 -name '*.cpp' -type f)
SRC_TARGETS = $(SRC_CODES:src/%.cpp=build/%.exec)

CXX ?= g++
CXXFLAGS = -O2 -Wall -std=c++14 -ggdb
CXXFLAGS += -fsanitize=undefined,address
GCOV ?= gcov

.DEFAULT_GOAL = test

.PHONY : test clean

build/%.exec: src/%.cpp
	@echo + [CXX] $@
	@mkdir -p $(dir $@)
	@cd $(dir $@) && $(CXX) $(CXXFLAGS) -ftest-coverage -fprofile-arcs ../../$< -o $(notdir $@)

build/%.gcda : build/%.exec src/%.cpp
	@echo + [GCOV] $@
	@cd $(dir $@) && ./$(notdir $<)
	@cd $(dir $@) && $(GCOV) -n -o $(notdir $@) $(notdir $(word 2,$^)) > /dev/null

test : $(SRC_CODES:src/%.cpp=build/%.gcda)

clean :
	rm -rf build

.DELETE_ON_ERROR:
