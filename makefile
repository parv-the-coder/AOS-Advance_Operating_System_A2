CXX = g++
CXXFLAGS = -g -Wall

TARGET = shell

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

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
