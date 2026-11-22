Paolo Pizarro Arispe

# P2.3 — Medir · Parcial 2, Variante A (Turnos de una clínica)

Código: [`parcial2.cpp`](parcial2.cpp), un port a C++ de `examen-p2-A.cs`. `GestorIngenuo` tiene la misma lógica que en el .cs, y `GestorMejorado` usa un heap + un hash.
Compilar y correr (Release): `g++ -O2 -std=c++17 -o parcial2.exe parcial2.cpp` y luego `.\parcial2.exe`.

## Pasos de prueba (salida del programa)
```
GestorIngenuo : 11 de 11 pasos correctos
GestorMejorado: 11 de 11 pasos correctos
Mismo orden de atencion con 5000 pacientes: SI
```
El desempate se cumple: Ana y Carla tienen urgencia 3 y sale primero Ana, que llegó antes. Además, con 5.000 pacientes las dos versiones atienden exactamente en el mismo orden, y en cada tamaño medido el número de control de `EjecutarCarga` es igual en las dos.

## Medición
- **Carga:** `EjecutarCarga(n)`, la misma del .cs: llegan n pacientes y después se atiende a los n.
- **Tamaños que se duplican:** n = 10.000, 20.000 y 40.000. La ingenua, con el mayor, tarda ~4,5 s (dentro de 0,5 a 20 s).
- **Protocolo:** 1 corrida de calentamiento (descartada) + 3 corridas medidas + promedio. Cada corrida usa un gestor nuevo. Compilado con `g++ -O2` (Release).
- Máquina: AMD Ryzen 5 7535HS, 16 GB RAM, Windows 11.

Tiempos en ms (punto decimal, sin separador de miles):

| n | Ingenuo (ms) | Veces más | Mejorado (ms) | Veces más | Ingenuo / Mejorado |
|---|---|---|---|---|---|
| 10000 | 148.94 | - | 3.420 | - | x44 |
| 20000 | 684.18 | x4.59 | 12.141 | x3.55 | x56 |
| 40000 | 4457.04 | x6.51 | 25.883 | x2.13 | x172 |

Una segunda corrida completa dio lo mismo en forma (ingenuo: 149.41, 653.56 y 4724.00 ms; mejorado: 3.424, 12.569 y 36.636 ms).

## Conclusión
- **El ingenuo se comporta como O(n²):** al duplicar n el tiempo crece x4.6 y después x6.5, nunca x2. Son n atenciones que recorren n pacientes cada una. Que pase de x4 en 40.000 es por la **memoria caché**: la lista de 40.000 pacientes, con sus nombres, ya no cabe en la caché rápida, y cada recorrido y cada `erase` (que corre a todos los de la derecha) cuesta más.
- **El mejorado se comporta como O(n log n)** (n operaciones de O(log n)): de 20.000 a 40.000 crece x2.13, apenas más que el doble. El x3.55 del primer paso es **ruido**: son tiempos de pocos milisegundos, que dependen del reloj, de la reserva de memoria de los nombres y de los procesos de fondo (en la segunda corrida los cocientes fueron x3.67 y x2.91).
- La forma se mantiene: la ventaja crece con n, de 44 veces a **172 veces** con 40.000. Con los 100.000 pacientes de un día, la diferencia sería todavía mayor.
