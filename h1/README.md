# H1 - Mi proceso lento (caso integrador)

**Proceso:** conciliación mensual del Registro de Compras (RCV) del SIN contra las facturas de compra cargadas en el sistema contable de Wistupiku.

1. Cada fin de mes, como encargado de sistemas, descargo del portal del SIN el archivo del Registro de Compras (CSV) con todas las facturas emitidas a nombre de la empresa.
2. Ese archivo se tiene que cruzar con las facturas que contabilidad registró en nuestro sistema, para encontrar facturas faltantes, duplicadas o con monto distinto.
3. Hoy el cruce se hace con un script/planilla que, por cada factura del SIN, recorre TODA la lista del sistema buscando la que tenga el mismo NIT, número de factura y código de autorización.
4. **Quién lo sufre:** el área de contabilidad (no puede cerrar el mes ni presentar el IVA hasta tener el cruce) y yo, que tengo que correrlo y revisar los resultados.
5. **Cuántos datos maneja:** entre 3.000 y 5.000 facturas por mes en cada lado; al hacer el cierre anual o revisar varios meses juntos se llega a 40.000 - 60.000 filas.
6. **Cuánto tarda hoy:** un mes normal tarda varios minutos; con datos de varios meses tarda más de una hora y a veces Excel se congela y hay que volver a empezar.
7. El problema crece cada año porque la empresa recibe más facturas electrónicas (facturación en línea).
8. Formato real de los datos: ver `muestra_registro_compras.csv` (20 filas de ejemplo, NIT y razones sociales anonimizados).
9. La meta del mes es bajar ese tiempo a segundos cambiando el algoritmo del cruce, y demostrarlo con una tabla de mediciones.

**Estimación de O:** O(n²) — porque por cada una de las n facturas del SIN se recorren las n facturas del sistema (un ciclo dentro de otro).
