#ifndef INSTRUCCIONES_H
#define INSTRUCCIONES_H

#include "proceso.h"
#include <string>

class EjecutorInstrucciones {
public:
    // Ejecutar una instrucción y devolver true si el proceso debe continuar
    static bool ejecutarInstruccion(Proceso* proceso, const std::string& instruccion);
    
private:
    static bool ejecutarADD(Proceso* proceso, const std::string& params);
    static bool ejecutarSUB(Proceso* proceso, const std::string& params);
    static bool ejecutarMUL(Proceso* proceso, const std::string& params);
    static bool ejecutarINC(Proceso* proceso, const std::string& params);
    static bool ejecutarJMP(Proceso* proceso, const std::string& params);
    static bool ejecutarNOP(Proceso* proceso);
    
    static int* obtenerRegistro(Proceso* proceso, const std::string& nombre);
    static bool esNumero(const std::string& str);
    static int obtenerValor(Proceso* proceso, const std::string& operando);
};

#endif // INSTRUCCIONES_H
