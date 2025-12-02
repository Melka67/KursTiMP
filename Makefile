CXX = g++
CXXFLAGS = -std=c++11 -Wall
LDFLAGS = -lssl -lcrypto
TARGET = server
SRC_DIR = src
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET) vcalc.log

run: all
	./$(TARGET)

.PHONY: all clean run