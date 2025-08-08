#ifndef PLANIFICADOR_H
#define PLANIFICADOR_H

#include <vector>
#include <string>
#include "proceso.h"
#include "instrucciones.h"

class PlanificadorRoundRobin {
public:
    // Por referencia para que fuera de schedule se vea el estado final
    void schedule(std::vector<Proceso>& procesos,
                  const std::vector<std::vector<std::string>>& instrucciones);
};

#endif // PLANIFICADOR_H



