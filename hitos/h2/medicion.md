Paolo Pizarro Arispe

# H2 · Medición: lista ingenua contra tabla hash

**Código:** [`conciliacion.cpp`](conciliacion.cpp). Es el **mismo problema** resuelto dos veces: conciliar el RCV del SIN contra el sistema y contar faltantes, sobrantes, montos distintos y duplicados, más la lista de faltantes ordenada.
- **Ingenua:** el sistema en una **lista** (`vector`) que se recorre entera para insertar (¿ya estaba?) y para buscar cada factura del SIN. O(n²).
- **Elegida:** el sistema en una **tabla hash** (`unordered_map`) con clave (NIT, nro): insertar, buscar y borrar en O(1) promedio. O(n) en total.

**Compilar y correr (Release):** `g++ -O2 -std=c++17 -o conciliacion.exe conciliacion.cpp` y luego `.\conciliacion.exe`

## Comprobación
- **Caso de prueba a mano** (5 facturas del SIN, 5 del sistema): las dos versiones dan 2 faltantes, 1 sobrante, 1 monto distinto y 1 duplicado, y la misma lista de faltantes. Salida: `Caso de prueba: lista OK | hash OK`.
- **En cada tamaño** el programa verifica que las dos versiones den exactamente el mismo resultado antes de medir (por ejemplo, con 80.000: 1608 faltantes, 800 sobrantes, 805 montos distintos y 452 duplicados).

## Datos y protocolo
- Datos simulados con forma de RCV: n facturas de ~n/12 proveedores. El sistema tiene las mismas **menos 2 %** (faltantes), **más 1 %** que no están en el SIN, 1 % con monto cambiado y 0,5 % cargadas dos veces, en otro orden.
- **Cuatro tamaños que se duplican:** 10.000 (2 meses), 20.000, 40.000 y 80.000 (más de un año).
- **Protocolo:** 1 corrida de calentamiento (descartada) + 3 corridas medidas + promedio. Compilado con `g++ -O2`.
- Máquina: AMD Ryzen 5 7535HS, 16 GB RAM, Windows 11.

Tiempos en ms (punto decimal, sin separador de miles):

| n | Ingenua: lista (ms) | Veces más | Elegida: hash (ms) | Veces más | Lista / Hash |
|---|---|---|---|---|---|
| 10000 | 57.04 | - | 1.015 | - | x56 |
| 20000 | 251.20 | x4.40 | 2.446 | x2.41 | x103 |
| 40000 | 2205.05 | x8.78 | 8.932 | x3.65 | x247 |
| 80000 | 11917.88 | x5.40 | 20.886 | x2.34 | x571 |

Una segunda corrida completa dio casi lo mismo (lista: 56.37, 247.42, 2258.68 y 11812.68 ms; hash: 0.986, 2.285, 8.972 y 21.259 ms), así que el salto en 40.000 no es casualidad.

## Conclusión
- **La lista se comporta como O(n²):** al duplicar n el tiempo se multiplica por 4 o más (x4.40, x8.78, x5.40), nunca por 2. Con 80.000 facturas tarda casi **12 segundos**, que es el "tarda minutos o se congela" del H1.
- **El hash se comporta como O(n):** al duplicar n el tiempo se multiplica por ~2 (x2.41, x2.34), con 21 ms para 80.000 facturas. La ventaja crece con n: de 56 veces más rápido con 10.000 a **571 veces** con 80.000.
- **Ruido y caché (lectura honesta):** el x8.78 de la lista y el x3.65 del hash en 40.000 son más que el x4 y el x2 esperados, y se repitieron en la segunda corrida. No es el O: es la **memoria caché** de la CPU. Hasta 20.000 facturas los datos caben en la caché rápida, y desde 40.000 ya no, así que cada acceso cuesta más. Además, los tiempos del hash son de pocos milisegundos y se mueven con el reloj y los procesos de fondo. La forma no cambia: la lista crece con n² y el hash con n.
