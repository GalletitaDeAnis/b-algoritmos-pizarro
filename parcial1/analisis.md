Paolo Pizarro Arispe

# P1.1 — Analizar · Parcial 1, Variante B

## a) Cuatro fragmentos (peor caso)

| Fragmento | Costo O | Justificación |
|---|---|---|
| **F1** | **O(n)** | Un solo ciclo de n vueltas con trabajo constante adentro (el `if` y la suma son O(1)). |
| **F2** | **O(n²)** | Un ciclo de n vueltas dentro de otro de n vueltas: n × n comparaciones, siempre (no tiene salida temprana). |
| **F3** | **O(log n)** | `k` se duplica en cada vuelta (1, 2, 4, 8, ...), así que llega a n en log₂ n vueltas. |
| **F4** | **O(n log n)** | El `for` externo da n vueltas, y en cada una el `while` divide `k` entre 3 desde n hasta 1: log₃ n vueltas. En total n × log₃ n, y la base del log es una constante que se descarta. |

## b) Dos recurrencias (teorema maestro de bolsillo: T(n) = a·T(n/b) + n^d)

**(i) T(n) = 4 T(n/3) + n**
- a = 4, b = 3, d = 1, así que b^d = 3¹ = 3.
- Comparación: **a = 4 > b^d = 3**, dominan las hojas.
- Forma: **T(n) = O(n^(log₃ 4)) ≈ O(n^1.26)**.

**(ii) T(n) = 2 T(n/2) + n²**
- a = 2, b = 2, d = 2, así que b^d = 2² = 4.
- Comparación: **a = 2 < b^d = 4**, domina la raíz (el trabajo del primer nivel).
- Forma: **T(n) = O(n²)**.
