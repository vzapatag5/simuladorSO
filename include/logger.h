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
