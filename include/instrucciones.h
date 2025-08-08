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

