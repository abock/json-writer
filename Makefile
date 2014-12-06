SOURCES = json-writer.cpp json-writer-test.cpp
HEADERS = json-writer.h
OBJECTS = $(SOURCES:.cpp=.o)
CXXFLAGS = -Wall -g

.PHONY: all clean

all: json-writer-test

clean:
	rm -rf *.o json-writer-test

json-writer-test: $(OBJECTS) $(HEADERS)
	$(CXX) $(OBJECTS) -o $@
