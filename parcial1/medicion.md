Paolo Pizarro Arispe

# P1.2 — Curar y medir · Parcial 1, Variante B

Código: [`parcial1.cpp`](parcial1.cpp), un port a C++ de `examen-p1-B.cs`. La lógica de la función lenta no cambia.
Compilar y correr (Release): `g++ -O2 -std=c++17 -o parcial1.exe parcial1.cpp` y luego `.\parcial1.exe`.

## 1. La cura
**`HayComprobantesRepetidosCurado` copia la lista, ordena la copia con `std::sort` y compara cada elemento con su vecino. Si hay repetidos, al ordenar quedan juntos. Costo: O(n log n) del orden + O(n) del recorrido = O(n log n).** No modifica la lista recibida (trabaja sobre una copia), y el programa lo verifica.

Los cinco casos (salida del programa):
```
repetido en el medio       esperado true  | por pares true  | tu cura true  OK
sin repetidos              esperado false | por pares false | tu cura false OK
lista vacia                esperado false | por pares false | tu cura false OK
un solo elemento           esperado false | por pares false | tu cura false OK
repetido en los extremos   esperado true  | por pares true  | tu cura true  OK
Casos: los 5 pasan (y la lista recibida no se modifica)
```

## 2. La medición
- **Peor caso:** lista SIN repetidos (`GenerarSinRepetidos`, semilla 77), así que las dos funciones tienen que mirarlo todo. Es el mismo algoritmo que en el .cs (los valores 1000..1000+n−1 mezclados con Fisher-Yates), pero con el generador de C++ (`mt19937`), por lo que el orden de la mezcla difiere del de .NET. Siguen siendo n valores distintos, así que es igual de peor caso.
- **Tamaños que se duplican:** n = 10.000, 20.000, 40.000 y 80.000.
- **Protocolo:** 1 corrida de calentamiento (descartada) + 3 corridas medidas + promedio. Compilado con `g++ -O2` (Release).
- Máquina: AMD Ryzen 5 7535HS, 16 GB RAM, Windows 11.

| n | Por pares (ms) | x fila anterior | Curado (ms) | x fila anterior | Por pares / Curado |
|---|---|---|---|---|---|
| 10000 | 23.96 | - | 0.686 | - | x35 |
| 20000 | 96.51 | x4.03 | 1.202 | x1.75 | x80 |
| 40000 | 344.27 | x3.57 | 2.748 | x2.29 | x125 |
| 80000 | 1412.18 | x4.10 | 5.145 | x1.87 | x274 |

## Conclusión
- **Por pares se comporta como O(n²):** al duplicar n, el tiempo se multiplica por ~4 (x4.03, x3.57, x4.10), que es justo lo que predice n² ((2n)² = 4n²).
- **El curado se comporta como O(n log n):** al duplicar n, el tiempo se multiplica por ~2 (x1.75, x2.29, x1.87, con un promedio de ~2x). Es apenas más que lineal, como predice n log n. La ventaja crece con n: de 35 veces más rápido con 10.000 a 274 veces con 80.000.
- **Ruido:** los tiempos del curado son de pocos milisegundos (0,7 a 5 ms), así que sus cocientes oscilan (1,75 a 2,29) por el reloj y los procesos de fondo. El x3.57 de por pares en 40.000 también es ruido (debería ser ~4). La forma general no cambia.
- **Peor tiempo medido:** **1412.18 ms (~1,4 s), con por pares y n = 80.000.** El curado con ese mismo n tardó 5,1 ms.
