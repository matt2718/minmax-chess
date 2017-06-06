CXX=g++
CXXFLAGS=-c
LDFLAGS=
SOURCES=main.cpp chessboard.cpp
OBJECTS=$(SOURCES:.cpp=.o)

.PHONY: all
all: chess

.PHONY: debug
debug: chess
debug: CXXFLAGS += -g

chess: $(OBJECTS)
	$(CXX) $(OBJECTS) -o chess $(LDFLAGS) 

.cpp.o:
	$(CXX) $(CXXFLAGS) $< -o $@
