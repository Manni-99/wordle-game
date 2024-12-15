# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Werror -pedantic-errors

# Project name and source files
TARGET = wordle
SOURCES = main.cc Wordle.cc
HEADERS = Wordle.h
OBJECTS = $(SOURCES:.cpp=.o)

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile source files into object files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJECTS) $(TARGET)

# Add dependencies
depend: $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -MM $^ > .depend

-include .depend
