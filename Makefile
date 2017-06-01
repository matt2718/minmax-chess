CXX=g++
CXXFLAGS=-c -g
LDFLAGS=
SOURCES=main.cpp chessboard.cpp
OBJECTS=$(SOURCES:.cpp=.o)

all: chess

chess: $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o chess

.cpp.o:
	$(CXX) $(CXXFLAGS) $< -o $@
