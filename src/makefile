Makefile para Simulador de Procesos

# Compilador y flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -Iinclude
DEBUG_FLAGS = -g -DDEBUG

# Directorios
SRC_DIR = src
INCLUDE_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

# Archivos
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
TARGET = $(BIN_DIR)/simulador

# Regla principal
all: directories $(TARGET)

# Crear directorios si no existen
directories:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

# Compilar ejecutable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@
	@echo "Compilación exitosa: $(TARGET)"

# Compilar archivos objeto
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compilación con debug
debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: all

# Limpiar archivos generados
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "Archivos de compilación eliminados"

# Ejecutar el programa
run: $(TARGET)
	./$(TARGET)