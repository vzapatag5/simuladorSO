/**
 * @file planificador.cpp
 * @brief Implementación del planificador Round Robin con colas.
 */

#include "planificador.h"
#include "instrucciones.h"
#include <iostream>
#include <cstring>

PlanificadorRoundRobin::PlanificadorRoundRobin(std::vector<Proceso>* listaProcesos) 
    : procesos(listaProcesos), tiempo(0) {
}

void PlanificadorRoundRobin::inicializarCola() {
    // Agregar todos los procesos en estado "Listo" a la cola
    for (size_t i = 0; i < procesos->size(); i++) {
        if (strcmp((*procesos)[i].estado, "Listo") == 0) {
            colaListos.push(i);
        }
    }
    
    std::cout << "Cola inicializada con " << colaListos.size() << " procesos." << std::endl;
}

bool PlanificadorRoundRobin::hayProcesosListos() {
    return !colaListos.empty();
}

int PlanificadorRoundRobin::obtenerSiguienteProceso() {
    if (colaListos.empty()) return -1;
    
    int indiceProceso = colaListos.front();
    colaListos.pop();
    return indiceProceso;
}

void PlanificadorRoundRobin::agregarProcesoACola(int indiceProceso) {
    if (indiceProceso >= 0 && indiceProceso < (int)procesos->size()) {
        colaListos.push(indiceProceso);
    }
}

void PlanificadorRoundRobin::eliminarProcesoTerminado(int indiceProceso) {
    // Los procesos terminados no se vuelven a agregar a la cola
    std::cout << "Proceso PID " << (*procesos)[indiceProceso].pid 
              << " eliminado de la cola (TERMINADO)" << std::endl;
}

void PlanificadorRoundRobin::mostrarEstadoColas() {
    std::cout << "Estado de la cola: " << colaListos.size() << " procesos listos" << std::endl;
}

void PlanificadorRoundRobin::ejecutarSimulacion() {
    std::cout << "\n=== INICIANDO SIMULACIÓN (Round Robin con Colas) ===" << std::endl;
    
    inicializarCola();
    int procesoAnterior = -1;
    
    while (hayProcesosListos()) {
        int procesoActual = obtenerSiguienteProceso();
        if (procesoActual == -1) break;
        
        Proceso* proceso = &(*procesos)[procesoActual];
        
        // Mostrar cambio de contexto
        if (procesoAnterior != -1 && procesoAnterior != procesoActual) {
            mostrarCambioContexto(&(*procesos)[procesoAnterior], proceso);
        }
        
        // Cambiar estado a ejecutando
        cambiarEstado(proceso, "Ejecutando");
        std::cout << "\n--- Proceso PID " << proceso->pid << " inicia ejecución ---" << std::endl;
        
        // Ejecutar el proceso por su quantum
        int quantumRestante = proceso->quantum;
        bool procesoTerminado = false;
        
        while (quantumRestante > 0 && !procesoTerminado) {
            std::cout << "Tiempo " << tiempo << ": Ejecutando proceso PID " 
                      << proceso->pid << " (PC=" << proceso->pc 
                      << ", Quantum restante: " << quantumRestante << ")" << std::endl;
            
            // Verificar si hay más instrucciones
            if (proceso->pc >= (int)proceso->instrucciones.size()) {
                cambiarEstado(proceso, "Terminado");
                std::cout << "  -> Proceso PID " << proceso->pid 
                          << " TERMINADO (sin más instrucciones)" << std::endl;
                procesoTerminado = true;
                eliminarProcesoTerminado(procesoActual);
                break;
            }
            
            // Ejecutar instrucción actual
            std::string instruccionActual = proceso->instrucciones[proceso->pc];
            EjecutorInstrucciones::ejecutarInstruccion(proceso, instruccionActual);
            
            // Incrementar PC y decrementar quantum
            proceso->pc++;
            quantumRestante--;
            tiempo++;
            
            // Mostrar estado actual de registros
            std::cout << "    Estado: AX=" << proceso->ax << ", BX=" << proceso->bx 
                      << ", CX=" << proceso->cx << std::endl;
        }
        
        // Si el proceso no terminó pero se agotó el quantum
        if (!procesoTerminado && strcmp(proceso->estado, "Ejecutando") == 0) {
            cambiarEstado(proceso, "Listo");
            std::cout << "  -> Quantum agotado, proceso PID " << proceso->pid 
                      << " vuelve a cola (PC=" << proceso->pc << ")" << std::endl;
            agregarProcesoACola(procesoActual);
        }
        
        procesoAnterior = procesoActual;
        mostrarEstadoColas();
    }
    
    std::cout << "\n=== SIMULACIÓN COMPLETADA ===" << std::endl;
}
