# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -Iinclude

# Directories
SRC_DIR = src
OBJ_DIR = build
BIN_DIR = bin
BIN = $(BIN_DIR)/main

# Find all .cpp files (from src/ or root), supports single or multiple files
SRCS = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard *.cpp)
OBJS = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(notdir $(SRCS)))

# Default target
all: $(BIN)

# Link all object files into the final binary in bin/ directory
$(BIN): $(OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

# Compile .cpp files into .o object files
$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create necessary directories
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Run the program from bin/ directory
run: all
	./$(BIN)

# Clean all build artifacts
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all run clean
