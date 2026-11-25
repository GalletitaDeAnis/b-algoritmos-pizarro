Paolo Pizarro Arispe

# I1 - Analizar y elegir · Evaluación integradora, Variante B (Compras de una escuela)

Código: [`integradora.cpp`](integradora.cpp), un port a C++ de `examen-int-B.cs`.

## a) Costo de `CompradorIngenuo.Elegir`: **O(n · 2ⁿ)**
- El ciclo de afuera recorre todas las máscaras de 0 a 2ⁿ − 1. Cada máscara es un subconjunto de útiles (cada bit indica "lo compro o no"): son **2ⁿ** vueltas.
- Por cada máscara, el ciclo de adentro revisa los **n** bits para sumar el costo y la utilidad.
- Total: 2ⁿ × n = **O(n · 2ⁿ)**. Cada útil más **duplica** el trabajo, así que cada +2 útiles el tiempo se multiplica por ~4.
- `EstaEnLaCompra` recorre la lista de elegidos: O(k), donde k es la cantidad de útiles comprados.

## b) La recurrencia T(n) = 2 T(n − 1) + 1
- La versión recursiva decide el primer útil (comprar o no comprar) y se llama **dos veces** con los n − 1 útiles que quedan.
- **El teorema maestro de bolsillo NO se aplica.** Sirve para T(n) = a T(n/b) + n^d, donde el problema se **divide** entre b. Aquí el tamaño solo **baja en 1** (n − 1, no n/2), así que no hay b.
- **Cuánto cuesta.** Se desarrolla la recurrencia:
  - T(n) = 2 T(n−1) + 1 = 4 T(n−2) + 2 + 1 = 8 T(n−3) + 4 + 2 + 1 = …
  - Al final queda 2ⁿ T(0) + (2ⁿ⁻¹ + … + 2 + 1) = 2ⁿ + 2ⁿ − 1.
  - Por lo tanto **T(n) = O(2ⁿ)**: es un árbol de decisiones con 2ⁿ hojas.

## c) ¿Sirve un voraz por utilidad por Bs? **No**
Contraejemplo con el primer caso de las pruebas (presupuesto **100 Bs**):

| Útil | Costo (Bs) | Utilidad | Utilidad por Bs |
|---|---|---|---|
| U1 | 20 | 120 | **6,0** |
| U2 | 40 | 200 | 5,0 |
| U3 | 60 | 240 | 4,0 |

- **Voraz** (el de mayor utilidad por Bs primero):
  - toma U1 (20 Bs) y U2 (lleva 60 Bs);
  - U3 ya no cabe (60 + 60 = 120 > 100).
  - Resultado: utilidad **320**, y sobran 40 Bs sin usar.
- **Óptimo:** U2 + U3 = 100 Bs exactos, utilidad **440**.
- Comprar U1, el más "rendidor", le quita lugar a U3, y como los útiles no se pueden partir, el voraz no puede corregirse. El voraz sí sería correcto si se pudiera comprar una fracción de un útil (mochila fraccionaria), pero aquí cada útil se compra entero o no se compra.

### Mi estrategia: **programación dinámica (mochila 0/1)**
- **Qué se repite:** la fuerza bruta se pregunta muchas veces lo mismo: "con los primeros i útiles y p Bs disponibles, ¿cuánta utilidad máxima puedo lograr?". Caminos distintos llegan a la misma pregunta.
  - Ejemplo: comprar U1 y no U2, o comprar U2 y no U1, deja dos situaciones con los mismos útiles por decidir.
  - La dinámica **anota cada respuesta en una tabla y no la vuelve a calcular**.
- **Recurrencia:**
  - `mejor[0][p] = 0`
  - `mejor[i][p] = mejor[i−1][p]` si no compro el útil i
  - `mejor[i][p] = max(mejor[i−1][p], mejor[i−1][p − costoᵢ] + utilidadᵢ)` si costoᵢ ≤ p
- **Por qué es correcta:** en cada casilla prueba las dos únicas opciones (comprar o no comprar) usando respuestas que ya son óptimas. No descarta nada como el voraz.
- **Cuáles útiles se compran:** se recorre la tabla de atrás para adelante. Si `mejor[i][p]` es distinto de `mejor[i−1][p]`, el útil i se compró, y se resta su costo a p. Esto es O(n).
- **Costo:** la tabla tiene (n + 1) × (P + 1) casillas y cada una se llena en O(1), así que es **O(n · P)** en tiempo y en memoria (P = presupuesto en Bs). Es **pseudo-polinomial**: depende del **valor** del presupuesto, no solo de cuántos útiles hay. En el código uso P = mín(presupuesto, suma de costos), porque con la suma de todos los costos ya entran todos los útiles (ver [`medicion.md`](medicion.md)).

## d) Estructura para `EstaEnLaCompra`: **tabla hash** (`unordered_set<string>`)
- Al terminar `Elegir`, se guardan los códigos comprados en un `unordered_set`. Cada consulta cuesta **O(1) promedio**.
- Recorrer la lista, como hace el ingenuo, cuesta **O(k)** por consulta. Con miles de consultas son miles × k comparaciones, contra miles × 1 con el hash.
- Llenar el conjunto cuesta O(k) una sola vez, dentro de `Elegir`.
