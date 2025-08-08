#ifndef PLANIFICADOR_H
#define PLANIFICADOR_H

#include "proceso.h"
#include <queue>
#include <vector>

class PlanificadorRoundRobin {
private:
    std::queue<int> colaListos;  // Cola de índices de procesos listos
    std::vector<Proceso>* procesos;
    int tiempo;
    
public:
    PlanificadorRoundRobin(std::vector<Proceso>* listaProcesos);
    
    void inicializarCola();
    bool hayProcesosListos();
    int obtenerSiguienteProceso();
    void agregarProcesoACola(int indiceProceso);
    void eliminarProcesoTerminado(int indiceProceso);
    void ejecutarSimulacion();
    
private:
    void mostrarEstadoColas();
};

#endif // PLANIFICADOR_H
