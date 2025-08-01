# Makefile para Simulador de Procesos
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
	@mkdir -p $(OBJ_DIR) $(BIN_DIR) $(INCLUDE_DIR)

# Compilar ejecutable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@
	@echo "✅ Compilación exitosa: $(TARGET)"

# Compilar archivos objeto
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compilación con debug
debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: all

# Limpiar archivos generados
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "🗑️  Archivos de compilación eliminados"

# Ejecutar el programa
run: $(TARGET)
	@echo "🚀 Ejecutando simulador..."
	@cd $(SRC_DIR) && ../$(TARGET)

# Ejecutar con ejemplo por defecto
demo: $(TARGET)
	@echo "🎯 Ejecutando demo con proceso_ejemplo.txt..."
	@cd $(SRC_DIR) && echo "1" | echo "proceso_ejemplo.txt" | ../$(TARGET)

# Compilar y ejecutar en un solo comando
go: clean all run

# Mostrar ayuda
help:
	@echo "📋 Comandos disponibles:"
	@echo "  make          - Compilar el proyecto"
	@echo "  make run      - Compilar y ejecutar"
	@echo "  make go       - Limpiar, compilar y ejecutar"
	@echo "  make demo     - Ejecutar con archivo de ejemplo"
	@echo "  make clean    - Limpiar archivos compilados"
	@echo "  make debug    - Compilar con información de debug"
	@echo "  make help     - Mostrar esta ayuda"

# Evitar conflictos con archivos del mismo nombre
.PHONY: all clean run demo go help debug directories