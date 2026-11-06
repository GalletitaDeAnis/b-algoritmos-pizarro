# Tu primera tabla de mediciones (Búsqueda Lineal)

Ejecutando mediciones...

| N | Cantidad de Datos | Tiempo Promedio (ms) | Ratio |
|---|---|---|---|
| n | 50000000 | 126.8566 ms | - |
| 4n | 200000000 | 505.2869 ms | ~3.98x |
| 16n | 800000000 | 1995.5670 ms | ~3.95x |

*Justificación: Al cuadruplicar la cantidad de datos (N), el tiempo de ejecución también se multiplicó por un factor muy cercano a 4 (Ratio ~4x). Las pequeñas fluctuaciones de milisegundos se deben a los procesos de fondo de mi sistema operativo, pero esta proporción constante demuestra empíricamente que el algoritmo tiene una complejidad temporal O(n).*
