/**
 * @file proceso.cpp
 * @brief Implementación de funciones para la gestión de procesos.
 *
 * Este archivo contiene la implementación de funciones para inicializar,
 * mostrar y cambiar el estado de un proceso en el simulador de procesos.
 */

 /**
    * @brief Inicializa los valores de un proceso.
    * 
    * Asigna los valores iniciales de los registros y el quantum, y establece el estado en "Listo".
    * 
    * @param p Puntero al proceso a inicializar.
    * @param pid Identificador del proceso.
    * @param ax Valor inicial del registro AX.
    * @param bx Valor inicial del registro BX.
    * @param cx Valor inicial del registro CX.
    * @param quantum Valor inicial del quantum del proceso.
    */
 
 /**
    * @brief Muestra la información de un proceso por consola.
    * 
    * Imprime los valores de los registros, el quantum y el estado del proceso.
    * 
    * @param p Puntero constante al proceso a mostrar.
    */
 
 /**
    * @brief Cambia el estado de un proceso.
    * 
    * Modifica el campo de estado del proceso al nuevo valor proporcionado.
    * 
    * @param p Puntero al proceso cuyo estado se va a cambiar.
    * @param nuevoEstado Cadena de caracteres con el nuevo estado.
    */
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