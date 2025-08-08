#ifndef CARGADOR_H
#define CARGADOR_H

#include <vector>
#include <string>
#include "proceso.h"

class CargadorProcesos {
public:
    // Cargar desde archivo (procesos.txt)
    bool cargarProcesos(const std::string& filename);

    // Cargar desde consola (mismas líneas que el archivo) hasta línea vacía
    bool cargarProcesosDesdeConsola();

    const std::vector<Proceso>& getProcesos() const;

private:
    std::vector<Proceso> procesos_;
};

#endif // CARGADOR_H




