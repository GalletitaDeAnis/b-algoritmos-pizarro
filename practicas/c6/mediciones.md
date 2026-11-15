# Mediciones: ordenamiento para mi caso (facturas del RCV)

**Máquina:** AMD Ryzen 5 7535HS, 16 GB RAM, Windows 11. **Compilador:** g++ 15.1 con `-O2` (modo Release).
**Formato:** tiempos en milisegundos con punto decimal (33.643 = 33,6 ms).
**Protocolo:** 1 corrida de calentamiento descartada, 3 repeticiones, promedio. El programa verifica que cada resultado quede ordenado y que todas las conciliaciones encuentren las mismas faltantes.

**Datos:** facturas sintéticas con el formato de [`h1/muestra_registro_compras.csv`](../../h1/muestra_registro_compras.csv): NIT, número de factura, código de autorización de 56 caracteres, fecha e importe. Hay unos n/12 proveedores, y cada uno numera sus facturas en orden de fecha. **Clave de orden:** (NIT, nro_factura).

## Parte A: ordenar facturas por (NIT, nro_factura). Tiempos en ms.

Escenarios reales de llegada de los datos:
- **fecha:** como lo entrega el SIN, en orden de fecha (desordenado respecto a la clave).
- **casi:** el archivo ya está ordenado por clave, pero el 2% de las facturas están fuera de lugar (se agregaron después).
- **ordenado:** el archivo ya está ordenado (por ejemplo, al volver a correr la conciliación).

| Facturas | Escenario | Inserción | MergeSort | QuickSort (pivote último) | std::stable_sort |
|---|---|---|---|---|---|
| 5.000 (1 mes) | fecha | 15.75 | 0.59 | 0.42 | 0.36 |
| 5.000 (1 mes) | casi | 0.72 | 0.42 | 0.84 | 0.22 |
| 5.000 (1 mes) | ordenado | 0.02 | 0.45 | **72.95** | 0.22 |
| 20.000 (4 meses) | fecha | 234.31 | 2.54 | 2.43 | 1.64 |
| 20.000 (4 meses) | casi | 10.67 | 1.78 | 12.84 | 1.17 |
| 20.000 (4 meses) | ordenado | 0.11 | 1.65 | **1997.19** | 1.64 |
| 80.000 (año completo) | fecha | no se corre (>30 s) | 19.42 | 18.53 | 11.79 |
| 80.000 (año completo) | casi | 320.48 | 13.68 | 48.12 | 8.24 |
| 80.000 (año completo) | ordenado | 0.88 | 12.79 | **33093.22** | 6.30 |

## Parte B: el proceso completo de conciliación (SIN contra sistema)

Al sistema le falta el 1% de las facturas (no se registraron) y exporta en otro orden que el SIN.

| Facturas | Hoy: lineal O(n²) (ms) | MergeSort + binaria (ms) | MergeSort + recorrido conjunto (ms) | Mejora (hoy / mejor) | Faltantes encontradas |
|---|---|---|---|---|---|
| 5.000 (1 mes) | 10.24 | 2.03 | 3.31 | ~5x | 60 |
| 20.000 (4 meses) | 178.32 | 9.34 | 15.58 | ~19x | 182 |
| 80.000 (año completo) | 2951.37 | 52.10 | 74.47 | ~57x | 778 |

Ratios de "Hoy" cuando los datos crecen 4x: ~17x y ~17x, lo que confirma O(n²). Ratios de "MergeSort + binaria": ~4,6x y ~5,6x, lo que confirma O(n log n).
