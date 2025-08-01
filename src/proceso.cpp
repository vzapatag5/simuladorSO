#include "proceso.h"
#include <iostream>
#include <cstring>

void inicializarProceso(Proceso* p, int pid, int ax, int bx, int cx, int quantum) {
    p->pid = pid;
    p->pc = 0;
    p->ax = ax;
    p->bx = bx;
    p->cx = cx;
    p->quantum = quantum;
    strcpy(p->estado, "Listo");
}

void mostrarProceso(const Proceso* p) {
    std::cout << "PID: " << p->pid 
              << " | PC: " << p->pc
              << " | AX: " << p->ax
              << " | BX: " << p->bx
              << " | CX: " << p->cx
              << " | Quantum: " << p->quantum
              << " | Estado: " << p->estado << std::endl;
}

void cambiarEstado(Proceso* p, const char* nuevoEstado) {
    strcpy(p->estado, nuevoEstado);
}