# Mediciones: búsqueda lineal contra búsqueda binaria

**Máquina:** AMD Ryzen 5 7535HS, 16 GB RAM, Windows 11. **Compilador:** g++ 15.1 con `-O2` (modo Release).
**Formato:** tiempos en milisegundos con punto decimal (33.643 = 33,6 ms).
**Protocolo:** 1 corrida de calentamiento descartada, 3 repeticiones, promedio.
**Qué se mide:** el tiempo total de hacer **1.000 búsquedas** sobre un arreglo ordenado de N enteros (la mitad de los valores buscados existen y la otra mitad no).

| N | Datos | Lineal (ms) | Binaria (ms) | **Cociente lineal/binaria** | Ratio lineal | Ratio binaria |
|---|---|---|---|---|---|---|
| n | 100000 | 33.643 | 0.0597 | **~564x** | - | - |
| 4n | 400000 | 139.187 | 0.0800 | **~1740x** | ~4.1x | ~1.34x |
| 16n | 1600000 | 553.894 | 0.1038 | **~5338x** | ~4.0x | ~1.30x |
| 64n | 6400000 | 4090.421 | 0.3510 | **~11655x** | ~7.4x | ~3.38x |

## ¿Cuánto cuesta ordenar primero?

La binaria exige datos ordenados. Medí lo que cuesta ordenar esos mismos datos (`std::sort`, mismo protocolo) y cuántas búsquedas hacen falta para recuperar ese costo:

| Datos | Ordenar (ms) | Búsquedas necesarias para amortizar el orden |
|---|---|---|
| 100.000 | 4.10 | ~122 |
| 400.000 | 18.62 | ~134 |
| 1.600.000 | 129.59 | ~234 |
| 6.400.000 | 623.19 | ~152 |

## Lectura

- **Lineal:** el ratio es ~4x cada vez que los datos se multiplican por 4, lo que confirma **O(n)**.
- **Binaria:** el ratio es ~1,3x (casi no crece), lo que confirma **O(log n)**. Con 4 veces más datos solo hace 2 pasos más.
- **El cociente crece con N:** la binaria no es "X veces más rápida" fija. Mientras más datos hay, más gana.
- **La fila 64n** sube más de lo esperado en las dos búsquedas (7,4x y 3,4x): 6,4 millones de enteros ocupan 25 MB y ya no caben en la caché del procesador, así que cada acceso va a la RAM, que es más lenta. La forma no cambia: la lineal sigue siendo miles de veces más lenta.

Salida del programa: `busqueda.cpp` (ver `README.md` para compilar).
