# Compiler settings
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -I./src

# Project layout
SRC_DIR := src
TEST_DIR := test
BUILD_DIR := build

# Application source files
APP_SOURCES := $(SRC_DIR)/enigma.cpp \
               $(SRC_DIR)/parse_args.cpp \
               $(SRC_DIR)/plugboard.cpp \
			   $(SRC_DIR)/utils.cpp 
APP := $(BUILD_DIR)/enigma

# Every .cpp file in test/ becomes a separately runnable test executable
TEST_SOURCES := $(wildcard $(TEST_DIR)/*.cpp)
TEST_BINS := $(patsubst $(TEST_DIR)/%.cpp,$(BUILD_DIR)/%,$(TEST_SOURCES))

# Sources shared by tests
TEST_SUPPORT_SOURCES := $(SRC_DIR)/parse_args.cpp \
                        $(SRC_DIR)/plugboard.cpp \
						$(SRC_DIR)/utils.cpp

.PHONY: all run test clean

all: $(APP)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(APP): $(APP_SOURCES) | $(BUILD_DIR)
	    $(CXX) $(CXXFLAGS) -o $@ $(APP_SOURCES)

# Builds a test executable using its test source plus the reusable program code.
$(BUILD_DIR)/%: $(TEST_DIR)/%.cpp $(TEST_SUPPORT_SOURCES) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $< $(TEST_SUPPORT_SOURCES)

# Build and run the main program. Pass program options after: make run ARGS='...'
run: $(APP)
	   $(APP) $(ARGS)

# Build all tests and run every test executable.
test: $(TEST_BINS)
	@for test_binary in $(TEST_BINS); do \
		echo "Running $$test_binary"; \
		./$$test_binary || exit $$?; \
	done

clean:
	rm -rf $(BUILD_DIR)
