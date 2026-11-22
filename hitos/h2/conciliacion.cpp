// Paolo Pizarro Arispe
// H2 - La estructura de mi caso: conciliacion del RCV del SIN contra el sistema contable.
// El MISMO problema resuelto dos veces:
//   - INGENUA: el sistema se guarda en una lista (vector) que se recorre entera en cada busqueda. O(n^2)
//   - ELEGIDA: el sistema se guarda en una tabla hash (unordered_map) con clave (NIT, nro_factura). O(n)
// Resultado de la conciliacion: faltantes (en el SIN y no en el sistema), sobrantes (en el sistema y no en
// el SIN), monto distinto y duplicados cargados en el sistema. La lista de faltantes sale ordenada por clave.
// Protocolo: 1 corrida de calentamiento (descartada) + 3 corridas medidas + promedio.
// Compilar (PowerShell): g++ -O2 -std=c++17 -o conciliacion.exe conciliacion.cpp
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <random>
#include <unordered_map>
#include <vector>

using namespace std;
using reloj = chrono::steady_clock;

struct Clave {
    uint64_t nit;  // NIT del proveedor
    uint64_t nro;  // numero de factura
    bool operator==(const Clave& o) const { return nit == o.nit && nro == o.nro; }
    bool operator<(const Clave& o) const { return nit != o.nit ? nit < o.nit : nro < o.nro; }
};
struct Factura {
    Clave clave;
    int64_t importeCentavos;  // en centavos para comparar montos sin errores de redondeo
};
// Funcion hash: mezcla NIT y numero en un solo numero de casillero
struct HashClave {
    size_t operator()(const Clave& c) const {
        uint64_t h = c.nit * 0x9E3779B97F4A7C15ULL ^ (c.nro + 0x632BE59BD9B4E019ULL + (c.nit << 6));
        return (size_t)(h ^ (h >> 29));
    }
};

struct Resultado {
    int faltantes = 0, sobrantes = 0, montoDistinto = 0, duplicados = 0;
    vector<Clave> listaFaltantes;  // para el reporte, ordenada por (NIT, nro)
    bool operator==(const Resultado& o) const {
        return faltantes == o.faltantes && sobrantes == o.sobrantes && montoDistinto == o.montoDistinto &&
               duplicados == o.duplicados && listaFaltantes == o.listaFaltantes;
    }
};

// ===================== INGENUA: una lista que se recorre entera =====================
Resultado conciliarLista(const vector<Factura>& rcv, const vector<Factura>& sistema) {
    Resultado r;
    vector<Factura> lista;  // insertar: antes hay que buscar si ya esta (duplicado) -> O(n) cada vez
    for (const Factura& g : sistema) {
        bool yaEsta = false;
        for (const Factura& x : lista)
            if (x.clave == g.clave) { yaEsta = true; break; }
        if (yaEsta) r.duplicados++;
        else lista.push_back(g);
    }
    vector<bool> usada(lista.size(), false);  // "borrar" = marcar como conciliada
    for (const Factura& f : rcv) {           // buscar por clave: recorre la lista -> O(n) cada vez
        size_t i = 0;
        while (i < lista.size() && !(!usada[i] && lista[i].clave == f.clave)) i++;
        if (i == lista.size()) { r.faltantes++; r.listaFaltantes.push_back(f.clave); continue; }
        if (lista[i].importeCentavos != f.importeCentavos) r.montoDistinto++;
        usada[i] = true;
    }
    for (bool u : usada)
        if (!u) r.sobrantes++;
    sort(r.listaFaltantes.begin(), r.listaFaltantes.end());  // reporte en orden: solo los k faltantes
    return r;
}

// ===================== ELEGIDA: tabla hash por (NIT, nro) =====================
Resultado conciliarHash(const vector<Factura>& rcv, const vector<Factura>& sistema) {
    Resultado r;
    unordered_map<Clave, int64_t, HashClave> tabla;
    tabla.reserve(sistema.size());  // sin rehash a mitad de camino
    for (const Factura& g : sistema)  // insertar: O(1) promedio; si la clave ya estaba es un duplicado
        if (!tabla.emplace(g.clave, g.importeCentavos).second) r.duplicados++;
    for (const Factura& f : rcv) {  // buscar por clave: O(1) promedio
        auto it = tabla.find(f.clave);
        if (it == tabla.end()) { r.faltantes++; r.listaFaltantes.push_back(f.clave); continue; }
        if (it->second != f.importeCentavos) r.montoDistinto++;
        tabla.erase(it);  // borrar: O(1) promedio; lo que queda al final son los sobrantes
    }
    r.sobrantes = (int)tabla.size();
    sort(r.listaFaltantes.begin(), r.listaFaltantes.end());  // reporte en orden: O(k log k), k pequeno
    return r;
}

// ===================== Datos con forma de RCV =====================
// n facturas del SIN de ~n/12 proveedores. El sistema tiene las mismas MENOS 2% (faltantes), MAS 1% que no
// estan en el SIN (sobrantes), con 1% de montos cambiados y 0,5% de facturas cargadas dos veces, en otro orden.
void generar(int n, unsigned semilla, vector<Factura>& rcv, vector<Factura>& sistema) {
    mt19937_64 azar(semilla);
    int proveedores = max(20, n / 12);
    vector<uint64_t> nits(proveedores), siguiente(proveedores);
    for (int p = 0; p < proveedores; p++) {
        nits[p] = 1000000000ULL + azar() % 9000000000ULL;
        siguiente[p] = 1 + azar() % 90000;
    }
    auto nueva = [&]() {
        int p = azar() % proveedores;
        return Factura{{nits[p], siguiente[p]++}, (int64_t)(azar() % 2000000)};
    };
    rcv.clear();
    sistema.clear();
    for (int i = 0; i < n; i++) rcv.push_back(nueva());
    for (const Factura& f : rcv) {
        uint64_t dado = azar() % 1000;
        if (dado < 20) continue;  // 2% faltantes
        Factura g = f;
        if (dado < 30) g.importeCentavos += 100;  // 1% monto distinto
        sistema.push_back(g);
        if (dado >= 30 && dado < 35) sistema.push_back(g);  // 0,5% duplicados
    }
    for (int i = 0; i < n / 100; i++) sistema.push_back(nueva());  // 1% sobrantes
    shuffle(sistema.begin(), sistema.end(), azar);  // contabilidad carga en otro orden
}

volatile int sumidero;  // evita que el compilador elimine la llamada

double medir(Resultado (*f)(const vector<Factura>&, const vector<Factura>&), const vector<Factura>& rcv,
             const vector<Factura>& sistema) {
    sumidero = f(rcv, sistema).faltantes;  // calentamiento (descartado)
    double total = 0;
    for (int r = 0; r < 3; r++) {
        auto t0 = reloj::now();
        sumidero = f(rcv, sistema).faltantes;
        auto t1 = reloj::now();
        total += chrono::duration<double, milli>(t1 - t0).count();
    }
    return total / 3.0;
}

int main() {
    // ---- Caso de prueba chico, con el resultado conocido a mano ----
    const uint64_t A = 1023456789, B = 3456789012;
    vector<Factura> rcv = {{{A, 101}, 50000}, {{A, 102}, 12000}, {{B, 7}, 99900}, {{B, 8}, 1000}, {{A, 103}, 7000}};
    vector<Factura> sistema = {
        {{B, 7}, 99900},                  // ok
        {{A, 101}, 50000},                // ok
        {{A, 101}, 50000},                // duplicado (cargada dos veces)
        {{A, 102}, 12500},                // monto distinto
        {{B, 9}, 3000},                   // sobrante (no esta en el SIN)
    };                                    // faltan (A,103) y (B,8)
    Resultado esperado;
    esperado.faltantes = 2, esperado.sobrantes = 1, esperado.montoDistinto = 1, esperado.duplicados = 1;
    esperado.listaFaltantes = {{A, 103}, {B, 8}};
    Resultado rl = conciliarLista(rcv, sistema), rh = conciliarHash(rcv, sistema);
    printf("Caso de prueba: lista %s | hash %s (esperado: 2 faltantes, 1 sobrante, 1 monto distinto, 1 duplicado)\n",
           rl == esperado ? "OK" : "FALLA", rh == esperado ? "OK" : "FALLA");
    if (!(rl == esperado && rh == esperado)) return 1;

    // ---- Medicion: 4 tamanos que se duplican ----
    printf("\n| n (facturas del SIN) | Ingenua: lista (ms) | Veces mas | Elegida: hash (ms) | Veces mas | Lista / Hash |\n");
    printf("|---|---|---|---|---|---|\n");
    double antL = 0, antH = 0;
    for (int k = 0, n = 10000; k < 4; k++, n *= 2) {
        generar(n, 2026 + k, rcv, sistema);
        Resultado a = conciliarLista(rcv, sistema), b = conciliarHash(rcv, sistema);
        if (!(a == b)) { printf("ERROR: las dos versiones no dan lo mismo con n = %d\n", n); return 1; }
        double tL = medir(conciliarLista, rcv, sistema);
        double tH = medir(conciliarHash, rcv, sistema);
        char vL[16] = "-", vH[16] = "-";
        if (k > 0) {
            snprintf(vL, sizeof vL, "x%.2f", tL / antL);
            snprintf(vH, sizeof vH, "x%.2f", tH / antH);
        }
        printf("| %d | %.2f | %s | %.3f | %s | x%.0f |\n", n, tL, vL, tH, vH, tL / tH);
        printf("  (resultado: %d faltantes, %d sobrantes, %d monto distinto, %d duplicados; iguales en las dos)\n",
               a.faltantes, a.sobrantes, a.montoDistinto, a.duplicados);
        fflush(stdout);
        antL = tL;
        antH = tH;
    }
    return 0;
}
