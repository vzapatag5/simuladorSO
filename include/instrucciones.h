

/**
 * @file instrucciones.h
 * @brief Declaración de la clase CargadorInstrucciones para la gestión y validación de instrucciones de procesos.
 *
 * Esta clase permite cargar instrucciones para procesos desde archivos o desde la consola,
 * validando su formato y almacenándolas para su posterior uso en la simulación.
 */

 /**
    * @class CargadorInstrucciones
    * @brief Clase encargada de cargar y validar instrucciones asociadas a procesos.
    *
    * Proporciona métodos para leer instrucciones desde archivos o desde la entrada estándar,
    * validando su formato y almacenándolas internamente.
    */
 
 /**
    * @brief Lee y valida las instrucciones de cada proceso desde archivos en un directorio especificado.
    * @param instrDir Directorio donde se encuentran los archivos de instrucciones (uno por proceso).
    * @param procesos Vector de procesos para los cuales se deben cargar las instrucciones.
    * @return true si todas las instrucciones fueron cargadas y validadas correctamente, false en caso contrario.
    */
 
 /**
    * @brief Solicita y valida instrucciones para cada proceso desde la consola.
    * @param procesos Vector de procesos para los cuales se deben ingresar las instrucciones.
    * @return true si todas las instrucciones fueron ingresadas y validadas correctamente, false en caso contrario.
    */
 
 /**
    * @brief Obtiene las instrucciones cargadas para todos los procesos.
    * @return Referencia constante a un vector de vectores de cadenas, donde cada subvector contiene las instrucciones de un proceso.
    */
#ifndef INSTRUCCIONES_H
#define INSTRUCCIONES_H

#include <vector>
#include <string>
#include "proceso.h"

class CargadorInstrucciones {
public:
    // Lee instr/<PID>.txt para cada proceso (VALIDA las instrucciones)
    bool cargarInstrucciones(const std::string& instrDir,
                             const std::vector<Proceso>& procesos);

    // Pide por consola las instrucciones por PID (END para terminar) y VALIDA
    bool cargarInstruccionesDesdeConsola(const std::vector<Proceso>& procesos);

    const std::vector<std::vector<std::string>>& getInstrucciones() const;

private:
    std::vector<std::vector<std::string>> instrucciones_;
};

#endif // INSTRUCCIONES_H

