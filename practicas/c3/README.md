# Práctica clase 3 — Primera tabla de mediciones

**Qué se pidió:** producir una tabla de mediciones en mi lenguaje y en mi máquina, siguiendo el protocolo
(calentamiento, 5 repeticiones, promedio, datos n / 4n / 16n).

**Qué hice:** búsqueda lineal en C++ buscando un valor que no existe (peor caso: recorre todo el arreglo).

**Resultado:** el ratio fue de ~4x en cada paso (4n y 16n), lo que confirma **O(n)**. Ver [`mediciones.md`](mediciones.md).

## Cómo correrlo
```bash
g++ -O2 -o busqueda_lineal busqueda_lineal.cpp
./busqueda_lineal
```
> Usa n = 50.000.000 enteros (16n = 800 millones, unos 3,2 GB de RAM). Si la máquina tiene poca memoria, bajar `n`.
