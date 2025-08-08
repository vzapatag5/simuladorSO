
/**
 * @file proceso.h
 * @brief Define la estructura Proceso que representa un proceso en el sistema.
 */

/**
 * @struct Proceso
 * @brief Estructura para representar un proceso y su estado en el sistema.
 *
 * Miembros:
 * - pid:     Identificador único del proceso.
 * - pc:      Contador de programa (dirección actual de instrucción).
 * - ax:      Registro de propósito general AX.
 * - bx:      Registro de propósito general BX.
 * - cx:      Registro de propósito general CX.
 * - quantum: Porción de tiempo asignada al proceso para su ejecución.
 * - estado:  Estado actual del proceso (por ejemplo, "ejecutando", "esperando").
 */
#ifndef PROCESO_H
#define PROCESO_H

struct Proceso {
    int pid;
    int pc;
    int ax;
    int bx;
    int cx;
    int quantum;
    char estado[10];
};

#endif // PROCESO_H
