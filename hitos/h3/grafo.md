Paolo Pizarro Arispe

# H3 · ¿Mi caso se puede modelar como un grafo?

**Sí.** Mi caso es la conciliación de compras de Wistupiku ([`h1/`](../../h1/README.md)), y se puede ver como grafo de tres maneras. La primera es la que más se parece a lo que ya resolví.

## Resumen
| Modelo | Nodos | Aristas | ¿Dirigido / con peso? | Qué pregunta responde | Costo |
|---|---|---|---|---|---|
| **1. Emparejar** (principal) | Facturas del SIN y facturas del sistema | Misma clave (NIT, nro) | No dirigido, sin peso | ¿Cuáles faltan, sobran o están duplicadas? | O(n) con hash |
| **2. Saldos** | Wistupiku y ~400 proveedores | Factura (proveedor → Wistupiku) y pago (Wistupiku → proveedor) | Dirigido, peso = importe | ¿Cada proveedor queda en saldo 0? | O(V + E) |
| **3. Dinámica de H3** | Estados (i, s): facturas decididas, centavos cubiertos | Incluir o no incluir la factura i | Dirigido, sin ciclos (DAG) | ¿Hay un camino de (0, 0) a (n, pago)? | O(n · S) |

**Tamaño en un mes:** ~5.000 facturas de cada lado, es decir **V ≈ 10.000 nodos** en el modelo 1. Cada factura tiene a lo sumo una arista (más si está duplicada), así que **E ≈ 5.000**. Es un grafo **disperso**: E es mucho menor que V², por eso se guarda como **lista de adyacencia** y no como matriz (una matriz de 10.000 × 10.000 serían 100 millones de casillas, casi todas vacías).

## 1. Conciliar = emparejar dos lados (grafo bipartito)
- **Nodos:** cada factura es un nodo, en dos grupos: las del **SIN** (RCV) a la izquierda y las del **sistema contable** a la derecha.
- **Aristas:** une una factura del SIN con una del sistema **si tienen la misma clave (NIT, nro)**.
- **Lo que busco en el grafo:**
  - **Nodo con 1 arista:** factura conciliada. Si los importes difieren, la arista se marca como "monto distinto".
  - **Nodo del SIN sin aristas:** **faltante** (no se registró en el sistema).
  - **Nodo del sistema sin aristas:** **sobrante** (no está en el SIN).
  - **Nodo con 2 o más aristas:** **duplicada**.
- **Costo:** construir las aristas comparando todos contra todos es mi O(n²) de H1. Con la tabla hash de H2, cada nodo encuentra su arista en O(1), así que el grafo se arma en **O(n)**. Es lo mismo que ya medí, visto como grafo.

```
 SIN                 Sistema
 (NIT 1, 101) ────── (NIT 1, 101)    conciliada
 (NIT 1, 102)                        faltante (sin arista)
 (NIT 2, 7)   ────── (NIT 2, 7)      monto distinto
                     (NIT 3, 55)     sobrante (sin arista)
```

## 2. Quién le debe a quién (grafo dirigido con pesos)
- **Nodos:** Wistupiku y los ~400 **proveedores** (NIT).
- **Aristas:** **proveedor → Wistupiku** por cada factura (peso = importe) y **Wistupiku → proveedor** por cada pago (peso = monto transferido).
- **Lo que busco:** el saldo de cada proveedor es la suma de las aristas que entran menos la de las que salen. Si no da 0, hay algo que revisar. Recorrer todas las aristas una vez cuesta **O(V + E)**, con V ≈ 400 proveedores y E ≈ 5.000 facturas más los pagos del mes.

## 3. La dinámica de H3 es un camino en un grafo (DAG de estados)
- **Nodos:** los estados **(i, s)**: "ya decidí las primeras i facturas y llevo s centavos cubiertos".
- **Aristas:** desde (i, s) salen dos, **una por cada decisión**:
  - **no incluir** la factura i: va a (i+1, s);
  - **incluirla**: va a (i+1, s + fᵢ), si no se pasa del pago.
- **Lo que busco:** **¿hay un camino** desde (0, 0) hasta (n, pago)? Es un grafo **sin ciclos**, porque i solo crece.
- **Por qué la dinámica gana:**
  - La **fuerza bruta** recorre **cada camino** por separado: 2ⁿ caminos.
  - La **dinámica** visita **cada nodo una sola vez**: n × S nodos.
  - Como muchos caminos llegan al mismo nodo, la dinámica es **O(n · S)** y no O(2ⁿ).

## Cuál usaría
Para la conciliación del mes **no me hace falta un algoritmo de grafos**: el modelo 1 ya está resuelto con la tabla hash de H2, en O(n). El grafo **sí me sirve para explicar** el proceso:
- las **faltantes y sobrantes son nodos aislados** y las **duplicadas son nodos con más de una arista**;
- la dinámica de H3 es **buscar un camino** sin repetir nodos.

Si más adelante quisiera ver **cadenas de pagos entre empresas del grupo**, el modelo 2 ya necesitaría recorridos del grafo, como BFS o DFS en O(V + E).
