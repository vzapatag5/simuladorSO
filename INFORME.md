# Informe Final – Simulador de Procesos (C/C++)

## Portada
- **Curso**: Sistemas Operativos
- **Proyecto**: Simulador Round–Robin
- **Integrantes**: Mariana Carrasquilla, Valentina Zapata, David García
- **Fecha**: 07/08/2025
- **Versión compilador**: (salida de `g++ --version`)

---

## 1. Descripción del funcionamiento
- **Entrada**: procesos con registros iniciales (AX, BX, CX) y **Quantum** por proceso; instrucciones por PID.
- **Modos**: 
  - **Archivo** (`procesos.txt` + `instr/<PID>.txt`), 
  - **Consola** (menú).
- **Ejecución**: planificador **Round–Robin** recorre los procesos; en cada quantum ejecuta **ADD, SUB, MUL, INC, JMP, NOP**.
- **Salida**: trazas con carga inicial, detalle por instrucción (efecto en registros y **quantum restante**), pila cuando se agota el quantum, **cambios de contexto** y **estados finales**.

---

## 2. Arquitectura y módulos
- `cargador.*`: lee y valida procesos desde archivo o consola.
- `instrucciones.*`: carga/valida instrucciones; **validador estricto** (solo ADD/SUB/MUL/INC/JMP/NOP).
- `planificador.*`: Round–Robin, impresión paso a paso, pila, cambios de contexto.
- `main.cpp`: menú, orquestación.
- **Logger opcional**: duplicación de stdout/stderr a `.log`.

Diagrama (alto nivel):
```
main → cargador → (procesos)
     → instrucciones → (listas por PID)
     → planificador(RR) → ejecución + trazas + estados finales
```

---

## 3. Metodología de medición
1. **Entorno** (Linux):
   ```bash
   lscpu > mediciones/cpuinfo.txt
   free -m > mediciones/ram.txt
   /usr/bin/time -v ./bin/simulador > mediciones/ejecucion.out 2> mediciones/ejecucion.time
   ```
2. **Extractor**:
   - CPU: modelo, núcleos.
   - RAM: total y disponible.
   - Proceso: `Maximum resident set size` (KB).
3. **Dataset**:
   - Completa `plantillas/resultados.csv` con columnas:
     - `escenario, cpu_model, ram_total_mb, procesos, instrucciones_totales, tiempo_s, max_rss_kb`.
4. **Gráficos**:
   - Ejecuta `python3 tools/graficar.py plantillas/resultados.csv` para generar `graficos/`.

---

## 4. Resultados (tablas/gráficos)
Ejemplo de tabla (rellenar con sus mediciones):
| Escenario | CPU                      | RAM(MB) | Proc | Instr | Tiempo(s) | MaxRSS(KB) |
|-----------|--------------------------|---------|------|-------|-----------|------------|
| A        | Intel i5-8250U (4c/8t)   | 7873    | 2    | 7     | 0.021     | 10240      |
| B        | Intel i5-8250U (4c/8t)   | 7873    | 10   | 90    | 0.085     | 16384      |

(Agrega aquí los gráficos PNG que genera `tools/graficar.py`).

---

## 5. Retos y soluciones (relación con la teoría)
1. **Gramática estricta de procesos** → *validación con regex*; evita estados inválidos.
2. **Ubicación/duplicidad de headers** → *convención include/src*; **Responsabilidad Única**.
3. **Entrada archivo/consola** → *interfaz CLI/menu*; **abstracción de I/O**.
4. **Validador de instrucciones** (solo ADD/SUB/MUL/INC/JMP/NOP) → *control del conjunto de instrucciones*; evita comportamiento indefinido.
5. **Quantum off-by-one** → impresión del **quantum restante tras ejecución**; *precisión temporal*.
6. **Cambios de contexto y pila** → *simulación de conmutación y cola/stack*; **Round–Robin** y **preempción**.
7. **Logs** → *observabilidad*; depuración y reproducibilidad.
8. **Prevención de bucles infinitos** (opcional) → *límite de pasos*; trade-off entre realismo y seguridad en demos.

---

## 6. Conclusiones
- La simulación refleja la **justicia temporal** de Round–Robin y el impacto del **Quantum** en los cambios de contexto.
- La instrumentación permitió estimar el **uso de memoria** y el costo de impresión/logs.
- La validación estricta de instrucciones reduce errores y facilita el análisis de resultados.
- Trabajos futuros: más instrucciones, estados adicionales (“Bloqueado/IO”), **prioridades** y **estadísticas** (turnaround, waiting, response time).

---

## 7. Anexos
- **Versión del compilador**: (pegar salida de `g++ --version`).
- **Comandos de medición** y ejemplos de salida.
- **Fragmentos de log** representativos.