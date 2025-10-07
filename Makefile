# Makefile for KVStore snapshot tests

CXX = g++
CXXFLAGS = -std=c++17 -O2 -pthread
TARGET = snapshot_tests

# List of source files
SRCS = main.cpp snapshot_consistency.cpp snapshot_isolation.cpp snapshot_stress.cpp FairShareMutex.cpp

# List of object files
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJS) $(TARGET)
