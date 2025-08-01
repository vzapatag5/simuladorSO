/**
 * @file proceso.h
 * @brief Define la estructura y funciones para el manejo de procesos simulados.
 *
 * Este archivo contiene la definición de la estructura Proceso, que representa
 * un proceso simulado con registros, contador de programa, quantum y estado.
 * También declara funciones para inicializar, mostrar y cambiar el estado de un proceso.
 *
 * Estructura:
 * - Proceso: Contiene información relevante de un proceso simulado.
 *
 * Funciones:
 * - inicializarProceso: Inicializa los valores de un proceso.
 * - mostrarProceso: Muestra la información de un proceso.
 * - cambiarEstado: Cambia el estado de un proceso.
 */

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