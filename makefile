
##
 # startup code provided by Paul Miller for COSC1114 - Operating Systems
 # Principles
 ##

# provide make targets here to build the two programs 

CXX := g++
CXXFLAGS := -Wall -Werror -std=c++20 -g -O

.default: all

all: fifo sjf rr

fifo: fifo.o pcb.o loader.o simulator.o
	$(CXX) $(CXXFLAGS) -o $@ $^

sjf: sjf.o pcb.o loader.o simulator.o 
	$(CXX) $(CXXFLAGS) -o $@ $^

rr: rr.o pcb.o loader.o simulator.o
	$(CXX) $(CXXFLAGS) -o $@ $^

run_all: run_all.o pcb.o loader.o simulator.o
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $^

clean:
	rm -rf fifo sjf rr run_all *.o *.dSYM

