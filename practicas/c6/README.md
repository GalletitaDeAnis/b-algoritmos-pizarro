# Práctica clase 6 — Elección argumentada del ordenamiento para mi caso

**Qué se pidió:** elegir el algoritmo de ordenamiento para los datos de MI caso: cuál, por qué y con qué evidencia.

**Mi caso ([`h1/`](../../h1/)):** conciliación del Registro de Compras (RCV) del SIN contra las facturas del sistema contable. Son de 5.000 facturas al mes a ~80.000 en un año, y la clave es (NIT, nro_factura).

**Documento de elección (plantilla de la clase 6): [`eleccion.md`](eleccion.md).**

**Evidencia:** `eleccion.cpp` mide los algoritmos de c5 sobre **facturas con el formato real del RCV**, en los tres escenarios en que me llegan los datos. Tablas en [`mediciones.md`](mediciones.md).

## Elección: **MergeSort**

En el código final del caso voy a usar `std::stable_sort` de C++, que es un MergeSort optimizado de la biblioteca estándar.

## Por qué

1. **Nunca se degrada (O(n log n) en el peor caso).** Mis datos muchas veces ya vienen ordenados o casi ordenados: cuando vuelvo a correr la conciliación, o cuando el sistema exporta por NIT. Con 80.000 facturas ya ordenadas, **QuickSort clásico tardó 33 segundos y MergeSort 13 ms** (~2.600 veces más lento). Ese es el peor caso de QuickSort, O(n²). O grande describe el peor caso, y mi peor caso es frecuente.
2. **Es estable.** Si dos registros tienen la misma clave (por ejemplo, una factura duplicada o anulada y vuelta a emitir), MergeSort respeta su orden original por fecha. Eso me deja reportar los duplicados en el orden en que ocurrieron. QuickSort no lo garantiza.
3. **Es rápido en todos los escenarios.** Con datos por fecha (el caso normal del SIN), MergeSort y QuickSort empatan (19,4 contra 18,5 ms con 80.000). La ventaja de QuickSort en c5 era de ~30%, y eso no compensa arriesgarse a 33 segundos.
4. **Su único costo, la memoria extra O(n), no importa aquí.** 80.000 facturas ocupan unos pocos MB.

## Descartados

| Algoritmo | Por qué no |
|---|---|
| Inserción | Gana solo si el archivo ya está perfectamente ordenado (0,9 ms). Con el orden por fecha del SIN es O(n²): 234 ms con 20.000 y más de 30 s con 80.000. Con apenas el 2% desordenado ya es 23 veces más lento que MergeSort (320 contra 13,7 ms). |
| QuickSort (pivote último) | Su peor caso son justamente datos ya ordenados, que en mi caso son frecuentes: 33 s con 80.000. Además no es estable. |

## Impacto en el proceso completo (Parte B)

Ordenar con MergeSort y buscar con binaria baja la conciliación de **O(n²) a O(n log n)**. Con un año de facturas (80.000) pasa de **2,95 s a 0,05 s, unas 57 veces más rápido**, y las tres versiones encuentran exactamente las mismas 778 facturas faltantes. La mejora crece con los datos: ~5x con un mes y ~19x con 4 meses.

> Nota: estos tiempos son de la simulación en C++. La herramienta que se usa hoy (planilla/Excel) tiene constantes mucho más grandes, por eso en la práctica tarda minutos u horas. Pero la **forma** es la misma: O(n²) contra O(n log n).

## Cómo correrlo
```bash
g++ -O2 -std=c++17 -Wl,--stack,268435456 -o eleccion eleccion.cpp
./eleccion
```
> `--stack` agranda la pila: el QuickSort clásico con datos ordenados hace 80.000 llamadas recursivas anidadas y, sin esto, el programa se cae (otra razón para no usarlo). Tarda unos 2,5 minutos, casi todo por ese QuickSort.
