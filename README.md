# Simulador de Procesos – README

## 1) Descripción breve
Simulador Round–Robin en C++ que:
- Lee **procesos** e **instrucciones** desde **archivo** (procesos.txt + instr/<PID>.txt) **o** desde **consola** (menú).
- Ejecuta instrucciones **ADD, SUB, MUL, INC, JMP, NOP** con **quantum** por proceso.
- Muestra en terminal: carga inicial (PID, PC, Quantum, AX, BX, CX), ejecución paso a paso, quantum restante, pila al agotar quantum, cambios de contexto y estados finales.
- Opcional: exporta todo el seguimiento a un **.log** (desde el menú).

## 2) Requisitos
- Linux
- `g++` (C++17) y `make`

## 3) Estructura
```
.
├─ Makefile
├─ README.md
├─ INFORME.md
├─ procesos.txt
├─ simulacion.log #se crea si se desea en el menú
├─ include/
│  ├─ cargador.h
│  ├─ instrucciones.h
|  ├─ logger.h
|  ├─ planificador.h
│  └─ proceso.h
├─ src/
│  ├─ main.cpp
│  ├─ cargador.cpp
│  ├─ instrucciones.cpp
│  ├─ proceso.cpp
│  └─ planificador.cpp
├─ instr/
│  ├─ 1.txt
│  ├─ 2.txt
│  ├─ 3.txt
│  └─ [otros archivos PID].txt
├─ capturas/         # para el informe
│  ├─ datos1.png
│  └─ memoriausada.png
└─ bin/ obj/          # se crean solos con 'make'
```

## 4) Compilación y ejecución
```bash
make clean && make
make run
```
Luego elige:
1. **Archivo**: usa `procesos.txt` y `instr/<PID>.txt`
2. **Consola**: ingresa procesos (línea vacía para terminar) e instrucciones (END por proceso).

> También se puede habilitar la exportación a **.log** (se pregunta en el menú).

## 5) Formato de entrada
**procesos.txt**
```
PID: 1, AX=5, BX=3, Quantum=2
PID: 2, CX=7, Quantum=3
```

**instr/1.txt**
```
MUL AX, CX
ADD BX, 1
NOP
```

**instr/2.txt**
```
SUB CX, 1
INC CX
SUB CX, AX
JMP 0
```

## 6) Instrucciones soportadas
- `ADD R, X`  | `SUB R, X`  | `MUL R, X`   
- `INC R`
- `JMP N` (N ≥ 0; índice de instrucción, 0 es la primera)
- `NOP`

> **Validador estricto**: si aparece otra instrucción o sintaxis inválida, se aborta la carga.

## 7) Parámetros visibles al ejecutar
- **Carga inicial**: `PID, PC, Quantum, AX, BX, CX, #Instrucciones, Estado`
- **Paso a paso**: `Instr[i] <OP ...> | efecto en registros | Quantum restante`
- **Eventos**: pila al agotar quantum, cambios de contexto
- **Cierre**: estados finales de todos los procesos

## 8) Solución de problemas
- **Salida interminable**: probablemente un bucle (`JMP 0`). Cambia el programa o activa un **límite de pasos** (opcional en `planificador.cpp`).
- **No encuentra archivos**: asegúrate de `procesos.txt` en la **raíz** y `instr/<PID>.txt` por cada proceso.
- **Compila pero no ejecuta**: verifica permisos (`chmod +x bin/simulador`) y que los `.txt` existan.

## 10) Versión de compilador
```bash
g++ (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0
```
