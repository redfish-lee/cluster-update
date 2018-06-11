CXX = g++
CXXFLAGS = -std=c++11
LFLAGS = -lpthread -pthread

TARGETS = inotify.cc

.PHONY: all
all: 
	$(CXX) $(CXXFLAGS) $(LFLAGS) $(TARGETS)

.PHONY: clean
clean: 
	rm -f *.o *.out
