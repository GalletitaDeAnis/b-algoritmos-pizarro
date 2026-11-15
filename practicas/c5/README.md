# Práctica clase 5 — El torneo de ordenamientos

**Qué se pidió:** una tabla de tiempos de un algoritmo cuadrático (burbuja o inserción) contra MergeSort y QuickSort con cantidades crecientes de datos, y un informe corto de conclusiones.

**Qué hice:** `torneo.cpp` en C++, con los tres algoritmos implementados a mano:
- **Inserción:** toma cada elemento y lo corre a la izquierda hasta su lugar. Son dos ciclos anidados, así que es **O(n²)**.
- **MergeSort:** parte en mitades, ordena cada una y las mezcla. Es **O(n log n) siempre** y estable.
- **QuickSort:** versión clásica con el último elemento como pivote. Es **O(n log n) en promedio, pero O(n²) en el peor caso**.

Datos: 5.000, 20.000, 80.000, 320.000, 1,28 millones y 5,12 millones de enteros aleatorios (cada paso por 4).
Tabla completa en [`mediciones.md`](mediciones.md).

## Informe de conclusiones

1. **La teoría se cumple en los ratios.** Cuando los datos crecen 4 veces, Inserción tarda ~15x más (O(n²) predice 16x) y MergeSort/QuickSort tardan ~4,5x más (O(n log n) predice un poco más de 4x).
2. **La distancia crece sin parar.** Con 5.000 datos Inserción es 7 veces más lenta que MergeSort. Con 320.000 datos es **296 veces** más lenta (11 segundos contra 37 ms). Con 5 millones de datos Inserción tardaría unos 47 minutos y MergeSort tarda 0,75 s.
3. **QuickSort fue el más rápido con datos aleatorios** (~30% menos que MergeSort), porque ordena en el mismo arreglo sin copiar a uno auxiliar. Pero ese resultado es con datos en desorden aleatorio: en [`../c6/`](../c6/) muestro que con datos ya ordenados este QuickSort se vuelve cuadrático.
4. **El salto en la fila 64n** (~28x, ~8x, ~7x en los tres) no es del algoritmo, es de la memoria: a partir de 320.000 enteros los datos ya no caben en la caché del procesador. Afecta a los tres por igual, y en las filas siguientes los ratios vuelven a ~4,5x.
5. **Conclusión:** para datos grandes, un algoritmo cuadrático no sirve, aunque el código sea más simple. La elección real está entre MergeSort y QuickSort, y depende de cómo llegan los datos (eso se decide en c6).

## Cómo correrlo
```bash
g++ -O2 -std=c++17 -o torneo torneo.cpp
./torneo
```
