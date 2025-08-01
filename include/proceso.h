#ifndef PROCESO_H
#define PROCESO_H

#include <string>

typedef struct {
    int pid;                    // ID del proceso
    int pc;                     // Program Counter simulado
    int ax, bx, cx;            // Registros simulados
    int quantum;               // Quantum asignado
    char estado[10];           // "Listo", "Ejecutando", "Terminado"
} Proceso;

// Funciones para manejo de procesos
void inicializarProceso(Proceso* p, int pid, int ax, int bx, int cx, int quantum);
void mostrarProceso(const Proceso* p);
void cambiarEstado(Proceso* p, const char* nuevoEstado);

#endif // PROCESO_H