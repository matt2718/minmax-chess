CXX=g++
CXXFLAGS=
LDFLAGS=
LDLIBS=
SOURCES=chessboard.cpp main.cpp minimax.cpp
OBJECTS=$(SOURCES:.cpp=.o)

.PHONY: all
all: chess

.PHONY: debug
debug: chess
debug: CXXFLAGS += -g -O0

chess: $(OBJECTS)
	$(CXX) -o chess $(CXXFLAGS) $(LDFLAGS) $(OBJECTS) $(LDLIBS)

.cpp.o:
	$(CXX) -o $@ -c $(CXXFLAGS) $<
