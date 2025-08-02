

/**
 * @file main.cpp
 * @brief Simulador de procesos con planificación Round Robin.
 *
 * Este archivo contiene la función principal del simulador de procesos. Permite cargar procesos
 * desde un archivo .txt o manualmente desde la consola, mostrando los procesos cargados y
 * ejecutando una simulación de planificación Round Robin.
 *
 * Características principales:
 * - Carga de procesos desde archivo o consola.
 * - Muestra los procesos cargados.
 * - Simulación de ejecución de procesos usando el algoritmo Round Robin.
 * - Cada proceso se ejecuta por su quantum definido, incrementando su contador de programa (PC).
 * - Si el proceso termina (PC >= CX), su estado cambia a "Terminado".
 * - Si se agota el quantum y el proceso no termina, vuelve a la cola en estado "Listo".
 * - La simulación finaliza cuando todos los procesos están en estado "Terminado".
 * - Al finalizar, muestra el estado final de todos los procesos.
 *
 *
 * Notas:
 * - El quantum puede ser configurado por proceso, pero en los ejemplos se usa el mismo valor para todos.
 * - El simulador está diseñado para ser extendido con más lógica en el futuro.
 */
#include "proceso.h"
#include "cargador.h" 
#include <iostream>
#include <cstring>  


int main() {
    std::cout << "=== SIMULADOR DE PROCESOS ===" << std::endl;
    
    CargadorProcesos cargador;
    int opcion;
    
    std::cout << "\n¿Cómo desea cargar los procesos?" << std::endl;
    std::cout << "1. Desde archivo .txt" << std::endl;
    std::cout << "2. Desde consola (manual)" << std::endl;
    std::cout << "Opción: ";
    std::cin >> opcion;
    
    switch (opcion) {
        case 1: {
            std::string nombreArchivo;
            std::cout << "Ingrese el nombre del archivo: ";
            std::cin >> nombreArchivo;
            
            if (cargador.cargarDesdeArchivo(nombreArchivo)) {
                std::cout << "Procesos cargados exitosamente desde archivo." << std::endl;
            } else {
                std::cout << "Fallando carga desde archivo. Usando modo manual." << std::endl;
                cargador.cargarDesdeConsola();
            }
            break;
        }
        case 2:
            cargador.cargarDesdeConsola();
            break;
        default:
            std::cout << "Opción no válida. Usando modo manual." << std::endl;
            cargador.cargarDesdeConsola();
    }
    
    // Mostrar procesos cargados
    cargador.mostrarProcesos();
    
    // Inicializar simulación
    auto& procesos = cargador.getProcesos();
    int tiempo = 0;
    int procesoActual = 0;
    
    std::cout << "\n=== INICIANDO SIMULACIÓN (Round Robin) ===" << std::endl;
    
    // Bucle principal de simulación
    while (true) {
        // Verificar si todos los procesos están terminados
        bool todosTerminados = true;
        for (const auto& p : procesos) {
            if (strcmp(p.estado, "Terminado") != 0) {
                todosTerminados = false;
                break;
            }
        }
        
        if (todosTerminados) {
            std::cout << "\n=== SIMULACIÓN COMPLETADA ===" << std::endl;
            break;
        }
        
        // Buscar el siguiente proceso listo
        bool procesoEncontrado = false;
        int intentos = 0;
        
        while (!procesoEncontrado && intentos < (int)procesos.size()) {
            if (strcmp(procesos[procesoActual].estado, "Listo") == 0) {
                procesoEncontrado = true;
            } else {
                procesoActual = (procesoActual + 1) % procesos.size();
                intentos++;
            }
        }
        
        if (!procesoEncontrado) {
            std::cout << "No hay procesos listos para ejecutar." << std::endl;
            break;
        }
        
        // Cambiar estado a ejecutando
        cambiarEstado(&procesos[procesoActual], "Ejecutando");
        std::cout << "\n--- Proceso PID " << procesos[procesoActual].pid << " inicia ejecución ---" << std::endl;
        
        // Ejecutar el proceso por su quantum completo
        int quantumRestante = procesos[procesoActual].quantum;
        
        while (quantumRestante > 0 && strcmp(procesos[procesoActual].estado, "Ejecutando") == 0) {
            // Ejecutar 1 ciclo
            std::cout << "Tiempo " << tiempo << ": Ejecutando proceso PID " 
                      << procesos[procesoActual].pid << " (PC=" << procesos[procesoActual].pc 
                      << ", Quantum restante: " << quantumRestante << ")" << std::endl;
            
            // Simular ejecución (incrementar PC)
            procesos[procesoActual].pc++;
            quantumRestante--;
            tiempo++;
            
            // Verificar si el proceso terminó (cuando PC >= CX)
            if (procesos[procesoActual].pc >= procesos[procesoActual].cx) {
                cambiarEstado(&procesos[procesoActual], "Terminado");
                std::cout << "  -> Proceso PID " << procesos[procesoActual].pid 
                          << " TERMINADO (PC=" << procesos[procesoActual].pc << ")" << std::endl;
                break;
            }
        }
        
        // Si el proceso no terminó pero se agotó el quantum
        if (strcmp(procesos[procesoActual].estado, "Ejecutando") == 0) {
            cambiarEstado(&procesos[procesoActual], "Listo");
            std::cout << "  -> Quantum agotado, proceso PID " << procesos[procesoActual].pid 
                      << " vuelve a cola (PC=" << procesos[procesoActual].pc << ")" << std::endl;
        }
        
        // Cambiar al siguiente proceso
        procesoActual = (procesoActual + 1) % procesos.size();
    }
    
    // Mostrar estado final
    std::cout << "\n=== ESTADO FINAL ===" << std::endl;
    cargador.mostrarProcesos();
}




/* para que funcione en caso tal 
cd ~/simulador-procesos/src
g++ -Wall -Wextra -g main.cpp proceso.cpp cargador.cpp -o simulador
./simulador       */

/*mejorar makefile, cambiar quantum en mi .txt siempre todo igual = 3, ¿tiene que ser igual siempre en todos los procesos?, pero eso dizque que dependen de mi rafaga de cpu que eso lo debo preguntar*/