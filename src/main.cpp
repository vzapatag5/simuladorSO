#include "proceso.h"
#include "cargador.cpp" // Incluir temporalmente hasta separar en headers
#include <iostream>
#include <cstring>  // Para strcmp

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
    
    // Aquí irá la lógica del simulador en las próximas semanas
    // REEMPLAZA la línea: std::cout << "\n[SIMULACIÓN - Por implementar en próximas semanas]" << std::endl;
// CON TODO ESTE CÓDIGO:

    // Inicializar simulación
    auto& procesos = cargador.getProcesos();
    int tiempo = 0;
    int procesoActual = 0;
    int quantumRestante = 0;
    
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
        
        while (!procesoEncontrado && intentos < procesos.size()) {
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
        
        // Inicializar quantum si es necesario
        if (quantumRestante <= 0) {
            quantumRestante = procesos[procesoActual].quantum;
            cambiarEstado(&procesos[procesoActual], "Ejecutando");
        }
        
        // Ejecutar proceso por 1 unidad de tiempo
        std::cout << "Tiempo " << tiempo << ": Ejecutando proceso PID " 
                  << procesos[procesoActual].pid << " (Quantum restante: " 
                  << quantumRestante << ")" << std::endl;
        
        // Simular ejecución (incrementar PC)
        procesos[procesoActual].pc++;
        quantumRestante--;
        tiempo++;
        
        // Verificar si el proceso terminó (ejemplo: cuando PC >= CX)
        if (procesos[procesoActual].pc >= procesos[procesoActual].cx) {
            cambiarEstado(&procesos[procesoActual], "Terminado");
            std::cout << "  -> Proceso PID " << procesos[procesoActual].pid 
                      << " TERMINADO" << std::endl;
            quantumRestante = 0;
            procesoActual = (procesoActual + 1) % procesos.size();
        }
        // Verificar si se agotó el quantum
        else if (quantumRestante <= 0) {
            cambiarEstado(&procesos[procesoActual], "Listo");
            std::cout << "  -> Quantum agotado, proceso vuelve a cola" << std::endl;
            procesoActual = (procesoActual + 1) % procesos.size();
        }
        
        // Pausa opcional para ver la simulación paso a paso
        // std::cin.get(); // Descomenta esta línea si quieres pausa manual
    }
    
    // Mostrar estado final
    std::cout << "\n=== ESTADO FINAL ===" << std::endl;
    cargador.mostrarProcesos();
    
    return 0;
}





/* para que funcione en caso tal 
cd ~/simulador-procesos/src
g++ -Wall -Wextra -g main.cpp proceso.cpp cargador.cpp -o simulador
./simulador       */


/*mejorar makefile, cambiar quantum siempre todo igual = 3, que dependen de mi rafaga de cpu que eso lo debo preguntar*/