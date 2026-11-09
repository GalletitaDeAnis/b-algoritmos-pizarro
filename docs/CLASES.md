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

## Próxima — Lun 9 nov: El torneo
- Búsqueda lineal contra binaria y los primeros ordenamientos.
- Se aplica a mi caso: ordenar las facturas y usar búsqueda binaria lleva el cruce de O(n²) a O(n log n).
