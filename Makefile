CXX := g++
CXXFLAGS := -std=c++2a

SOURCE := src/main.cc
TARGET := bin/lmf

all:
	$(CXX) -o $(TARGET) $(SOURCE) $(CXXFLAGS)

.PHONY: all
