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
├── h1/                  <- caso integrador, semana 1: mi proceso lento
│   ├── README.md
│   └── muestra_registro_compras.csv
└── practicas/
    └── c3/              <- taller clase 3: primera tabla de mediciones
        ├── README.md
        ├── busqueda_lineal.cpp
        └── mediciones.md
```

## Estado de entregas

| Entrega | Carpeta | Plazo | Estado | Contenido |
|---|---|---|---|---|
| H1 Tu proceso lento | [`h1/`](h1/) | Dom 8 nov, 23:59 | ✅ Entregado | Conciliación RCV del SIN vs sistema: estimación **O(n²)** |
| Taller clase 3: tabla de mediciones | [`practicas/c3/`](practicas/c3/) | Dom 8 nov, 23:59 | ✅ Entregado | Búsqueda lineal en C++: ratios ~4x, **O(n)** |
| Diagnóstica de la materia | (Google Forms) | Vie 6 nov, 17:00 | — | No vale nota, no va en el repo |

## Mi caso integrador (lo que optimizo todo el mes)

**Proceso:** conciliación mensual del Registro de Compras (RCV) del SIN contra las facturas del sistema contable de Wistupiku.
Hoy compara cada factura contra todas las demás (**O(n²)**). La meta es bajar ese tiempo con mejores algoritmos
(búsqueda binaria, ordenamiento, hash...) y demostrarlo **con una tabla de mediciones**.
Detalle en [`h1/README.md`](h1/README.md).

## Cómo entregar cada tarea (checklist)

1. Crear la carpeta que pide la tarea (`h2/`, `practicas/c4/`, ...).
2. Poner el código y un `README.md` corto: qué se pidió, qué hice, resultado.
3. Si afirmo algo sobre velocidad, agregar una **tabla de mediciones** (protocolo en `docs/REGLAS.md`).
4. `git add .`, `git commit -m "..."` y `git push` **antes del domingo 23:59**.
5. Revisar en GitHub que el commit aparece con la fecha correcta.
6. Pegar el link del repo en la tarea de Classroom y marcarla como entregada.
7. Actualizar la tabla "Estado de entregas" de este README y `docs/CLASES.md`.
