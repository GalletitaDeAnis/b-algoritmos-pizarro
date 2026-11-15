# Resumen de clases

## Clase 1 — Lun 3 nov: El duelo
- Dos programas resuelven el mismo problema con la misma respuesta: uno tarda 1 s y el otro menos de 1 ms. **La diferencia es el algoritmo.**
- Presentación del proyecto del mes: elegir un proceso real y lento del trabajo y optimizarlo durante el curso, midiendo los resultados.
- Material: `codigo-clase1.zip` (`01-lento-vs-rapido.cs`).
- Tarea: **H1 Tu proceso lento**, en [`h1/`](../h1/).

## Clases 2 y 3 — Jue 5 nov: O grande y medir bien
- Teoría de O grande y protocolo de medición (ver [`REGLAS.md`](REGLAS.md)).
- Demo: un algoritmo cuadrático que con 2.000 datos tarda unos milisegundos y con 200.000 tarda ~10 s.
- Material: `codigo-clases2y3.zip` (`02-ordenes-de-crecimiento.cs`, `03-medicion-empirica.cs`).
  Para correrlo: `dotnet run -c Release -- ordenes` o `dotnet run -c Release -- medicion` desde la carpeta `demo/`.
- Taller: primera tabla de mediciones en mi lenguaje, en [`practicas/c3/`](../practicas/c3/).

### Checklist entregado el Dom 8 nov, 23:59
1. H1 completo en `h1/` (proceso, quién lo sufre, datos de muestra, estimación de O con el porqué).
2. Tabla de mediciones del taller en `practicas/c3/`.

## Semana 2 — Clases 4, 5 y 6 (9 al 13 nov): buscar y ordenar con evidencia
Tarea: **Práctica de la semana 2**, entrega Dom 15 nov a las 23:59, pegando el link de la carpeta `practicas/` (nunca el de un commit).

- **Clase 4: búsqueda lineal contra binaria.** Lineal O(n) contra binaria O(log n), que exige datos ordenados. Protocolo de esta semana: calentar, **repetir 3 veces**, promediar.
  Se pidió una tabla con los dos tiempos y su cociente, y dos líneas sobre si en mi proceso conviene ordenar primero. Entregado en [`practicas/c4/`](../practicas/c4/).
- **Clase 5: el torneo de ordenamientos.** Un cuadrático (burbuja o inserción, O(n²)) contra MergeSort y QuickSort (O(n log n)) con datos crecientes, más un informe corto. Entregado en [`practicas/c5/`](../practicas/c5/).
- **Clase 6: elegir el ordenamiento para mi caso.** Cuál, por qué y con qué evidencia. Elegí **MergeSort** (peor caso garantizado y estable). QuickSort clásico se vuelve O(n²) con datos ya ordenados. Entregado en [`practicas/c6/`](../practicas/c6/).

### Conceptos clave de la semana
| Algoritmo | Mejor | Promedio | Peor | Estable | Memoria extra |
|---|---|---|---|---|---|
| Búsqueda lineal | O(1) | O(n) | O(n) | — | O(1) |
| Búsqueda binaria (requiere orden) | O(1) | O(log n) | O(log n) | — | O(1) |
| Inserción | O(n) (ya ordenado) | O(n²) | O(n²) | Sí | O(1) |
| MergeSort | O(n log n) | O(n log n) | O(n log n) | Sí | O(n) |
| QuickSort (pivote último) | O(n log n) | O(n log n) | O(n²) (ya ordenado) | No | O(log n) pila |

- **Estable:** si dos elementos tienen la misma clave, conservan su orden original.
- **Ordenar primero conviene** cuando se van a hacer muchas búsquedas sobre los mismos datos (en mi caso, una por cada factura).
