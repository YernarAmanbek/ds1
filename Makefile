CXX ?= g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic
TARGET := browser_engine
SOURCES := main.cpp ArrayStack.cpp BrowserHistory.cpp ImageQueue.cpp
HEADERS := ArrayStack.h BrowserHistory.h ImageQueue.h

.PHONY: all run test clean
all: $(TARGET)
$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)
run: $(TARGET)
	./$(TARGET)
stack_tests: tests/stack_tests.cpp ArrayStack.cpp BrowserHistory.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -I. tests/stack_tests.cpp ArrayStack.cpp BrowserHistory.cpp -o stack_tests
queue_tests: tests/queue_tests.cpp ImageQueue.cpp ImageQueue.h
	$(CXX) $(CXXFLAGS) -I. tests/queue_tests.cpp ImageQueue.cpp -o queue_tests
test: stack_tests queue_tests
	./stack_tests
	./queue_tests
clean:
	rm -f $(TARGET) stack_tests queue_tests
