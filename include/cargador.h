
/**
 * @class CargadorProcesos
 * @brief Clase responsable de cargar procesos desde un archivo o desde la consola.
 *
 * Esta clase proporciona métodos para cargar una lista de procesos desde un archivo
 * (por ejemplo, "procesos.txt") o desde la entrada estándar (consola). Los procesos
 * cargados se almacenan internamente y pueden ser accedidos mediante un método getter.
 */
#ifndef CARGADOR_H
#define CARGADOR_H

#include <vector>
#include <string>
#include "proceso.h"

class CargadorProcesos {
public:
    // Cargar desde archivo (procesos.txt)
    bool cargarProcesos(const std::string& filename);

    // Cargar desde consola 
    bool cargarProcesosDesdeConsola();

    const std::vector<Proceso>& getProcesos() const;

private:
    std::vector<Proceso> procesos_;
};

#endif // CARGADOR_H




