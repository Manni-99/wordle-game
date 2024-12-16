# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Werror -pedantic-errors

# Project name and source files
TARGET = wordle
TEST_TARGET = wordle_tests
SOURCES = Wordle.cc main.cc  # main.cc is for the main program only
HEADERS = Wordle.h
TEST_SOURCES = WordleUnitTests.cc  # Test source file
OBJECTS = $(SOURCES:.cc=.o)  # Object files for the main program
TEST_OBJECTS = $(TEST_SOURCES:.cc=.o) Wordle.o  # Object files for the test program (Wordle.o needed)

# Default target
all: $(TARGET) $(TEST_TARGET)

# Build the main executable
$(TARGET): $(OBJECTS)
	@echo "Building main executable..."
	$(CXX) $(CXXFLAGS) -o $@ $^

# Build the test executable
$(TEST_TARGET): $(TEST_OBJECTS)  # Only link test-related objects + Wordle.o
	@echo "Building test executable..."
	$(CXX) $(CXXFLAGS) -o $@ $^ -lgtest -lgtest_main -pthread

# Compile source files into object files
%.o: %.cc $(HEADERS)
	@echo "Compiling $< ..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files, but don't delete the main or test executables
clean:
	rm -f $(OBJECTS) $(TEST_OBJECTS) $(TARGET) $(TEST_TARGET)

# Add dependencies
depend: $(SOURCES) $(HEADERS) $(TEST_SOURCES)
	$(CXX) $(CXXFLAGS) -MM $^ > .depend

-include .depend