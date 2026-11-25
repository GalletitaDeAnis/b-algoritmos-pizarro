Paolo Pizarro Arispe

# I3 - Medir · Evaluación integradora, Variante B (Compras de una escuela)

Código: [`integradora.cpp`](integradora.cpp). Compilar y correr (Release): `g++ -O2 -std=c++17 -o integradora.exe integradora.cpp` y luego `.\integradora.exe`.

## Pruebas (salida del programa)
```
CompradorIngenuo : 8 de 8 pruebas correctas
CompradorMejorado: 8 de 8 pruebas correctas
```

## Medición
- **Carga:** `EjecutarCarga(n)`, igual que en el .cs: n útiles al azar con semilla fija n + 1000, costo de 4 a 49 Bs, utilidad de 10 a 99 y presupuesto igual a un tercio del costo total. Uso el generador de C++ (`mt19937`), así que los números no son idénticos a los del `Random` de C#, pero la forma de los datos es la misma.
- **Tamaños:** n = 20, 22, 24 y 26 (cada +2 la ingenua se multiplica por ~4). Con 26 útiles la ingenua tarda ~2,9 s, dentro de 0,5 a 20 s.
- **Protocolo:** 1 corrida de calentamiento (descartada) + 3 corridas medidas + promedio. Compilado con `g++ -O2` (Release).
- **Máquina:** AMD Ryzen 5 7535HS, 16 GB RAM, Windows 11.
- **Formato:** tiempos en ms, con punto decimal y sin separador de miles.

| n | Ingenuo (ms) | Veces más | Mejorado (ms) | Veces más | Utilidad ingenuo | Utilidad mejorado |
|---|---|---|---|---|---|---|
| 20 | 45.01 | - | 0.0118 | - | 647 | 647 |
| 22 | 169.30 | x3.76 | 0.0194 | x1.65 | 893 | 893 |
| 24 | 677.04 | x4.00 | 0.0103 | x0.53 | 839 | 839 |
| 26 | 2927.80 | x4.32 | 0.0139 | x1.35 | 700 | 700 |

Una corrida anterior dio la misma forma (ingenuo: 44.25, 158.82, 676.26 y 2874.24 ms).

**Misma utilidad verificada:** en los cuatro tamaños las dos versiones dan exactamente la misma utilidad.

## Conclusión
- **El ingenuo es exponencial, O(n · 2ⁿ):** cada +2 útiles el tiempo se multiplica por ~4 (x3.76, x4.00, x4.32), como predice 2² = 4. El factor n agrega un poco más (26/24 ≈ 1,08), por eso el último paso da x4.32.
- **El x3.76 del primer paso** se aleja de 4 por **ruido**: con 45 ms la corrida es corta, y pesan el reloj, la caché y los procesos de fondo. En la corrida anterior ese mismo paso dio x3.59.
- **El mejorado es O(n · P):** con n = 26 y P ≈ 300 Bs son unas 8.000 casillas, que se llenan en unos **10 a 20 microsegundos**. Sus cocientes (x1.65, x0.53, x1.35) **no significan nada**: los tiempos están al nivel del ruido del reloj. Además, P cambia con los datos de cada n, así que ni siquiera tiene por qué crecer.
- **Con 26 útiles, la dinámica es ~210.000 veces más rápida** (2928 ms contra 0,014 ms), y la diferencia se duplica con cada útil más.

## Mi solución con 1.000 útiles y presupuesto 10.000
```
Mejorado con 1000 utiles y presupuesto 10000: 24.16 ms (utilidad 36224)
```
Son 1.001 × 10.001 ≈ **10 millones de casillas**, en **~24 ms**. La ingenua, con 1.000 útiles, necesitaría 2¹⁰⁰⁰ combinaciones: no terminaría nunca.

## ¿Y si el presupuesto fuera 1.000.000.000 Bs?
- La dinámica es **O(n · P)**: depende del **valor** del presupuesto, no de cuántos útiles hay. Con P = 10⁹ y 1.000 útiles, la tabla tendría **10¹² casillas**.
  - **Memoria:** con 4 bytes por casilla son ~4 TB, imposible en mis 16 GB. Ni con una sola fila (10⁹ casillas, ~4 GB) cabe cómodamente.
  - **Tiempo:** 10 millones de casillas tardaron 24 ms, así que 10¹² casillas serían unas 100.000 veces más: **~40 minutos**.
- **No falla la lógica, falla el tamaño:** es pseudo-polinomial. Con números chicos va rápido, pero si el presupuesto crece, crece la tabla.
- **Lo que hice en mi código:** si el presupuesto supera la suma de todos los costos, con esa suma ya entran todos los útiles. Por eso uso **P = mín(presupuesto, suma de costos)**. Con 1.000 útiles, la suma es ~27.000 Bs, así que la tabla no pasa de ~27 millones de casillas:
```
Mejorado con 1000 utiles y presupuesto 1000000000: 60.33 ms (utilidad 53796)
```
- **Si los costos también fueran enormes** y el recorte no alcanzara, haría la dinámica **por utilidad** en lugar de por Bs: "costo mínimo para lograr una utilidad u". Cuesta O(n · U), con U = suma de utilidades (≤ 1.000 × 99 ≈ 100.000), y no depende del presupuesto.
