# Elección del algoritmo para MI caso — practicas/c6/eleccion.md

Paolo Pizarro · Plantilla de la clase 6. La evidencia medida está en [`mediciones.md`](mediciones.md) (programa `eleccion.cpp`) y en [`../c4/`](../c4/) y [`../c5/`](../c5/).

## 1. Mi proceso, en una línea
Conciliar el Registro de Compras (RCV) que descargo del SIN contra las facturas registradas en el sistema contable de Wistupiku, para encontrar facturas faltantes, duplicadas o con monto distinto (mi H1).

## 2. Mis datos
- **Cuántos datos manejo hoy (n):** de 3.000 a 5.000 facturas por mes en cada lista (SIN y sistema).
- **Cuántos en el peor mes o el peor día:** en el cierre anual, o al revisar varios meses juntos, de 60.000 a 80.000 facturas.
- **Qué forma tienen:**
  - Clave: (NIT del proveedor, nro de factura), dos números enteros. Cada fila trae además un código de autorización de 56 caracteres, la fecha y el importe.
  - **¿Vienen ordenados?** El SIN los entrega **ordenados por fecha**, que para la clave es desorden. Pero cuando vuelvo a correr la conciliación, o el sistema exporta por NIT, **llegan ya ordenados o casi ordenados**.
  - **¿Hay repetidos?** Sí: unos cientos de proveedores con muchas facturas cada uno (el NIT se repite mucho), y a veces facturas duplicadas o anuladas y reemitidas, que hay que detectar.
  - **Rango de valores:** los NIT van de 7 a 10 dígitos y los números de factura hasta millones. Son demasiados valores posibles para ordenar contando (counting sort).
  - **¿Cambian mucho?** Cada mes es una lista nueva, pero dentro del mes es fija: se carga una vez y se consulta miles de veces.

## 3. La operación que más pesa
**Buscar**: por cada factura del SIN (n) hay que encontrar la misma factura en el sistema (n). Hoy se hace con búsqueda lineal, así que se **comparan todas con todas: O(n²)**. Ordenar es el precio que se paga para poder buscar rápido.

## 4. Dos candidatos

| Técnica | Cómo funciona (una línea) | Costo en el peor caso (O) | Qué necesita |
|---|---|---|---|
| **A: lo de hoy, búsqueda lineal por cada factura** | Por cada factura del SIN, recorre toda la lista del sistema hasta encontrarla. | n búsquedas × O(n) = **O(n²)** | Nada: ni orden ni memoria extra. |
| **B: MergeSort + búsqueda binaria** | Ordena el sistema una vez por (NIT, nro) con MergeSort y busca cada factura del SIN partiendo al medio. | O(n log n) + n × O(log n) = **O(n log n)** | Memoria extra O(n) para el MergeSort (unos MB con 80.000 facturas), y datos ordenados (los deja así el mismo MergeSort). |

**Recurrencias de B (teorema maestro de bolsillo, T(n) = a T(n/b) + n^d):**
- **MergeSort:** T(n) = 2 T(n/2) + n, con a = 2, b = 2, d = 1. Como b^d = 2 y **a = b^d**, todos los pisos pesan lo mismo: **n^d log n = O(n log n)** (log n pisos con n de trabajo en cada uno).
- **Búsqueda binaria:** T(n) = T(n/2) + 1, con a = 1, b = 2, d = 0. Como b^d = 1 y **a = b^d**: **n^0 log n = O(log n)** por búsqueda.

**Candidato que descarté para el ordenamiento: QuickSort con pivote último.** Su promedio es T(n) = 2 T(n/2) + n, es decir O(n log n). Pero con datos ya ordenados el pivote deja todo de un lado y la recurrencia queda T(n) = T(n-1) + n, es decir **O(n²)**. Mis datos llegan ordenados con frecuencia, así que ese peor caso me toca seguido.

## 5. Mi elección y por qué
**Elijo B: MergeSort + búsqueda binaria.** Con MIS datos es mejor porque:
1. **Hago miles de búsquedas sobre los mismos datos.** En c4 medí que ordenar cuesta lo mismo que unas 120 a 230 búsquedas lineales, y yo hago de 5.000 a 80.000. El orden se paga solo muchas veces.
2. **Lo medí en el proceso completo**, con facturas con el formato del RCV: con 80.000 facturas, A tarda **2951 ms** y B tarda **52 ms (~57x)**. Las dos encuentran exactamente las mismas 778 faltantes. Cuando los datos crecen 4x, A crece ~17x (O(n²)) y B ~5x (O(n log n)).
3. **MergeSort y no QuickSort:** con 80.000 facturas ya ordenadas, QuickSort (pivote último) tardó **33093 ms** y MergeSort **12.8 ms**. MergeSort es O(n log n) incluso en el peor caso, y además es **estable**: las facturas con la misma clave mantienen su orden por fecha, así que las duplicadas quedan juntas y en el orden en que ocurrieron.
4. **No Inserción:** solo gana si el archivo ya está perfectamente ordenado (0.88 ms). Con el orden por fecha del SIN tardó 234 ms con apenas 20.000 facturas (O(n²)).

**Cuándo cambiaría de opinión:**
- **Si n es chico o busco pocas facturas:** si solo verifico unas pocas facturas sueltas (menos de ~200 búsquedas, el punto de amortización de c4), no vale la pena ordenar y la lineal (A) alcanza. Con un mes completo (n = 5.000) B ya gana ~5x, así que para el proceso mensual siempre conviene B.
- **Si solo necesitara saber "¿está o no está?"** y no el reporte ordenado por proveedor, un conjunto/diccionario por clave (HashSet, O(1) promedio por consulta) podría ser todavía más rápido (O(n) en total). Lo voy a medir cuando veamos estructuras de datos. Por ahora me quedo con B, porque contabilidad necesita el reporte ordenado por NIT y la detección de duplicados (que quedan juntos al ordenar).
- **Si los datos tuvieran pocos valores posibles** (por ejemplo, solo ordenar por mes, de 1 a 12), ordenaría contando (O(n + k)). Con NIT y nro de factura no aplica.

## 6. Cómo lo voy a medir
- **Tamaños:** n = 5.000 (un mes), 20.000 (4 meses) y 80.000 (un año), con datos con la forma del RCV. También en los tres escenarios de llegada: por fecha, casi ordenado (2% fuera de lugar) y ya ordenado.
- **Protocolo:** compilado en Release (`g++ -O2`), 1 corrida de calentamiento descartada, **3 repeticiones y promedio**. El programa verifica que el resultado esté ordenado y que todas las versiones encuentren las mismas faltantes.
- **Lectura:** los ratios entre tamaños (×4 de datos) tienen que mostrar la forma: ~16x para O(n²) y algo más de 4x para O(n log n).
- **Próximo paso:** repetir la medición con un archivo real del RCV exportado del portal del SIN, con datos anonimizados.
