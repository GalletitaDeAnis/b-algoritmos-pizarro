# Mediciones: torneo de ordenamientos

**Máquina:** AMD Ryzen 5 7535HS, 16 GB RAM, Windows 11. **Compilador:** g++ 15.1 con `-O2` (modo Release).
**Formato:** tiempos en milisegundos con punto decimal (33.643 = 33,6 ms).
**Protocolo:** 1 corrida de calentamiento descartada, 3 repeticiones, promedio. Los tres algoritmos ordenan la misma copia de los mismos enteros aleatorios, y el programa verifica que el resultado quede ordenado.

| N | Datos | Inserción (ms) | MergeSort (ms) | QuickSort (ms) | Ratio Inserción | Ratio Merge | Ratio Quick | Inserción / Merge |
|---|---|---|---|---|---|---|---|---|
| n | 5000 | 1.78 | 0.25 | 0.17 | - | - | - | ~7x |
| 4n | 20000 | 26.22 | 1.01 | 0.78 | ~14.7x | ~4.0x | ~4.6x | ~26x |
| 16n | 80000 | 388.36 | 4.64 | 3.57 | ~14.8x | ~4.6x | ~4.6x | ~84x |
| 64n | 320000 | 10990.90 | 37.17 | 26.47 | ~28.3x | ~8.0x | ~7.4x | ~296x |
| 256n | 1280000 | no se corre (~176 s estimado) | 175.08 | 127.11 | - | ~4.7x | ~4.8x | - |
| 1024n | 5120000 | no se corre (~47 min estimado) | 748.44 | 518.27 | - | ~4.3x | ~4.1x | - |

Las estimaciones de Inserción multiplican por 16 la fila anterior (la regla de O(n²) cuando los datos crecen 4 veces).
