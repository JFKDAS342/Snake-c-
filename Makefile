# Компилятор и флаги
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
TARGET = snake_game
SOURCES = Snake.cpp
LIBS = -lncurses

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES) $(LIBS)


debug: CXXFLAGS += -g -DDEBUG
debug: $(TARGET)


release: CXXFLAGS += -O3 -DNDEBUG
release: $(TARGET)


clean:
	rm -f $(TARGET) *.o


run: $(TARGET)
	./$(TARGET)


install: $(TARGET)
	sudo cp $(TARGET) /usr/local/bin/

.PHONY: clean run debug release install help