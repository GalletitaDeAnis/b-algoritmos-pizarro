Paolo Pizarro Arispe

# P2.1 — Elegir · Parcial 2, Variante A (Turnos de una clínica)

**Un día normal:** 100.000 registros, 100.000 atenciones y 30.000 consultas de `EstaEsperando`. Tomo n = 100.000 (log₂ 100.000 ≈ 17).

## Mi elección: **HEAP (cola de prioridad) + HASH (conjunto)**, combinados
- **Heap de mínimo** (`priority_queue` con `greater`) con la clave **(urgencia, llegada, nombre)**. Las tuplas se comparan campo por campo: sale la urgencia menor (la más crítica) y, **si empatan, la llegada menor (el que llegó antes)**. Ese es el desempate. `llegada` es un contador que sube con cada registro.
- **Hash** (`unordered_set<string>`) con los nombres que siguen esperando, para responder `EstaEsperando` sin recorrer.

| Operación | Cuántas veces al día | Estructura que la resuelve | Costo O |
|---|---|---|---|
| `Registrar(nombre, urgencia)` | 100.000 | heap (insertar: el dato **sube**) + hash (insertar) | **O(log n)** + O(1) = O(log n) |
| `AtenderSiguiente()` | 100.000 | heap (ver la raíz O(1) y extraerla: el último **baja**) + hash (borrar el nombre) | **O(log n)** + O(1) = O(log n) |
| `EstaEsperando(nombre)` | 30.000 | hash | **O(1)** promedio |
| `Esperando` | (pocas) | tamaño del heap | **O(1)** |

## Por qué no sirve la lista simple (con números, n = 100.000)
En la lista, `AtenderSiguiente` **recorre todos** los que esperan para encontrar al más urgente y después hace `RemoveAt`, que corre a los de la derecha. `EstaEsperando` también recorre la lista.

| | Lista simple | Heap + hash |
|---|---|---|
| Registrar (100.000) | 100.000 × 1 = 10⁵ | 100.000 × 17 = 1,7 × 10⁶ |
| Atender (100.000) | 100.000 × 100.000 = **10¹⁰** | 100.000 × 17 = 1,7 × 10⁶ |
| EstaEsperando (30.000) | 30.000 × 100.000 = **3 × 10⁹** | 30.000 × 1 = 3 × 10⁴ |
| **Total del día** | **≈ 1,3 × 10¹⁰** | **≈ 3,4 × 10⁶** |

La lista hace **unas 3.800 veces más trabajo**. Con 100.000 atenciones, cada una de O(n), el día entero es O(n²). Con el heap + hash es O(n log n).

**Por qué hacen falta las dos:** el heap da el más urgente en O(log n), pero **no sabe buscar un nombre** (tendría que recorrer el arreglo: O(n)). El hash busca un nombre en O(1), pero **no sabe quién es el más urgente** (no tiene orden). Cada una cubre lo que la otra no hace. Al atender se borra el nombre del hash, así que siempre contiene exactamente a los que esperan.

## Cuándo cambiaría de opinión
- **Si hubiera que sacar a un paciente del medio** (por ejemplo, "se fue sin ser atendido") o **listar la fila completa en orden** muchas veces, cambiaría el heap por un **árbol balanceado**: `set<tuple<urgencia, llegada, nombre>>`, que en C# es `SortedSet`. Da el más urgente con `begin()` en O(log n), borra cualquiera en O(log n) y recorre en orden. El heap no puede borrar del medio sin recorrer.
- **Si `EstaEsperando` desapareciera** (0 consultas), bastaría el heap solo, sin el hash, y se ahorraría memoria.
- **Si las urgencias siguieran siendo solo 5 niveles fijos** y nunca un puntaje, se podría usar **5 colas FIFO** (una por urgencia): registrar es encolar en la cola de su nivel, O(1), y atender es tomar el primero de la primera cola no vacía, O(5) = O(1). Elijo el heap porque sirve igual si la urgencia pasa a tener más niveles, y log₂ 100.000 ≈ 17 pasos ya es muy poco.
