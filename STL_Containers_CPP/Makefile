CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -Werror -g -I./containers -I/usr/include/gtest
LDFLAGS := -pthread -lgtest -lgtest_main

SRC_DIR := containers
TEST_DIR := tests
BUILD_DIR := build

SOURCES := $(shell find $(SRC_DIR) -name '*.cpp')
TEST_SOURCES := $(shell find $(TEST_DIR) -name '*.cpp')

OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))
TEST_OBJECTS := $(patsubst $(TEST_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(TEST_SOURCES))

TEST_TARGET := test_runner

.PHONY: all test clean

all: test

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(OBJECTS) $(TEST_OBJECTS)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

rebuild: clean all

clean:
	rm -rf $(BUILD_DIR) $(TEST_TARGET)
