
/**
 * @file instrucciones.cpp
 * @brief Implementación de la clase CargadorInstrucciones y funciones auxiliares para la validación y carga de instrucciones de procesos.
 *
 * Este archivo contiene funciones para:
 * - Eliminar espacios en blanco y convertir cadenas a mayúsculas.
 * - Validar instrucciones de un lenguaje ensamblador simple (NOP, INC, JMP, ADD, SUB, MUL).
 * - Cargar instrucciones desde archivos o desde la consola para una lista de procesos.
 *
 * Funciones auxiliares:
 * - trim: Elimina espacios en blanco al inicio y final de una cadena.
 * - upper: Convierte una cadena a mayúsculas.
 * - isReg: Verifica si una cadena representa un registro válido (AX, BX, CX).
 * - parseIntSafe: Intenta convertir una cadena a entero de forma segura.
 * - validarInstruccion: Valida la sintaxis y semántica de una instrucción individual.
 *
 * Métodos de CargadorInstrucciones:
 * - cargarInstrucciones: Carga instrucciones desde archivos de texto, uno por proceso.
 * - cargarInstruccionesDesdeConsola: Permite ingresar instrucciones manualmente por consola.
 * - getInstrucciones: Devuelve las instrucciones cargadas para todos los procesos.
 *
 * Las instrucciones válidas son:
 * - NOP: Sin operandos.
 * - INC <REG>: Incrementa el registro especificado.
 * - JMP <INT>: Salta a la línea indicada.
 * - ADD|SUB|MUL <REG>, <REG|INT>: Operaciones aritméticas entre registros o enteros.
 *
 * Se reportan errores detallados en caso de instrucciones inválidas.
 */
#include "instrucciones.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <sstream>

static std::string trim(const std::string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    size_t b = s.find_last_not_of(" \t\r\n");
    if (a == std::string::npos) return "";
    return s.substr(a, b - a + 1);
}
static std::string upper(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c){ return std::toupper(c); });
    return s;
}
static bool isReg(const std::string& r) {
    return r=="AX" || r=="BX" || r=="CX";
}
static bool parseIntSafe(const std::string& s, int& out) {
    try {
        size_t idx=0; int val = std::stoi(s, &idx);
        if (idx != s.size()) return false;
        out = val; return true;
    } catch (...) { return false; }
}

static bool validarInstruccion(const std::string& original, std::string& error) {
    error.clear();
    std::string raw = trim(original);
    if (raw.empty()) { error = "Linea vacia"; return false; }

    std::string s = upper(raw);

    std::string op, rest;
    {
        std::istringstream iss(s);
        iss >> op;
        std::getline(iss, rest);
        rest = trim(rest);
    }

    if (op=="NOP") {
        // NOP no tiene operandos
        if (!rest.empty()) { error = "NOP no lleva operandos"; return false; }
        return true;
    }
    if (op=="INC") {
        // INC <REG>
        if (rest.empty()) { error = "INC requiere un registro"; return false; }
        if (!isReg(rest)) { error = "INC solo admite AX/BX/CX"; return false; }
        return true;
    }
    if (op=="JMP") {
        // JMP <INT>
        if (rest.empty()) { error = "JMP requiere destino (entero)"; return false; }
        int tgt=0;
        if (!parseIntSafe(rest, tgt)) { error = "Destino JMP debe ser entero"; return false; }
        if (tgt < 0) { error = "Destino JMP no puede ser negativo"; return false; }
        return true;
    }
    if (op=="ADD" || op=="SUB" || op=="MUL") {
        // ADD|SUB|MUL <REG>, <REG|INT>
        if (rest.empty()) { error = op + " requiere 2 operandos"; return false; }
        size_t cpos = rest.find(',');
        if (cpos == std::string::npos) { error = op + " requiere coma entre operandos"; return false; }
        std::string a1 = trim(rest.substr(0,cpos));
        std::string a2 = trim(rest.substr(cpos+1));
        if (!isReg(a1)) { error = "Destino debe ser AX/BX/CX"; return false; }
        if (!isReg(a2)) {
            int v=0;
            if (!parseIntSafe(a2, v)) { error = "Segundo operando debe ser AX/BX/CX o entero"; return false; }
        }
        return true;
    }

    error = "Instruccion no permitida: " + op;
    return false;
}

bool CargadorInstrucciones::cargarInstrucciones(const std::string& instrDir,
                                                const std::vector<Proceso>& procesos) {
    instrucciones_.clear();
    for (const auto& p : procesos) {
        std::string path = instrDir + "/" + std::to_string(p.pid) + ".txt";
        std::ifstream file(path);
        if (!file) {
            std::cerr << "Error: no se encontró " << path << "\n";
            return false;
        }
        std::vector<std::string> lista;
        std::string line;
        int lineNum = 0;
        while (std::getline(file, line)) {
            ++lineNum;
            if (trim(line).empty()) continue; // ignorar líneas vacías
            std::string err;
            if (!validarInstruccion(line, err)) {
                std::cerr << "Error en " << path << " (linea " << lineNum << "): " << err
                          << " | Texto: " << line << "\n";
                return false;
            }
            lista.push_back(line);
        }
        instrucciones_.push_back(lista);
    }
    return true;
}

bool CargadorInstrucciones::cargarInstruccionesDesdeConsola(const std::vector<Proceso>& procesos) {
    instrucciones_.clear();
    std::cout << "\nIngrese instrucciones por proceso (una por línea). "
                 "Escriba END para terminar cada proceso.\n\n";
    for (const auto& p : procesos) {
        std::cout << "[PID " << p.pid << "] Ingrese instrucciones (END para terminar):\n";
        std::vector<std::string> lista;
        std::string line;
        int lineNum = 0;
        while (true) {
            std::cout << "  > ";
            std::getline(std::cin, line);
            if (line == "END") break;
            if (trim(line).empty()) continue;
            ++lineNum;

            std::string err;
            if (!validarInstruccion(line, err)) {
                std::cerr << "  Error (linea " << lineNum << "): " << err
                          << " | Texto: " << line << "\n";
                return false;
            }
            lista.push_back(line);
        }
        instrucciones_.push_back(lista);
        std::cout << "\n";
    }
    return true;
}

const std::vector<std::vector<std::string>>&
CargadorInstrucciones::getInstrucciones() const {
    return instrucciones_;
}
