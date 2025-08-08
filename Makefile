SRC_DIR=src
INCLUDE_DIR=include
OBJ_DIR=obj
BIN_DIR=bin
CXX=g++
CXXFLAGS=-std=c++17 -I$(INCLUDE_DIR) -Wall -Wextra

SOURCES=$(wildcard $(SRC_DIR)/*.cpp)
OBJECTS=$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

all: directories $(BIN_DIR)/simulador

directories:
	mkdir -p $(OBJ_DIR) $(BIN_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(INCLUDE_DIR)/*.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN_DIR)/simulador: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

run: all
	./$(BIN_DIR)/simulador -f procesos.txt

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean run directories
