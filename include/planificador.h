
/**
 * @file planificador.h
 * @brief Declaración de la clase PlanificadorRoundRobin para la planificación de procesos usando el algoritmo Round Robin.
 *
 * Este archivo contiene la definición de la clase PlanificadorRoundRobin, que implementa el método de planificación
 * de procesos Round Robin. Utiliza referencias para modificar el estado de los procesos fuera del método schedule.
 */

 /**
    * @class PlanificadorRoundRobin
    * @brief Clase que implementa el algoritmo de planificación Round Robin.
    *
    * La clase proporciona un método para planificar una lista de procesos según el algoritmo Round Robin,
    * recibiendo las instrucciones asociadas a cada proceso.
    */

/**
 * @brief Planifica la ejecución de los procesos utilizando el algoritmo Round Robin.
 * 
 * @param procesos Vector de procesos a planificar. Se pasa por referencia para reflejar los cambios en el estado de los procesos.
 * @param instrucciones Vector de vectores de cadenas que representan las instrucciones asociadas a cada proceso.
 */
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



