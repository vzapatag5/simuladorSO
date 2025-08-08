
/**
 * @file main.cpp
 * @brief Simulador de procesos con planificación Round-Robin.
 *
 * Este programa permite simular la ejecución de procesos utilizando el algoritmo de planificación Round-Robin.
 * Ofrece dos modos de entrada para los procesos e instrucciones: desde archivos o desde la consola.
 * Además, permite la opción de exportar el seguimiento de la simulación a un archivo de log.
 *
 * Funcionalidades principales:
 * - Menú interactivo para seleccionar el modo de entrada y la opción de logging.
 * - Carga de procesos e instrucciones desde archivo o consola.
 * - Ejecución de la simulación con planificación Round-Robin.
 * - Exportación opcional de la salida y errores a un archivo de log.
 * - Permite ejecutar múltiples simulaciones en una misma sesión.
 *
 * Dependencias:
 * - cargador.h: Definición de CargadorProcesos para cargar procesos.
 * - instrucciones.h: Definición de CargadorInstrucciones para cargar instrucciones.
 * - planificador.h: Definición de PlanificadorRoundRobin para la simulación.
 * - logger.h: Definición de TeeBuf para duplicar la salida a archivo.
 *
 * @author
 * @date
 */
#include <iostream>
#include <fstream>
#include <string>
#include "cargador.h"
#include "instrucciones.h"
#include "planificador.h"
#include "logger.h"  

static void sep(char ch='=', int n=60){ for(int i=0;i<n;++i) std::cout<<ch; std::cout<<"\n"; }

static int menu() {
    sep('='); std::cout << "SIMULADOR DE PROCESOS (Round-Robin)\n"; sep('=');
    std::cout << "Seleccione modo de entrada:\n"
              << "  1) Desde ARCHIVO (procesos.txt + instr/<PID>.txt)\n"
              << "  2) Desde CONSOLA\n"
              << "  3) Salir\n> ";
    std::string op; std::getline(std::cin, op);
    if (op=="1") return 1; if (op=="2") return 2; return 3;
}

int main() {
    while (true) {
        int op = menu();
        if (op == 3) { std::cout << "Saliendo...\n"; return 0; }

        // --- ACTIVAR LOG ---
        bool usarLog = false;
        std::cout << "¿Exportar el seguimiento a .log? (s/n): ";
        std::string r; std::getline(std::cin, r);
        if (!r.empty() && (r=="s" || r=="S")) usarLog = true;

        std::string logName = "simulacion.log";
        if (usarLog) {
            std::cout << "Nombre del archivo .log [simulacion.log]: ";
            std::string tmp; std::getline(std::cin, tmp);
            if (!tmp.empty()) logName = tmp;
        }

        std::ofstream log;
        TeeBuf *teeOut = nullptr, *teeErr = nullptr;
        std::streambuf *oldCout = nullptr, *oldCerr = nullptr;

        if (usarLog) {
            log.open(logName, std::ios::out | std::ios::trunc);
            if (!log) {
                std::cerr << "No se pudo abrir " << logName << " para escritura. Continuando sin log.\n";
                usarLog = false;
            } else {
                // Duplicar cout y cerr hacia el archivo:
                oldCout = std::cout.rdbuf();
                oldCerr = std::cerr.rdbuf();
                teeOut = new TeeBuf(oldCout, log.rdbuf());
                teeErr = new TeeBuf(oldCerr, log.rdbuf());
                std::cout.rdbuf(teeOut);
                std::cerr.rdbuf(teeErr);
            }
        }
        // --- FIN ACTIVAR LOG ---

        CargadorProcesos carg;
        CargadorInstrucciones ci;
        std::vector<Proceso> procesos;

        if (op == 1) {
            std::string file;
            std::cout << "Archivo de procesos [default: procesos.txt]: ";
            std::getline(std::cin, file);
            if (file.empty()) file = "procesos.txt";
            if (!carg.cargarProcesos(file)) {
                // Restaurar buffers si hubo log antes de continuar el while
                if (usarLog) {
                    std::cout.rdbuf(oldCout);
                    std::cerr.rdbuf(oldCerr);
                    delete teeOut; delete teeErr;
                    log.close();
                }
                continue;
            }
            procesos = carg.getProcesos();

            if (!ci.cargarInstrucciones("instr", procesos)) {
                if (usarLog) {
                    std::cout.rdbuf(oldCout);
                    std::cerr.rdbuf(oldCerr);
                    delete teeOut; delete teeErr;
                    log.close();
                }
                continue;
            }
        } else {
            if (!carg.cargarProcesosDesdeConsola()) {
                if (usarLog) {
                    std::cout.rdbuf(oldCout);
                    std::cerr.rdbuf(oldCerr);
                    delete teeOut; delete teeErr;
                    log.close();
                }
                continue;
            }
            procesos = carg.getProcesos();

            if (!ci.cargarInstruccionesDesdeConsola(procesos)) {
                if (usarLog) {
                    std::cout.rdbuf(oldCout);
                    std::cerr.rdbuf(oldCerr);
                    delete teeOut; delete teeErr;
                    log.close();
                }
                continue;
            }
        }

        std::cout << "\n→ Empezando simulacion...\n\n";
        PlanificadorRoundRobin rr;
        rr.schedule(procesos, ci.getInstrucciones());

        if (usarLog) {
            std::cout << "\n[Log guardado en " << logName << "]\n";
            std::cout.rdbuf(oldCout);
            std::cerr.rdbuf(oldCerr);
            delete teeOut; delete teeErr;
            log.close();
        }

        std::cout << "\n¿Desea ejecutar otra simulacion? (s/n): ";
        std::string again; std::getline(std::cin, again);
        if (again!="s" && again!="S") break;
    }
    return 0;
}
