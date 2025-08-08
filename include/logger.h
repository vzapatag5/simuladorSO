
/**
 * @class TeeBuf
 * @brief Un buffer de flujo que escribe la salida en dos buffers de flujo separados simultáneamente.
 *
 * Esta clase hereda de std::streambuf y permite duplicar la salida en dos buffers de flujo diferentes.
 * Es útil para registrar logs o redirigir la salida a múltiples destinos, como escribir en un archivo
 * y en la consola al mismo tiempo.
 *
 * @note Esta clase no toma posesión de los buffers de flujo proporcionados.
 *
 * @param sb1 Puntero al primer buffer de flujo.
 * @param sb2 Puntero al segundo buffer de flujo.
 *
 * @fn TeeBuf::TeeBuf(std::streambuf* sb1, std::streambuf* sb2)
 * @brief Construye un TeeBuf con dos buffers de destino.
 *
 * @fn TeeBuf::overflow(int ch)
 * @brief Escribe un carácter en ambos buffers de flujo.
 * @param ch El carácter a escribir.
 * @return El carácter escrito en caso de éxito, o EOF en caso de error.
 *
 * @fn TeeBuf::sync()
 * @brief Sincroniza ambos buffers de flujo.
 * @return 0 en caso de éxito, -1 en caso de error.
 */
#ifndef LOGGER_TEEBUF_H
#define LOGGER_TEEBUF_H

#include <streambuf>
#include <ostream>

class TeeBuf : public std::streambuf {
public:
    TeeBuf(std::streambuf* sb1, std::streambuf* sb2) : sb1_(sb1), sb2_(sb2) {}

protected:
    // Escribe un carácter en ambos buffers
    int overflow(int ch) override {
        if (ch == EOF) return !EOF;
        const int r1 = sb1_->sputc(ch);
        const int r2 = sb2_->sputc(ch);
        return (r1 == EOF || r2 == EOF) ? EOF : ch;
    }
    // Sincroniza ambos buffers
    int sync() override {
        const int r1 = sb1_->pubsync();
        const int r2 = sb2_->pubsync();
        return (r1 == 0 && r2 == 0) ? 0 : -1;
    }

private:
    std::streambuf* sb1_;
    std::streambuf* sb2_;
};

#endif // LOGGER_TEEBUF_H
