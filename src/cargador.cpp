#include "proceso.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

class CargadorProcesos {
private:
    std::vector<Proceso> listaProcesos;

public:
    // Cargar procesos desde archivo
    bool cargarDesdeArchivo(const std::string& nombreArchivo) {
        std::ifstream archivo(nombreArchivo);
        if (!archivo.is_open()) {
            std::cout << "Error: No se pudo abrir el archivo " << nombreArchivo << std::endl;
            return false;
        }

        std::string linea;
        while (std::getline(archivo, linea)) {
            // Parsear línea: PID: 1, AX=2, BX=3, CX=1, Quantum=3
            if (linea.empty() || linea[0] == '#') continue; // Ignorar líneas vacías y comentarios
            
            Proceso nuevoProceso;
            if (parsearLinea(linea, nuevoProceso)) {
                listaProcesos.push_back(nuevoProceso);
                std::cout << "Proceso cargado: ";
                mostrarProceso(&nuevoProceso);
            }
        }

        archivo.close();
        return true;
    }

    // Cargar procesos desde consola
    void cargarDesdeConsola() {
        int numProcesos;
        std::cout << "¿Cuántos procesos desea crear? ";
        std::cin >> numProcesos;

        for (int i = 0; i < numProcesos; i++) {
            Proceso nuevoProceso;
            int pid, ax, bx, cx, quantum;
            
            std::cout << "\n--- Proceso " << (i+1) << " ---" << std::endl;
            std::cout << "PID: "; std::cin >> pid;
            std::cout << "AX: "; std::cin >> ax;
            std::cout << "BX: "; std::cin >> bx;
            std::cout << "CX: "; std::cin >> cx;
            std::cout << "Quantum: "; std::cin >> quantum;

            inicializarProceso(&nuevoProceso, pid, ax, bx, cx, quantum);
            listaProcesos.push_back(nuevoProceso);
        }
    }

    // Mostrar todos los procesos cargados
    void mostrarProcesos() {
        std::cout << "\n=== PROCESOS CARGADOS ===" << std::endl;
        for (const auto& proceso : listaProcesos) {
            mostrarProceso(&proceso);
        }
    }

    // Obtener lista de procesos
    std::vector<Proceso>& getProcesos() {
        return listaProcesos;
    }

private:
    bool parsearLinea(const std::string& linea, Proceso& proceso) {
        // Ejemplo de línea: "PID: 1, AX=2, BX=3, CX=1, Quantum=3"
        std::istringstream iss(linea);
        std::string token;
        
        int pid, ax, bx, cx, quantum;
        
        // Método simple de parsing
        if (sscanf(linea.c_str(), "PID: %d, AX=%d, BX=%d, CX=%d, Quantum=%d", 
                   &pid, &ax, &bx, &cx, &quantum) == 5) {
            inicializarProceso(&proceso, pid, ax, bx, cx, quantum);
            return true;
        }
        
        std::cout << "Error al parsear línea: " << linea << std::endl;
        return false;
    }
};