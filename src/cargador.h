/**
 * @file cargador.h
 * @brief Declaración de la clase CargadorProcesos para gestionar la carga y manejo de procesos.
 *
 * Esta clase permite cargar procesos desde un archivo o desde la consola, almacenarlos internamente,
 * mostrarlos y proporcionar acceso a la lista de procesos cargados. Utiliza la clase Proceso para
 * representar cada proceso individual.
 *
 * Métodos principales:
 * - cargarDesdeArchivo: Carga procesos desde un archivo de texto.
 * - cargarDesdeConsola: Permite ingresar procesos manualmente desde la consola.
 * - mostrarProcesos: Muestra todos los procesos cargados.
 * - getProcesos: Devuelve una referencia a la lista de procesos cargados.
 */

#ifndef CARGADOR_H
#define CARGADOR_H

#include "proceso.h"
#include <vector>
#include <string>

class CargadorProcesos {
private:
    std::vector<Proceso> listaProcesos;
    
    // Función privada para parsear líneas del archivo
    bool parsearLinea(const std::string& linea, Proceso& proceso);

public:
    // Constructor
    CargadorProcesos();
    
    // Cargar procesos desde archivo
    bool cargarDesdeArchivo(const std::string& nombreArchivo);
    
    // Cargar procesos desde consola
    void cargarDesdeConsola();
    
    // Mostrar todos los procesos cargados
    void mostrarProcesos();
    
    // Obtener lista de procesos
    std::vector<Proceso>& getProcesos();
};

#endif // CARGADOR_H