Paolo Pizarro Arispe

# H2 · Inventario de mi caso (clase 8, ajustado con la clase 9)

**Caso:** conciliación mensual del Registro de Compras (RCV) del SIN contra las facturas de compra cargadas en el sistema contable de Wistupiku (ver [`h1/`](../../h1/README.md)).

## Los datos
| Dato | Qué es | Ejemplo |
|---|---|---|
| **Clave** | **(NIT del proveedor, número de factura)**: identifica una factura sin repetirse | (1023456789, 101) |
| Código de autorización | texto largo del SIN (se usa para revisar a mano, no para buscar) | 56 caracteres |
| Fecha de la factura | día en que se emitió | 2026-10-14 |
| Importe | monto total (lo guardo en centavos para comparar exacto) | 500.00 Bs |
| Estado | válida / anulada | V |

**Cuántos:** de 3.000 a 5.000 facturas por mes **en cada lado** (SIN y sistema). En el cierre anual o al revisar varios meses juntos: de 40.000 a 60.000 por lado.

## Las operaciones que hago (en un mes de ~5.000 facturas)
| # | Operación | Para qué | Cuántas veces al mes |
|---|---|---|---|
| 1 | **Buscar por clave** (NIT, nro) | Por cada factura del SIN: ¿está en el sistema? | **~5.000 (una por factura del SIN)** ← la que más hago |
| 2 | **Insertar** | Cargar cada factura del sistema en la estructura; al insertar detecto si ya estaba (duplicada) | ~5.000 |
| 3 | **Borrar** | Sacar la factura ya conciliada; lo que queda al final son los **sobrantes** (en el sistema pero no en el SIN) | ~4.900 |
| 4 | Comparar el importe | Una vez encontrada, ver si el monto coincide | ~4.900 (es O(1), va junto con la búsqueda) |
| 5 | Recorrer en orden | Reporte de faltantes ordenado por NIT para contabilidad | 1 vez, solo sobre los ~100 faltantes |
| 6 | Rango (por fechas) | Facturas de un período al revisar varios meses en el cierre | muy pocas: ~1 al mes, unas 12 al año |
| 7 | El menor / el siguiente | Siguiente número de factura de un proveedor (para ver saltos) | casi nunca (hoy no se hace) |

**Lo que más hago:** buscar por clave exacta, insertar y borrar (las tres, ~5.000 veces al mes). El orden (operaciones 5 a 7) se necesita poco y sobre pocos datos.

## Ajuste con la clase 9
En la clase 8 la respuesta obvia era una tabla hash, porque busca por clave en O(1). En la clase 9 vimos que el hash **pierde el orden**: no da el menor, el siguiente ni un rango. Revisé si mi caso necesita orden:
- El reporte ordenado es solo de los **faltantes** (~2 % = ~100 facturas), y ordenarlos al final cuesta k log k ≈ 700 operaciones: es casi gratis.
- Los rangos por fecha y el "siguiente número" son raros (operaciones 6 y 7).

Por eso el orden **no** decide la estructura: la decide la búsqueda por clave exacta. La elección está en [`estructura.md`](estructura.md).
