Paolo Pizarro Arispe

# H2 · La estructura que elijo para mi caso

## Elección: **tabla hash** con clave (NIT, número de factura)
En C++: `unordered_map<Clave, importe>` (en C# sería un `Dictionary`). Las facturas del sistema se cargan en la tabla y cada factura del SIN se busca en ella. Para el reporte, al final ordeno solo la lista de faltantes.

## Costo de las operaciones que más hago
Mes normal: n ≈ 5.000 facturas por lado (log₂ 5.000 ≈ 12).

| Operación | Cuántas veces la hago (al mes) | Costo O en la tabla hash | Total al mes con hash | Con una lista simple |
|---|---|---|---|---|
| Buscar por clave (NIT, nro) | ~5.000 | **O(1)** promedio | ~5.000 | 5.000 × 5.000 = **25.000.000** |
| Insertar (y detectar duplicados) | ~5.000 | **O(1)** promedio | ~5.000 | 5.000 × 5.000 = **25.000.000** (para saber si ya estaba hay que recorrer) |
| Borrar la conciliada | ~4.900 | **O(1)** promedio | ~4.900 | O(n) cada una (buscarla primero) |
| Recorrer en orden (reporte de faltantes) | 1, sobre k ≈ 100 | **O(k log k)**: ordeno solo los faltantes | ~700 | igual |
| Rango por fechas | ~1 | O(n): el hash no tiene orden, hay que recorrer todo | ~5.000 | O(n) |

**Total del mes:** con hash ≈ **15.000 operaciones**; con la lista ≈ **50.000.000**. En el cierre anual (n ≈ 60.000), la lista llega a 2 × 60.000² ≈ **7.200.000.000**, contra unas 180.000 con el hash.

## Por qué
1. **La operación que más hago es buscar por clave EXACTA** (¿está esta factura?). Para eso, la tabla hash es lo más barato: O(1), contra O(n) de la lista y O(log n) de un árbol o de la búsqueda binaria.
2. **Por qué no sirve la lista simple:** cada búsqueda recorre todo el sistema. Con 5.000 búsquedas sobre 5.000 facturas son 25 millones de comparaciones al mes, y crece con n². Es justo el O(n²) de mi H1.
3. **Contra lo de la semana 2** (MergeSort + búsqueda binaria, O(n log n)): también funciona, pero cuesta n log n al ordenar más log n por búsqueda (~12 pasos), y cada factura que llega tarde (insertar en el medio de un arreglo ordenado) cuesta O(n). El hash hace insertar, buscar y borrar en O(1).
4. **El orden casi no lo uso** (ver [`inventario.md`](inventario.md)): solo el reporte de ~100 faltantes, y ordenarlos al final es casi gratis.
5. **Las otras estructuras no encajan:** pila y cola sacan por orden de llegada, no por clave. El heap da el más importante, pero aquí no hay "el más importante": hay que encontrar una factura en particular.

**Lo que pago a cambio:** memoria extra (los casilleros y el factor de carga; con `reserve(n)` evito el rehash a mitad del proceso) y que no tengo el orden. Además, la función hash tiene que mezclar bien el NIT y el número: con una mala, todas las facturas de un proveedor caerían en el mismo casillero y la búsqueda volvería a ser O(n).

## Cuándo cambiaría de opinión
Si contabilidad empieza a pedir **rangos por fecha o "el siguiente número de factura" de cada proveedor muchas veces** (por ejemplo, para encontrar saltos de numeración en cada conciliación), cambiaría a un **árbol de búsqueda balanceado** (`std::map` en C++, `SortedDictionary` en C#): buscar sigue siendo barato, O(log n) ≈ 12 pasos con 5.000 facturas, y además da el siguiente y los rangos en O(log n + k). Si se necesitan las dos cosas a la vez, se pueden **combinar**: hash para buscar y árbol por fecha para los rangos, a cambio del doble de memoria.
