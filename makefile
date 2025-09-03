# # Compiler and flags
# CXX = g++
# CXXFLAGS = -g

# # Target executable
# TARGET = main

# # Source files
# SRCS = shell.cpp

# # Object files
# OBJS = $(SRCS:.cpp=.o)

# # Header files
# HEADERS = 

# # Default target
# all: $(TARGET)

# # Rule to build the target executable
# $(TARGET): $(OBJS)
# 	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) -lreadline

# # Rule to compile source files into object files
# %.o: %.cpp $(HEADERS)
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# # Clean up object files and the executable
# clean:
# 	rm -f $(OBJS) $(TARGET)

# # Run the program
# run: $(TARGET)
# 	./$(TARGET)

# # Phony targets
# .PHONY: all clean

# Compiler and flags
CXX = g++
CXXFLAGS = -g -Wall

# Target executable
TARGET = shell

# Source and object files
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

# Header files
HEADERS = $(wildcard *.h)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) -lreadline

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
