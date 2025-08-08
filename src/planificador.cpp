#include "planificador.h"
#include <iostream>
#include <cstring>
#include <stack>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <sstream>

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
    // default CX
    return p.cx;
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

void PlanificadorRoundRobin::schedule(std::vector<Proceso>& procesos,
                                      const std::vector<std::vector<std::string>>& instrucciones) {
    const int n = static_cast<int>(procesos.size());
    int finished = 0;
    std::stack<int> pila; // PIDs

    // Sección inicial (incluye AX,BX,CX)
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

    int idx = 0;
    bool banner = true;

    while (finished < n) {
        // Buscar un proceso con instrucciones pendientes
        int countTerm = 0;
        while (procesos[idx].pc >= static_cast<int>(instrucciones[idx].size())) {
            idx = (idx + 1) % n;
            if (++countTerm > n) break; // todos terminaron
        }
        if (countTerm > n || finished == n) break;

        Proceso &p = procesos[idx];

        if (banner) {
            printSeparator('=');
            std::cout << "INICIO DE PLANIFICACION (Round-Robin)\n";
            printSeparator('=');
            std::cout << "Cargando Proceso " << p.pid
                      << " (PC=" << p.pc << ", AX=" << p.ax
                      << ", BX=" << p.bx << ", CX=" << p.cx
                      << ", Estado=" << p.estado << ")\n\n";
            banner = false;
        }

        printSeparator('.');
        std::cout << "EJECUTANDO PROCESO " << p.pid << "\n";
        printSeparator('.');

        int q_rest = p.quantum;

        while (q_rest > 0 && p.pc < static_cast<int>(instrucciones[idx].size())) {
            std::string raw = trim(instrucciones[idx][p.pc]);
            std::string s = upper(raw);

            // Parse op y operandos
            // Formatos:
            // ADD R, X   | SUB R, X   | MUL R, X   (R in AX/BX/CX; X en AX/BX/CX o entero)
            // INC R
            // JMP N
            // NOP
            std::string op, a1, a2;
            {
                std::istringstream iss(s);
                iss >> op;
                std::string rest; std::getline(iss, rest);
                rest = trim(rest);
                // si hay coma, separar
                size_t cpos = rest.find(',');
                if (cpos != std::string::npos) {
                    a1 = trim(rest.substr(0,cpos));
                    a2 = trim(rest.substr(cpos+1));
                } else {
                    a1 = trim(rest);
                }
            }

            // Ejecutar y mostrar efecto
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
                else regRef(p,a1) = before * valSrc; // MUL

                int after = regRef(p,a1);
                std::cout << "  - " << raw << "  | "
                          << a1 << ": " << before
                          << (op=="ADD"? " + " : (op=="SUB"? " - " : " * "))
                          << (srcIsImm? std::to_string(valSrc) : a2+"("+std::to_string(valSrc)+")")
                          << " = " << after
                          << "  | Quantum restante: " << (q_rest-1) << "\n";

                // avanzar PC y descontar quantum
                p.pc++;
                q_rest--;
            }
            else if (op=="INC") {
                if (!isReg(a1)) { std::cerr << "  ! Registro invalido en: " << raw << "\n"; break; }
                int before = regRef(p,a1);
                regRef(p,a1) = before + 1;
                std::cout << "  - " << raw << "  | "
                          << a1 << ": " << before << " -> " << regRef(p,a1)
                          << "  | Quantum restante: " << (q_rest-1) << "\n";
                p.pc++;
                q_rest--;
            }
            else if (op=="JMP") {
                int tgt=0;
                if (!parseIntSafe(a1, tgt)) {
                    std::cerr << "  ! Destino JMP invalido en: " << raw << "\n"; break;
                }
                int oldPC = p.pc;
                if (tgt < 0 || tgt >= static_cast<int>(instrucciones[idx].size())) {
                    std::cerr << "  ! JMP fuera de rango (" << tgt << "), proceso termina.\n";
                    p.pc = static_cast<int>(instrucciones[idx].size());
                } else {
                    p.pc = tgt;
                }
                std::cout << "  - " << raw << "  | PC: " << oldPC << " -> " << p.pc
                          << "  | Quantum restante: " << (q_rest-1) << "\n";
                q_rest--;
            }
            else if (op=="NOP") {
                std::cout << "  - " << raw << "  | (sin efecto)"
                          << "  | Quantum restante: " << (q_rest-1) << "\n";
                p.pc++;
                q_rest--;
            }
            else {
                std::cerr << "  ! Instruccion desconocida: " << raw << "\n";
                // la tratamos como NOP para no trabarnos
                p.pc++;
                q_rest--;
            }
        }

        // Estado tras ejecutar su porción
        if (p.pc >= static_cast<int>(instrucciones[idx].size())) {
            std::strcpy(p.estado, "Terminado");
            finished++;
            std::cout << "  ✔ Proceso " << p.pid << " ha TERMINADO todas sus instrucciones.\n";
        } else if (q_rest == 0) {
            std::strcpy(p.estado, "Listo");
            std::cout << "  ⏳ Quantum AGOTADO para Proceso " << p.pid << ". Se apila.\n";
            pila.push(p.pid);

            std::cout << "  Pila (top→bottom): ";
            std::stack<int> tmp = pila;
            bool first = true;
            while (!tmp.empty()) {
                std::cout << (first ? "" : " -> ") << tmp.top();
                tmp.pop();
                first = false;
            }
            std::cout << "\n";
        }
        std::cout << "\n";

        // Elegir siguiente listo/no terminado
        int next = (idx + 1) % n;
        int vueltas = 0;
        while (vueltas < n && procesos[next].pc >= static_cast<int>(instrucciones[next].size())) {
            next = (next + 1) % n;
            ++vueltas;
        }

        if (finished < n) {
            printSeparator('-');
            std::cout << "CAMBIO DE CONTEXTO\n";
            printSeparator('-');
            std::cout << "  Guardando [P" << p.pid << "]: PC=" << p.pc
                      << ", AX=" << p.ax << ", BX=" << p.bx << ", CX=" << p.cx
                      << ", Estado=" << p.estado << "\n";
            auto &np = procesos[next];
            std::cout << "  Cargando  [P" << np.pid << "]: PC=" << np.pc
                      << ", AX=" << np.ax << ", BX=" << np.bx << ", CX=" << np.cx
                      << ", Estado=" << np.estado << "\n\n";
        }

        idx = next;
    }

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


