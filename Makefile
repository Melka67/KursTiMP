CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
LDFLAGS = -lssl -lcrypto

SRCDIR = src
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = server

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

install: $(TARGET)
	sudo mkdir -p /etc /var/log
	sudo cp $(TARGET) /usr/local/bin/
	sudo touch /etc/vcalc.conf
	sudo touch /var/log/vcalc.log
	sudo chmod 666 /etc/vcalc.conf /var/log/vcalc.log

.PHONY: clean install