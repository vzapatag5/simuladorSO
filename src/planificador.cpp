
/**
 * @file planificador.cpp
 * @brief Implementación de un planificador Round-Robin para procesos simulados.
 *
 * Este archivo contiene la implementación de la clase PlanificadorRoundRobin y funciones auxiliares
 * para simular la planificación de procesos utilizando el algoritmo Round-Robin. Los procesos
 * ejecutan instrucciones simples tipo ensamblador (ADD, SUB, MUL, INC, JMP, NOP) sobre registros
 * AX, BX y CX, con soporte para quantum configurable y visualización detallada del estado de la
 * cola de listos y de los procesos.
 *
 * Funciones auxiliares:
 * - printSeparator: Imprime una línea separadora en consola.
 * - trim: Elimina espacios en blanco al inicio y final de una cadena.
 * - upper: Convierte una cadena a mayúsculas.
 * - regRef: Devuelve una referencia al registro solicitado de un proceso.
 * - isReg: Verifica si una cadena corresponde a un registro válido.
 * - parseIntSafe: Intenta convertir una cadena a entero de forma segura.
 * - printReadyQueue: Imprime el contenido de la cola de listos.
 *
 * Método principal:
 * - PlanificadorRoundRobin::schedule: Ejecuta la planificación Round-Robin sobre un conjunto de procesos
 *   y sus instrucciones, mostrando el avance, cambios de contexto y estados finales.
 *
 * Detalles de la simulación:
 * - Cada proceso tiene un quantum configurable.
 * - Se simulan instrucciones básicas de tipo ensamblador.
 * - Se muestra el estado de los procesos y la cola de listos en cada paso.
 * - Se gestionan los cambios de contexto y la finalización de procesos.
 *
 * @author
 * @date
 */
#include "planificador.h"
#include <iostream>
#include <cstring>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <queue>

static void printSeparator(char ch='=', int n=60) {
    for (int i=0;i<n;++i) std::cout << ch;
    std::cout << "\n";
}

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

static int& regRef(Proceso& p, const std::string& r) {
    if (r == "AX") return p.ax;
    if (r == "BX") return p.bx;
    return p.cx; // CX por defecto
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

// Imprime el contenido de la cola de listos
static void printReadyQueue(const std::queue<int>& q, const std::vector<Proceso>& procesos) {
    std::queue<int> tmp = q;
    std::cout << "  COLA (front→back): ";
    bool first = true;
    while (!tmp.empty()) {
        int idx = tmp.front(); tmp.pop();
        std::cout << (first ? "" : " -> ") << "P" << procesos[idx].pid;
        first = false;
    }
    if (first) std::cout << "(vacía)";
    std::cout << "\n";
}

void PlanificadorRoundRobin::schedule(std::vector<Proceso>& procesos,
                                      const std::vector<std::vector<std::string>>& instrucciones) {
    const int n = static_cast<int>(procesos.size());
    int finished = 0;

    // 1) Cabecera + listado inicial (incluye AX,BX,CX)
    printSeparator('=');
    std::cout << "PROCESOS E INSTRUCCIONES CARGADAS\n";
    printSeparator('-');
    for (int i = 0; i < n; ++i) {
        std::cout << "  Proceso " << procesos[i].pid
                  << " | PC=" << procesos[i].pc
                  << " | Q=" << procesos[i].quantum
                  << " | AX=" << procesos[i].ax
                  << " | BX=" << procesos[i].bx
                  << " | CX=" << procesos[i].cx
                  << " | Instrucciones=" << instrucciones[i].size()
                  << " | Estado=" << procesos[i].estado << "\n";
    }
    printSeparator('=');
    std::cout << "\n";

    // 2) Construir COLA DE LISTOS (FIFO real)
    std::queue<int> ready;
    for (int i = 0; i < n; ++i) {
        if (procesos[i].pc < static_cast<int>(instrucciones[i].size())) {
            std::strcpy(procesos[i].estado, "Listo");
            ready.push(i);
        } else {
            std::strcpy(procesos[i].estado, "Terminado");
            finished++;
        }
    }

    std::cout << "COLA INICIAL:\n";
    printReadyQueue(ready, procesos);
    std::cout << "\n";

    if (ready.empty()) {
        printSeparator('=');
        std::cout << "PLANIFICACION COMPLETA\n";
        printSeparator('=');
        return;
    }

    printSeparator('=');
    std::cout << "INICIO DE PLANIFICACION (Round-Robin)\n";
    printSeparator('=');

    // 3) Bucle principal: siempre tomar del FRONT y reencolar al BACK si no terminó
    while (!ready.empty()) {
        int idx = ready.front();
        ready.pop();
        Proceso &p = procesos[idx];

        printSeparator('.');
        std::cout << "EJECUTANDO PROCESO " << p.pid << " | "
                  << "PC=" << p.pc
                  << " | AX=" << p.ax
                  << " | BX=" << p.bx
                  << " | CX=" << p.cx
                  << " | Q=" << p.quantum << "\n";
        printSeparator('.');

        int q_rest = p.quantum;

        while (q_rest > 0 && p.pc < static_cast<int>(instrucciones[idx].size())) {
            int instrIndex = p.pc;               // PC antes de ejecutar
            std::string raw = trim(instrucciones[idx][p.pc]);
            std::string s = upper(raw);

            // Parse op y operandos
            std::string op, rest, a1, a2;
            {
                std::istringstream iss(s);
                iss >> op;
                std::getline(iss, rest);
                rest = trim(rest);
                size_t cpos = rest.find(',');
                if (cpos != std::string::npos) {
                    a1 = trim(rest.substr(0,cpos));
                    a2 = trim(rest.substr(cpos+1));
                } else {
                    a1 = trim(rest);
                }
            }

            if (op=="ADD" || op=="SUB" || op=="MUL") {
                if (!isReg(a1)) { std::cerr << "  ! Destino invalido en: " << raw << "\n"; break; }
                int valSrc=0; bool srcIsImm=false;
                if (isReg(a2)) { valSrc = regRef(p,a2); }
                else {
                    if (!parseIntSafe(a2, valSrc)) { std::cerr << "  ! Operando invalido en: " << raw << "\n"; break; }
                    srcIsImm = true;
                }
                int before = regRef(p,a1);
                if (op=="ADD") regRef(p,a1) = before + valSrc;
                else if (op=="SUB") regRef(p,a1) = before - valSrc;
                else regRef(p,a1) = before * valSrc;

                int after = regRef(p,a1);
                p.pc++;        // avanzamos a la siguiente instrucción
                q_rest--;      // consumimos quantum

                std::cout << "  - Instr[" << instrIndex << "] " << raw << "  | "
                          << a1 << ": " << before
                          << (op=="ADD"? " + " : (op=="SUB"? " - " : " * "))
                          << (srcIsImm? std::to_string(valSrc) : a2+"("+std::to_string(valSrc)+")")
                          << " = " << after
                          << "  | Quantum restante: " << q_rest << "\n";
            }
            else if (op=="INC") {
                if (!isReg(a1)) { std::cerr << "  ! Registro invalido en: " << raw << "\n"; break; }
                int before = regRef(p,a1);
                regRef(p,a1) = before + 1;

                p.pc++;
                q_rest--;

                std::cout << "  - Instr[" << instrIndex << "] " << raw << "  | "
                          << a1 << ": " << before << " -> " << regRef(p,a1)
                          << "  | Quantum restante: " << q_rest << "\n";
            }
            else if (op=="JMP") {
                int tgt=0;
                if (!parseIntSafe(a1, tgt)) {
                    std::cerr << "  ! Destino JMP invalido en: " << raw << "\n"; break;
                }
                int oldPC = p.pc;
                if (tgt < 0 || tgt >= static_cast<int>(instrucciones[idx].size())) {
                    std::cerr << "  ! JMP fuera de rango (" << tgt << "), proceso termina.\n";
                    p.pc = static_cast<int>(instrucciones[idx].size()); // fuerza fin
                } else {
                    p.pc = tgt;
                }

                q_rest--;

                std::cout << "  - Instr[" << instrIndex << "] " << raw
                          << "  | PC: " << oldPC << " -> " << p.pc
                          << "  | Quantum restante: " << q_rest << "\n";
            }
            else if (op=="NOP") {
                p.pc++;
                q_rest--;
                std::cout << "  - Instr[" << instrIndex << "] " << raw
                          << "  | (sin efecto)"
                          << "  | Quantum restante: " << q_rest << "\n";
            }
            else {
            
                p.pc++;
                q_rest--;
                std::cout << "  - Instr[" << instrIndex << "] " << raw
                          << "  | (desconocida→NOP)"
                          << "  | Quantum restante: " << q_rest << "\n";
            }
        }

        // 4) Post-quantum: decidir si termina o vuelve a la cola
        if (p.pc >= static_cast<int>(instrucciones[idx].size())) {
            std::strcpy(p.estado, "Terminado");
            finished++;
            std::cout << "  ✔ Proceso " << p.pid << " ha TERMINADO todas sus instrucciones.\n";
        } else {
            std::strcpy(p.estado, "Listo");
            std::cout << "  ⏳ Quantum AGOTADO para Proceso " << p.pid << ". Se ENCOLA al final.\n";
            ready.push(idx);
            printReadyQueue(ready, procesos);
        }
        std::cout << "\n";

        // 5) Cambio de contexto (si hay alguien listo)
        if (!ready.empty()) {
            Proceso &np = procesos[ready.front()];
            printSeparator('-');
            std::cout << "CAMBIO DE CONTEXTO\n";
            printSeparator('-');
            std::cout << "  Guardando [P" << p.pid << "]: PC=" << p.pc
                      << ", AX=" << p.ax << ", BX=" << p.bx << ", CX=" << p.cx
                      << ", Estado=" << p.estado << "\n";
            std::cout << "  Cargando  [P" << np.pid << "]: PC=" << np.pc
                      << ", AX=" << np.ax << ", BX=" << np.bx << ", CX=" << np.cx
                      << ", Estado=" << np.estado << "\n\n";
        }
    }

    // 6) Cierre
    printSeparator('=');
    std::cout << "PLANIFICACION COMPLETA\n";
    printSeparator('=');

    std::cout << "\nESTADOS FINALES DE PROCESOS\n";
    printSeparator('-');
    for (const auto& p : procesos) {
        std::cout << "  Proceso " << p.pid
                  << " | PC=" << p.pc
                  << " | AX=" << p.ax
                  << " | BX=" << p.bx
                  << " | CX=" << p.cx
                  << " | Estado=" << p.estado << "\n";
    }
    printSeparator('-');
}
