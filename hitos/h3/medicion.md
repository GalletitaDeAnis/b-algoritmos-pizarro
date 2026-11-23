Paolo Pizarro Arispe

# H3 · Medición: fuerza bruta, voraz y dinámica

**Código:** [`pagos.cpp`](pagos.cpp). Compilar y correr (Release): `g++ -O2 -std=c++17 -o pagos.exe pagos.cpp` y luego `.\pagos.exe`.
- **Datos simulados con forma de mi caso:** n facturas pendientes de un proveedor, de 50,00 a 2.000,00 Bs (en centavos). El pago es la suma de una parte de ellas, así que **siempre existe una combinación exacta**.
- **Protocolo:** 1 corrida de calentamiento (descartada) + 3 corridas medidas + promedio. Compilado con `g++ -O2`. Máquina: AMD Ryzen 5 7535HS, 16 GB RAM, Windows 11.
- Tiempos en ms (punto decimal, sin separador de miles).

## 1. ¿Quién acierta? (calidad de la respuesta)
**Ejemplo a mano:** pago de 1000.00 Bs; facturas 600.00, 500.00, 500.00 y 300.00.
```
voraz    : 600.00 300.00  -> suma 900.00 Bs     (deja 100.00 Bs sin cubrir)
dinamica : 500.00 500.00  -> suma 1000.00 Bs    (exacto, igual que la fuerza bruta)
```

**1.000 pagos al azar, con 15 facturas pendientes cada uno:**

| Método | Pagos cubiertos exacto | Promedio sin cubrir |
|---|---|---|
| Fuerza bruta | 1000 de 1000 | 0.00 Bs |
| Voraz | **7 de 1000** | 71.14 Bs |
| Dinámica | 1000 de 1000 (igual a la bruta en los 1000) | 0.00 Bs |

## 2. ¿Cuánto tarda? Fuerza bruta contra dinámica (n crece de a 4)
| n | Fuerza bruta (ms) | Veces más | Dinámica (ms) | Veces más | Voraz (ms) | Pago (Bs) |
|---|---|---|---|---|---|---|
| 12 | 0.025 | - | 2.629 | - | 0.0004 | 3195.03 |
| 16 | 0.334 | x13.2 | 23.928 | x9.10 | 0.0006 | 11948.99 |
| 20 | 4.199 | x12.6 | 30.683 | x1.28 | 0.0004 | 10233.08 |
| 24 | 70.516 | x16.8 | 43.176 | x1.41 | 0.0007 | 13236.42 |
| 28 | 1156.335 | x16.4 | 74.525 | x1.73 | 0.0007 | 18301.39 |

## 3. Dinámica contra voraz con n que se duplica (la bruta ya no termina)
La última columna es n · S (S = pago en centavos): **el costo que predice la dinámica**.

| n | Dinámica (ms) | Veces más | Voraz (ms) | Veces más | Pago (Bs) | Voraz deja sin cubrir (Bs) | n · S (millones) | Veces más n · S |
|---|---|---|---|---|---|---|---|---|
| 20 | 10.55 | - | 0.0006 | - | 6705.66 | 19.99 | 13.4 | - |
| 40 | 82.66 | x7.83 | 0.0013 | x2.00 | 17863.22 | 45.08 | 71.5 | x5.3 |
| 80 | 248.83 | x3.01 | 0.0018 | x1.39 | 37654.76 | 0.10 | 301.2 | x4.2 |
| 160 | 809.40 | x3.25 | 0.0035 | x2.00 | 77098.07 | 16.95 | 1233.6 | x4.1 |

## Conclusión
- **La fuerza bruta es O(2ⁿ):** cada 4 facturas más, el tiempo se multiplica por ~16 (x12.6 a x16.8; 2⁴ = 16). Con 28 facturas ya tarda 1,2 s, y con 40 serían horas.
- **La dinámica es O(n · S), no solo de n:** sus cocientes siguen al **monto del pago** más que a n. De 16 a 20 facturas casi no cambia (x1.28), porque el pago bajó de 11.949 a 10.233 Bs. En la tabla 3, n · S crece ~x4 por paso (x5.3, x4.2, x4.1) y el tiempo x7.8, x3.0 y x3.3: sigue la misma forma. El x7.83 se aleja por **ruido y caché**: la tabla de 7,7 millones de casillas ya no cabe en la caché rápida.
- **El voraz es O(n log n) y siempre tarda microsegundos**, pero **se equivoca en 993 de cada 1.000 pagos**. Sus tiempos (0,0004 a 0,0035 ms) están al nivel del ruido del reloj, así que sus cocientes no significan nada.
- **Con pocas facturas (12), la fuerza bruta le gana a la dinámica** (0,03 ms contra 2,6 ms). La dinámica empieza a convenir desde ~20 facturas, y ahí la diferencia crece muy rápido: con 28 ya es 15 veces más rápida.
