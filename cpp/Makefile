
##
# Engender
#
# @file
# @version 0.1

CXX = g++
CXXFLAGS = -O2 -g -Wall -fmessage-length=0 -std=c++23

.PHONY: all clean

objects = analyse.o corpus.o generate.o layouts.o main.o weights.o

all: engender

engender: $(objects)
	$(CXX) $(CXXFLAGS) -o engender $(objects)

analyse: analyse.cpp analyse.h corpus.h
	$(CXX) $(CXXFLAGS) -c analyse.cpp

corpus: corpus.cpp corpus.h
	$(CXX) $(CXXFLAGS) -c corpus.cpp

generate: generate.cpp generate.h
	$(CXX) $(CXXFLAGS) -c generate.cpp

layouts: layouts.cpp layouts.h
	$(CXX) $(CXXFLAGS) -c layout.cpp

main: main.cpp main.h analyse.h corpus.h layouts.h
	$(CXX) $(CXXFLAGS) -c main.cpp

weights: weights.cpp weights.h
	$(CXX) $(CXXFLAGS) -c weights.cpp

clean:
	rm -f $(objects) engender

# end
