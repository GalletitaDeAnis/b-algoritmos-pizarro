# Reglas del curso

## Entregas
- **Todo se entrega por commit en este repositorio.** El commit con fecha es la evidencia.
- **Plazo: domingo a las 23:59** de cada semana ("lo de cada semana se entrega hasta el domingo").
- En Classroom: pegar el link del repo y marcar como entregado. Classroom **no acepta entregas después de la fecha**.
- Nombre del repo pedido por el docente: `caso-algoritmos-apellido`. Este repo se llama `b-algoritmos-pizarro`: hay que confirmar con el docente si se debe renombrar.

## Regla de oro
> **Toda afirmación de velocidad viene con su tabla de mediciones. Sin tabla no hay afirmación.**

## O grande (clase 2)
- Describe cuánto **CRECE** el trabajo cuando crecen los datos.
- Un ciclo: **O(n)**.
- Un ciclo dentro de otro: **O(n²)**.
- Las constantes se descartan porque la FORMA no cambia: O(3n) = O(n).
- O grande describe siempre el **peor caso**.
- Con 10 veces más datos, O(n) tarda unas 10 veces más y O(n²) unas 100 veces más.

## Protocolo de medición (clase 3)
1. **Descartar la primera corrida** (calentamiento).
2. **Repetir 5 veces y promediar.**
3. **Escalar los datos a n, 4n y 16n**, para que la forma aparezca en los ratios:
   - O(n): ratio de ~4x en cada paso
   - O(n²): ratio de ~16x en cada paso
   - O(log n): el ratio casi no crece
4. Compilar y correr en modo **Release** (`dotnet run -c Release`, `g++ -O2`).

## Las tres trampas
1. Medir una sola vez.
2. Medir en modo Debug.
3. Medir con datos de juguete (muy pocos datos).

## Formato de tabla de mediciones

| N | Datos | Tiempo promedio (ms) | Ratio |
|---|---|---|---|
| n | ... | ... | - |
| 4n | ... | ... | ~?x |
| 16n | ... | ... | ~?x |

Agregar una línea de conclusión, por ejemplo: "el ratio de ~Xx confirma O(...)".
