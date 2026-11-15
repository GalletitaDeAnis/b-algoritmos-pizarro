# Práctica clase 4 — Búsqueda lineal contra búsqueda binaria

**Qué se pidió:** comparar con mediciones la búsqueda lineal contra la binaria en mi lenguaje, con una tabla de los dos tiempos y su cociente, medidos con el protocolo (calentar, repetir 3 veces, promediar), y dos líneas sobre si en MI proceso conviene ordenar primero y por qué.

**Qué hice:** `busqueda.cpp` en C++. Implementa las dos búsquedas, hace 1.000 búsquedas sobre 100.000, 400.000, 1,6 millones y 6,4 millones de datos, y además mide cuánto cuesta ordenar.

**Resultado:** tabla completa en [`mediciones.md`](mediciones.md).

| Datos | Lineal (ms) | Binaria (ms) | Cociente |
|---|---|---|---|
| 100.000 | 33.6 | 0.060 | ~564x |
| 400.000 | 139.2 | 0.080 | ~1740x |
| 1.600.000 | 553.9 | 0.104 | ~5338x |
| 6.400.000 | 4090.4 | 0.351 | ~11655x |

## ¿En mi proceso conviene ordenar primero?

**Sí.** En la conciliación del RCV hago una búsqueda **por cada factura del SIN** (miles por mes, decenas de miles en el año), y ordenar cuesta lo mismo que unas 120 a 230 búsquedas lineales.
Como hago muchísimas más búsquedas que eso, ordenar una sola vez y usar binaria (O(n log n) en total) le gana por mucho a buscar lineal cada vez (O(n²) en total). Lo comprobé con facturas en [`../c6/`](../c6/), Parte B.

## Cómo correrlo
```bash
g++ -O2 -std=c++17 -o busqueda busqueda.cpp
./busqueda
```
> En Windows con MSYS2, compilar desde PowerShell o CMD (en Git Bash el `g++` puede fallar sin mostrar errores por un conflicto del PATH).
