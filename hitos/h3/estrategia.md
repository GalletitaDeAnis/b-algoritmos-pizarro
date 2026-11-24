Paolo Pizarro Arispe

# H3 · ¿Voraz o dinámica? La estrategia de mi caso

**Mi caso:** la conciliación de compras de Wistupiku ([`h1/`](../../h1/README.md)). Las semanas anteriores la resolvieron con estructuras: ordenar + binaria (semana 2) y tabla hash (H2). Esa parte es **buscar**, no **decidir**: no hay nada que optimizar, por lo que ni voraz ni dinámica aplican ahí.

Pero el mismo proceso tiene una **decisión que se repite** y que hoy se hace a mano.

## La decisión que se repite: ¿qué facturas cubre este pago?
Los proveedores no cobran factura por factura. Hacen **una transferencia por varias facturas juntas**, o nosotros les pagamos un monto global. Al conciliar el extracto bancario con las facturas pendientes del proveedor hay que decidir **qué facturas cubre ese pago**:

- **Decisión (por cada factura pendiente del proveedor):** ¿la incluyo en este pago, sí o no?
- **Resultado a optimizar:** cubrir el monto pagado **lo más posible sin pasarse**. Lo ideal es exacto, con diferencia 0 Bs. Si sobra o falta, contabilidad tiene que investigar.
- **Tamaño real:** cada proveedor tiene de 5 a 40 facturas pendientes en el mes, y hay ~400 proveedores (n/12 de las ~5.000 facturas).

Es el problema de la **suma de subconjuntos**: una **mochila** donde el peso y el valor son el mismo número (el importe).

## Las tres estrategias
| Estrategia | Cómo decide | ¿Correcta? | Costo |
|---|---|---|---|
| **Fuerza bruta** | Prueba todas las combinaciones (cada factura: incluir o no) | Sí | **O(2ⁿ)** |
| **Voraz** | Ordena de mayor a menor y toma cada factura **si cabe**: lo mejor ahora, sin volver atrás | **No siempre** | **O(n log n)** |
| **Dinámica** | Tabla de "montos alcanzables": anota lo ya calculado para no recalcularlo | Sí | **O(n · S)**, S = pago en centavos |

### ¿Alcanza el voraz? **No.** Contraejemplo
Pago de **1.000,00 Bs**; facturas pendientes: 600, 500, 500 y 300.
- **Voraz:** toma 600 (la más grande). 500 ya no cabe (1.100 > 1.000), y el otro 500 tampoco. Toma 300 y suma **900**: deja **100 Bs sin explicar**.
- **Correcto:** 500 + 500 = **1.000 exacto**.

El voraz falla porque **elegir la más grande ahora le cierra el camino** a la combinación correcta. En la simulación con 1.000 pagos de 15 facturas cada uno (siempre existe una combinación exacta), el voraz acertó **solo 7 de 1.000**. Ver [`medicion.md`](medicion.md).

### ¿Hay subproblemas que se repiten? **Sí → dinámica**
La fuerza bruta se pregunta muchas veces lo mismo: "con las facturas 1 a i, ¿puedo sumar exactamente s?". Por ejemplo, con las facturas 600, 500, 500 y 300: "tomo el primer 500 y no el segundo" y "tomo el segundo 500 y no el primero" llegan al **mismo subproblema** (500 Bs ya cubiertos, falta decidir el 300) por caminos distintos, y la fuerza bruta lo resuelve dos veces. La dinámica lo anota una sola vez. Visto como grafo, es un camino en un DAG de estados (ver [`grafo.md`](grafo.md)).

**Recurrencia:**
```
alcanzable(0, 0) = sí ;  alcanzable(0, s > 0) = no
alcanzable(i, s) = alcanzable(i−1, s)            (no incluyo la factura i)
                   O alcanzable(i−1, s − fᵢ)     (la incluyo, si fᵢ ≤ s)
```
La tabla tiene **n × S** casillas y cada una se llena en O(1), así que el costo es **O(n · S)**. En el código se usa un solo arreglo `quien[s]` de tamaño S + 1: se recorre s de mayor a menor para que cada factura se use una sola vez, y además se guarda **con qué factura** se alcanzó cada monto, para poder **reconstruir cuáles son**. Al final la respuesta es el mayor s ≤ pago que sea alcanzable.

## Mi elección: **programación dinámica**
1. **Es correcta siempre.** En la conciliación, una diferencia inventada por el voraz es peor que tardar: contabilidad perdería tiempo buscando un error que no existe.
2. **Es práctica en mis tamaños.** Con 28 facturas, la fuerza bruta tarda 1,2 s y la dinámica 0,07 s. Con 40 facturas, la bruta serían 2⁴⁰ ≈ 10¹² combinaciones (horas), y la dinámica tarda 0,08 s.
3. **Ojo, es "pseudo-polinomial":** O(n · S) depende del **monto** en centavos, no solo de cuántas facturas hay. Un pago de 77.000 Bs son 7,7 millones de centavos: la tabla ocupa ~31 MB y tarda ~0,8 s con 160 facturas. Para mis pagos (unos miles de Bs) es instantáneo.

## Cuándo cambiaría de opinión
- **Con muy pocas facturas (menos de ~20)**, la fuerza bruta es incluso **más rápida** que la dinámica (con 12 facturas: 0,03 ms contra 2,6 ms), porque 2¹² = 4.096 combinaciones es menos que una tabla de cientos de miles de centavos.
- **Si los pagos fueran enormes** (millones de Bs, S gigante) y con muchas facturas, la tabla no cabría en memoria. Usaría la dinámica con los montos en **Bs enteros** (S 100 veces menor) para proponer candidatos y revisar los centavos a mano, o aceptaría el voraz como **sugerencia rápida**, que siempre revisa un humano.
- **El voraz sí sería correcto** si los montos fueran "como monedas" (cada factura múltiplo de la siguiente, por ejemplo 1.000, 500, 100, 50), pero las facturas reales no son así.

## Panorama NP (lo visto en la clase 12)
La suma de subconjuntos es un problema **NP-completo**: nadie conoce un algoritmo que lo resuelva en tiempo polinomial **en el tamaño de los números**. La dinámica no "rompe" eso: es rápida solo porque mis montos (S) son chicos. Si S creciera sin límite, volvería a ser exponencial en la cantidad de dígitos del monto.
