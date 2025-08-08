/**
 * @file instrucciones.cpp
 * @brief Implementación del ejecutor de instrucciones siguiendo la sintaxis del proyecto.
 */

#include "instrucciones.h"
#include <iostream>
#include <sstream>
#include <algorithm>

bool EjecutorInstrucciones::ejecutarInstruccion(Proceso* proceso, const std::string& instruccion) {
    std::istringstream iss(instruccion);
    std::string comando;
    iss >> comando;
    
    // Convertir a mayúsculas
    std::transform(comando.begin(), comando.end(), comando.begin(), ::toupper);
    
    std::cout << "  Ejecutando: " << instruccion << std::endl;
    
    if (comando == "ADD") {
        std::string resto;
        std::getline(iss, resto);
        return ejecutarADD(proceso, resto);
    } else if (comando == "SUB") {
        std::string resto;
        std::getline(iss, resto);
        return ejecutarSUB(proceso, resto);
    } else if (comando == "MUL") {
        std::string resto;
        std::getline(iss, resto);
        return ejecutarMUL(proceso, resto);
    } else if (comando == "INC") {
        std::string resto;
        std::getline(iss, resto);
        return ejecutarINC(proceso, resto);
    } else if (comando == "JMP") {
        std::string resto;
        std::getline(iss, resto);
        return ejecutarJMP(proceso, resto);
    } else if (comando == "NOP") {
        return ejecutarNOP(proceso);
    } else {
        std::cout << "  Error: Instrucción desconocida: " << comando << std::endl;
        return true; // Continuar ejecución
    }
}

bool EjecutorInstrucciones::ejecutarADD(Proceso* proceso, const std::string& params) {
    std::istringstream iss(params);
    std::string reg1, coma, operando2;
    iss >> reg1 >> coma >> operando2;
    
    // Remover coma si está pegada al registro
    if (reg1.back() == ',') {
        reg1.pop_back();
    }
    
    int* registro = obtenerRegistro(proceso, reg1);
    if (!registro) return true;
    
    int valor2 = obtenerValor(proceso, operando2);
    int valorAnterior = *registro;
    *registro += valor2;
    
    std::cout << "    " << reg1 << ": " << valorAnterior << " + " << valor2 << " = " << *registro << std::endl;
    return true;
}

bool EjecutorInstrucciones::ejecutarSUB(Proceso* proceso, const std::string& params) {
    std::istringstream iss(params);
    std::string reg1, coma, operando2;
    iss >> reg1 >> coma >> operando2;
    
    if (reg1.back() == ',') {
        reg1.pop_back();
    }
    
    int* registro = obtenerRegistro(proceso, reg1);
    if (!registro) return true;
    
    int valor2 = obtenerValor(proceso, operando2);
    int valorAnterior = *registro;
    *registro -= valor2;
    
    std::cout << "    " << reg1 << ": " << valorAnterior << " - " << valor2 << " = " << *registro << std::endl;
    return true;
}

bool EjecutorInstrucciones::ejecutarMUL(Proceso* proceso, const std::string& params) {
    std::istringstream iss(params);
    std::string reg1, coma, operando2;
    iss >> reg1 >> coma >> operando2;
    
    if (reg1.back() == ',') {
        reg1.pop_back();
    }
    
    int* registro = obtenerRegistro(proceso, reg1);
    if (!registro) return true;
    
    int valor2 = obtenerValor(proceso, operando2);
    int valorAnterior = *registro;
    *registro *= valor2;
    
    std::cout << "    " << reg1 << ": " << valorAnterior << " * " << valor2 << " = " << *registro << std::endl;
    return true;
}

bool EjecutorInstrucciones::ejecutarINC(Proceso* proceso, const std::string& params) {
    std::istringstream iss(params);
    std::string reg;
    iss >> reg;
    
    int* registro = obtenerRegistro(proceso, reg);
    if (!registro) return true;
    
    int valorAnterior = *registro;
    (*registro)++;
    
    std::cout << "    " << reg << ": " << valorAnterior << " + 1 = " << *registro << std::endl;
    return true;
}

bool EjecutorInstrucciones::ejecutarJMP(Proceso* proceso, const std::string& params) {
    std::istringstream iss(params);
    int nuevaLinea;
    iss >> nuevaLinea;
    
    std::cout << "    Saltando de línea " << proceso->pc << " a línea " << nuevaLinea << std::endl;
    proceso->pc = nuevaLinea - 1; // -1 porque se incrementará después
    return true;
}

bool EjecutorInstrucciones::ejecutarNOP(Proceso* proceso) {
    std::cout << "    NOP - No operation" << std::endl;
    return true;
}

int* EjecutorInstrucciones::obtenerRegistro(Proceso* proceso, const std::string& nombre) {
    std::string reg = nombre;
    std::transform(reg.begin(), reg.end(), reg.begin(), ::toupper);
    
    if (reg == "AX") return &proceso->ax;
    else if (reg == "BX") return &proceso->bx;
    else if (reg == "CX") return &proceso->cx;
    else {
        std::cout << "    Error: Registro desconocido: " << nombre << std::endl;
        return nullptr;
    }
}

bool EjecutorInstrucciones::esNumero(const std::string& str) {
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}

int EjecutorInstrucciones::obtenerValor(Proceso* proceso, const std::string& operando) {
    if (esNumero(operando)) {
        return std::stoi(operando);
    } else {
        int* registro = obtenerRegistro(proceso, operando);
        return registro ? *registro : 0;
    }
}
