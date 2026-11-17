Paolo Pizarro Arispe

# P1.3 — Elegir · Parcial 1, Variante B

**Escenario:** n = 250.000 códigos (llegan desordenados cada mañana), q = 8.000 consultas al día del tipo «¿está este código?». **Memoria limitada: no se puede duplicar el listado.**
Dato: log₂ 250.000 ≈ 18.

## Costo total aproximado de un día (en operaciones)

| Opción | Preparar (mañana) | Consultas | **Total** |
|---|---|---|---|
| **1. Recorrer todo en cada consulta** | 0 | q × n = 8.000 × 250.000 = 2.000.000.000 | **≈ 2 × 10⁹** |
| **2. Ordenar n log n + binaria** | n log n = 250.000 × 18 = 4.500.000 | q × log n = 8.000 × 18 = 144.000 | **≈ 4,6 × 10⁶** |
| **3. Ordenar cuadrático + binaria** | n² = 250.000² = 62.500.000.000 | q × log n = 144.000 | **≈ 6,25 × 10¹⁰** |

(Peor caso de cada consulta lineal: el código no está y se recorre todo.)

## Mi elección: **opción 2**, con un ordenamiento n log n **en el lugar**
- Es **unas 430 veces más barata que la 1** (2 × 10⁹ / 4,6 × 10⁶) y **unas 13.000 veces más barata que la 3**.
- **Por la memoria:** como no se puede duplicar el listado, **no uso MergeSort** (necesita un arreglo auxiliar de tamaño n). Uso uno n log n que ordena en el mismo arreglo: **HeapSort**, o el `std::sort` de C++ (introsort), que es O(n log n) incluso en el peor caso y casi no usa memoria extra. QuickSort simple también ordena en el lugar, pero con un mal pivote cae a O(n²).
- **La opción 3 es la peor de todas:** el ordenamiento cuadrático cuesta más (6,25 × 10¹⁰) que hacer las 8.000 consultas lineales (2 × 10⁹). Ordenar solo conviene si ordenar es barato.
- **Apoyo en lo que medí (P1.2):** con 80.000 datos, el método O(n²) tardó 1.412 ms y el que ordena con n log n tardó 5,1 ms (x274). Con 250.000 datos la diferencia es todavía mayor, porque al duplicar n el cuadrático crece x4 y el n log n solo ~x2.

## ¿A partir de cuántas consultas cambiaría de opinión?
- **Entre 1 y 2:** se igualan cuando q × n = n log n + q × log n, es decir q ≈ n log n / (n − log n) ≈ **log n ≈ 18 consultas**.
  **Con menos de ~18 consultas al día** no vale la pena ordenar y conviene la opción 1 (recorrer). Con 18 o más, siempre gana la 2. Con 8.000, la 2 gana por mucho.
- **Entre 1 y 3:** se igualan cuando q × n = n², es decir **q = n = 250.000 consultas**. Recién con más de 250.000 consultas al día la 3 le ganaría a la 1. Pero **la 3 nunca le gana a la 2**, porque las dos hacen las mismas binarias y la 2 ordena más barato.
- **Si la memoria NO estuviera limitada,** podría considerar una tabla hash (O(1) por consulta) o MergeSort. Con la restricción de no duplicar el listado, me quedo con la 2.
