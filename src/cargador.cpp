
/**
 * @file cargador.cpp
 * @brief Implementación del cargador de procesos para el simulador de sistema operativo.
 *
 * Este archivo contiene la lógica para cargar procesos desde un archivo o desde la consola,
 * validando la sintaxis de cada línea según la gramática definida:
 *   PID: <num> [, AX=<num>][, BX=<num>][, CX=<num>] , Quantum=<num>
 * 
 * Funcionalidades principales:
 * - Validación y parseo de líneas de procesos usando expresiones regulares.
 * - Carga de procesos desde archivo o entrada estándar.
 * - Manejo de errores de sintaxis y reporte de líneas inválidas.
 * - Almacenamiento de los procesos cargados en un vector.
 *
 * Clases y funciones:
 * - CargadorProcesos: Clase principal para la carga y gestión de procesos.
 *   - bool cargarProcesos(const std::string& filename): Carga procesos desde un archivo.
 *   - bool cargarProcesosDesdeConsola(): Carga procesos desde la entrada estándar.
 *   - const std::vector<Proceso>& getProcesos() const: Acceso a los procesos cargados.
 * - parseLineaProceso: Función auxiliar para parsear y validar una línea de proceso.
 *
 * @author
 * @date
 */
#include "cargador.h"
#include <fstream>
#include <iostream>
#include <regex>
#include <cstring> 

// Gramática: PID: <num> [, AX=<num>][, BX=<num>][, CX=<num>] , Quantum=<num>
static const std::regex kProcRegex(
    R"(^(?:\s*)PID:\s*(\d+)((?:,\s*(AX|BX|CX)=\d+)*)\s*,\s*Quantum=(\d+)\s*$)"
);

static void parseLineaProceso(const std::string& line, std::vector<Proceso>& out, int lineNum=-1) {
    std::smatch m;
    if (!std::regex_match(line, m, kProcRegex)) {
        std::cerr << "Error de sintaxis" << (lineNum>=0? " en línea "+std::to_string(lineNum):"")
                  << ": " << line << "\n";
        throw std::runtime_error("Linea invalida");
    }

    Proceso p{};
    p.pid = std::stoi(m[1].str());
    p.pc = 0;
    p.ax = p.bx = p.cx = 0;
    p.quantum = std::stoi(m[4].str());

    const std::string regs = m[2].str();
    std::regex rgxRegs("(AX|BX|CX)=(\\d+)");
    for (auto it = std::sregex_iterator(regs.begin(), regs.end(), rgxRegs);
         it != std::sregex_iterator(); ++it) {
        std::string r = (*it)[1].str();
        int v = std::stoi((*it)[2].str());
        if (r == "AX") p.ax = v;
        else if (r == "BX") p.bx = v;
        else if (r == "CX") p.cx = v;
    }

    std::strcpy(p.estado, "Listo");
    out.push_back(p);
}

bool CargadorProcesos::cargarProcesos(const std::string& filename) {
    procesos_.clear();
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error: no se pudo abrir " << filename << "\n";
        return false;
    }
    std::string line;
    int lineNum = 0;
    try {
        while (std::getline(file, line)) {
            ++lineNum;
            if (line.empty()) continue;
            parseLineaProceso(line, procesos_, lineNum);
        }
    } catch (...) {
        procesos_.clear();
        return false;
    }
    return !procesos_.empty();
}

bool CargadorProcesos::cargarProcesosDesdeConsola() {
    procesos_.clear();
    std::cout << "Ingrese procesos (una línea por proceso). Ejemplos:\n";
    std::cout << "  PID: 1, AX=5, BX=3, Quantum=2\n";
    std::cout << "  PID: 2, CX=7, Quantum=3\n";
    std::cout << "Finalice con una línea vacía.\n\n";

    std::string line;
    int lineNum = 0;
    try {
        while (true) {
            std::cout << "> ";
            std::getline(std::cin, line);
            if (line.empty()) break;
            ++lineNum;
            parseLineaProceso(line, procesos_, lineNum);
        }
    } catch (...) {
        procesos_.clear();
        return false;
    }
    if (procesos_.empty()) {
        std::cerr << "No se ingresó ningún proceso.\n";
        return false;
    }
    return true;
}

const std::vector<Proceso>& CargadorProcesos::getProcesos() const {
    return procesos_;
}

