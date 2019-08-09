.DEFAULT_GOAL = tex

.PHONY : tex test clean

TEX_SRCS=$(shell find src tex -type f)

build/tex/code.tex : $(TEX_SRCS) script/*
	mkdir -p $(dir $@)
	./script/make-tex.py > build/tex/code.tex

build/tex/main.pdf : $(TEX_SRCS) script/* build/tex/code.tex
	mkdir -p $(dir $@)
	cp -r tex build/
	cd build/tex && xelatex main.tex
	cd build/tex && xelatex main.tex
    
tex : build/tex/main.pdf
	xdg-open $<

SRC_CODES = $(shell ./script/get-srclist.py)
SRC_TARGETS = $(SRC_CODES:src/%.cpp=build/%.exec)

CXX ?= g++
CXXFLAGS = -O2 -Wall -std=c++14 -ggdb
CXXFLAGS += -fsanitize=undefined,address
GCOV ?= gcov

build/%.exec: src/%.cpp
	@echo + [CXX] $@
	@mkdir -p $(dir $@)
	@cd $(dir $@) && $(CXX) $(CXXFLAGS) -ftest-coverage -fprofile-arcs ../../$< -o $(notdir $@)

build/%.gcda : build/%.exec src/%.cpp
	@echo + [GCOV] $@
	@cd $(dir $@) && ./$(notdir $<)
	@cd $(dir $@) && $(GCOV) -n -o $(notdir $@) $(notdir $(word 2,$^)) > /dev/null

test : $(SRC_CODES:src/%.cpp=build/%.gcda) build/tex/code.tex


clean :
	rm -rf build 
.DELETE_ON_ERROR:

