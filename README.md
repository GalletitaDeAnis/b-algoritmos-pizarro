# Análisis de Algoritmos — Paolo Pizarro

Repositorio del curso de **Análisis de Algoritmos** (Ing. Josué Chura, UAB, noviembre 2026).
Aquí va todo lo que se entrega: el caso integrador (H1, H2, ...) y las prácticas de cada clase.

> **Regla de la casa:** lo de cada semana se entrega hasta el **DOMINGO a las 23:59**, por commit en este repo.
> **El commit con fecha es la evidencia.** Lo que se sube después de la hora no cuenta.
> Todas las reglas están en [`docs/REGLAS.md`](docs/REGLAS.md).

## Estructura

```
.
├── README.md            <- este archivo (índice y estado de entregas)
├── docs/
│   ├── REGLAS.md        <- reglas del curso, protocolo de medición, O grande
│   └── CLASES.md        <- resumen de cada clase y qué se pidió
├── hitos/h2/            <- caso integrador, H2: la estructura de mi caso (tabla hash)
├── parcial1/            <- Parcial 1, variante B
├── h1/                  <- caso integrador, semana 1: mi proceso lento
│   ├── README.md
│   └── muestra_registro_compras.csv
└── practicas/           <- link de entrega semana 2: .../tree/main/practicas
    ├── c3/              <- clase 3: primera tabla de mediciones (búsqueda lineal)
    ├── c4/              <- clase 4: búsqueda lineal contra binaria
    ├── c5/              <- clase 5: torneo de ordenamientos
    └── c6/              <- clase 6: elección del ordenamiento para mi caso
        (cada una con README.md, el .cpp y mediciones.md)
```

## Estado de entregas

| Entrega | Carpeta | Plazo | Estado | Contenido |
|---|---|---|---|---|
| H1 Tu proceso lento | [`h1/`](h1/) | Dom 8 nov, 23:59 | ✅ Entregado | Conciliación RCV del SIN vs sistema: estimación **O(n²)** |
| Taller clase 3: tabla de mediciones | [`practicas/c3/`](practicas/c3/) | Dom 8 nov, 23:59 | ✅ Entregado | Búsqueda lineal en C++: ratios ~4x, **O(n)** |
| Diagnóstica de la materia | (Google Forms) | Vie 6 nov, 17:00 | — | No vale nota, no va en el repo |
| Semana 2: c4 búsqueda lineal contra binaria | [`practicas/c4/`](practicas/c4/) | Dom 15 nov, 23:59 | ✅ Entregado | Cociente de 564x a 11.655x; sí conviene ordenar primero |
| Semana 2: c5 torneo de ordenamientos | [`practicas/c5/`](practicas/c5/) | Dom 15 nov, 23:59 | ✅ Entregado | Inserción ~15x por paso (O(n²)) contra Merge/Quick ~4,5x (O(n log n)) |
| Semana 2: c6 elección para mi caso ([`eleccion.md`](practicas/c6/eleccion.md)) | [`practicas/c6/`](practicas/c6/) | Dom 15 nov, 23:59 | ✅ Entregado | **MergeSort**: QuickSort clásico tarda 33 s con facturas ya ordenadas |
| Parcial 1 (variante B) | [`parcial1/`](parcial1/) | Lun 16 nov, 20:40 | ✅ Entregado | Análisis, cura O(n log n), medición y elección |
| H2 La estructura de tu caso | [`hitos/h2/`](hitos/h2/) | Dom 22 nov, 23:59 | ⏳ Por subir | **Tabla hash** por (NIT, nro): con 80.000 facturas, 11.9 s → 21 ms (x571) |

**Link para pegar en Classroom (semana 2):** https://github.com/GalletitaDeAnis/b-algoritmos-pizarro/tree/main/practicas

## Mi caso integrador (lo que optimizo todo el mes)

**Proceso:** conciliación mensual del Registro de Compras (RCV) del SIN contra las facturas del sistema contable de Wistupiku.
Hoy compara cada factura contra todas las demás (**O(n²)**). La meta es bajar ese tiempo con mejores algoritmos
(búsqueda binaria, ordenamiento, hash...) y demostrarlo **con una tabla de mediciones**.

**Avance:** H2 (unidad 3), una **tabla hash** con clave (NIT, nro) deja la conciliación en O(n): con 80.000 facturas pasa de 11.9 s a 21 ms ([`hitos/h2/`](hitos/h2/)). Antes, en la semana 2, ordenar con MergeSort y buscar con binaria lleva el proceso a O(n log n). Con un año de facturas pasa de 2,95 s a 0,05 s (~57x) en la simulación ([`practicas/c6/`](practicas/c6/)).
Detalle en [`h1/README.md`](h1/README.md).

## Cómo entregar cada tarea (checklist)

1. Crear la carpeta que pide la tarea (`h2/`, `practicas/c4/`, ...).
2. Poner el código y un `README.md` corto: qué se pidió, qué hice, resultado.
3. Si afirmo algo sobre velocidad, agregar una **tabla de mediciones** (protocolo en `docs/REGLAS.md`).
4. `git add .`, `git commit -m "..."` y `git push` **antes del domingo 23:59**.
5. Revisar en GitHub que el commit aparece con la fecha correcta.
6. Pegar el link del repo en la tarea de Classroom y marcarla como entregada.
7. Actualizar la tabla "Estado de entregas" de este README y `docs/CLASES.md`.
