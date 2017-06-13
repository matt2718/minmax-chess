CXX=g++
CXXFLAGS=-c
LDFLAGS=
SOURCES=main.cpp chessboard.cpp minimax.cpp
OBJECTS=$(SOURCES:.cpp=.o)

.PHONY: all
all: chess

.PHONY: debug
debug: chess
debug: CXXFLAGS += -g -O0

chess: $(OBJECTS)
	$(CXX) $(OBJECTS) -o chess $(LDFLAGS) 

.cpp.o:
	$(CXX) $(CXXFLAGS) $< -o $@
