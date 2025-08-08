#ifndef PROCESO_H
#define PROCESO_H

struct Proceso {
    int pid;
    int pc;
    int ax;
    int bx;
    int cx;
    int quantum;
    char estado[10];
};

#endif // PROCESO_H
