// Practica clase 6: eleccion del algoritmo de ordenamiento para MI caso.
// Caso: conciliacion del Registro de Compras (RCV) del SIN contra el sistema contable.
// Los datos imitan el formato de h1/muestra_registro_compras.csv:
//   NIT del proveedor, nro de factura, codigo de autorizacion (texto largo), fecha, importe.
// Clave para ordenar/buscar: (NIT, nro_factura).
//
// Parte A: que ordenamiento conviene para estas facturas, en los escenarios reales de llegada.
// Parte B: el proceso completo de conciliacion, hoy (O(n^2)) contra la version ordenada.
// Protocolo: 1 corrida de calentamiento (descartada), 3 repeticiones, promedio.
// Compilar: g++ -O2 -std=c++17 -Wl,--stack,268435456 -o eleccion eleccion.cpp
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <functional>
#include <random>
#include <string>
#include <vector>

using namespace std;
using reloj = chrono::steady_clock;

struct Factura {
    uint64_t nit;
    uint64_t nro;
    string codigoAutorizacion;  // ~56 caracteres, como en el RCV
    int dia;                    // dia del anio de la fecha de factura
    double importe;
};

bool menor(const Factura& a, const Factura& b) {  // orden por clave (NIT, nro)
    return a.nit != b.nit ? a.nit < b.nit : a.nro < b.nro;
}
bool mismaClave(const Factura& a, const Factura& b) { return a.nit == b.nit && a.nro == b.nro; }

// ================= Ordenamientos (los mismos de c5, para facturas) =================
void insercion(vector<Factura>& a) {
    for (size_t i = 1; i < a.size(); i++) {
        Factura x = move(a[i]);
        size_t j = i;
        while (j > 0 && menor(x, a[j - 1])) { a[j] = move(a[j - 1]); j--; }
        a[j] = move(x);
    }
}

void mergeRec(vector<Factura>& a, vector<Factura>& tmp, int lo, int hi) {
    if (hi - lo < 2) return;
    int mid = lo + (hi - lo) / 2;
    mergeRec(a, tmp, lo, mid);
    mergeRec(a, tmp, mid, hi);
    int i = lo, j = mid, k = lo;
    while (i < mid && j < hi) tmp[k++] = menor(a[j], a[i]) ? move(a[j++]) : move(a[i++]);
    while (i < mid) tmp[k++] = move(a[i++]);
    while (j < hi) tmp[k++] = move(a[j++]);
    for (int t = lo; t < hi; t++) a[t] = move(tmp[t]);
}
void mergeSort(vector<Factura>& a) {
    vector<Factura> tmp(a.size());
    mergeRec(a, tmp, 0, (int)a.size());
}

// QuickSort clasico (pivote = ultimo). Su peor caso es con datos YA ordenados.
void quickRec(vector<Factura>& a, int lo, int hi) {
    if (lo >= hi) return;
    int i = lo;
    for (int j = lo; j < hi; j++)
        if (menor(a[j], a[hi])) swap(a[i++], a[j]);
    swap(a[i], a[hi]);
    quickRec(a, lo, i - 1);
    quickRec(a, i + 1, hi);
}
void quickSort(vector<Factura>& a) { quickRec(a, 0, (int)a.size() - 1); }

void stdStableSort(vector<Factura>& a) { stable_sort(a.begin(), a.end(), menor); }

// ================= Generacion de datos con forma de RCV =================
mt19937_64 rng(2026);

string codigoAleatorio() {
    static const char hex[] = "0123456789ABCDEF";
    string s(56, '0');
    for (char& c : s) c = hex[rng() % 16];
    return s;
}

// n facturas de ~n/12 proveedores; cada proveedor numera sus facturas en orden de fecha.
vector<Factura> generarRCV(int n) {
    int proveedores = max(20, n / 12);
    vector<uint64_t> nits(proveedores);
    for (auto& x : nits) x = 1000000000ULL + rng() % 9000000000ULL;
    vector<Factura> v(n);
    vector<uint64_t> siguienteNro(proveedores);
    for (auto& x : siguienteNro) x = 1 + rng() % 90000;
    vector<int> dias(n);
    for (int& d : dias) d = rng() % 365;
    sort(dias.begin(), dias.end());  // el SIN entrega el RCV en orden de FECHA
    for (int i = 0; i < n; i++) {
        int p = rng() % proveedores;
        v[i] = {nits[p], siguienteNro[p]++, codigoAleatorio(), dias[i], (double)(rng() % 2000000) / 100.0};
    }
    return v;
}

// ================= Medicion =================
double medirOrden(void (*ordenar)(vector<Factura>&), const vector<Factura>& datos) {
    double total = 0;
    for (int r = 0; r < 4; r++) {  // r = 0 calentamiento
        vector<Factura> copia = datos;
        auto t0 = reloj::now();
        ordenar(copia);
        auto t1 = reloj::now();
        if (!is_sorted(copia.begin(), copia.end(), menor)) { printf("ERROR: no ordeno\n"); exit(1); }
        if (r > 0) total += chrono::duration<double, milli>(t1 - t0).count();
    }
    return total / 3.0;
}

// ---- Conciliacion: devuelve cuantas facturas del SIN NO estan en el sistema ----
// Hoy: por cada factura del SIN se recorre todo el sistema. O(n^2)
int conciliarHoy(const vector<Factura>& sin, vector<Factura> sistema) {
    int faltan = 0;
    for (const Factura& f : sin) {
        bool encontrada = false;
        for (const Factura& g : sistema)
            if (mismaClave(f, g)) { encontrada = true; break; }
        if (!encontrada) faltan++;
    }
    return faltan;
}
// Propuesta 1: ordenar el sistema con MergeSort + busqueda binaria por cada factura. O(n log n)
int conciliarBinaria(const vector<Factura>& sin, vector<Factura> sistema) {
    mergeSort(sistema);
    int faltan = 0;
    for (const Factura& f : sin) {
        int lo = 0, hi = (int)sistema.size() - 1;
        bool encontrada = false;
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            if (mismaClave(sistema[mid], f)) { encontrada = true; break; }
            if (menor(sistema[mid], f)) lo = mid + 1; else hi = mid - 1;
        }
        if (!encontrada) faltan++;
    }
    return faltan;
}
// Propuesta 2: ordenar AMBAS listas con MergeSort y recorrerlas juntas una sola vez. O(n log n)
int conciliarMezcla(vector<Factura> sin, vector<Factura> sistema) {
    mergeSort(sin);
    mergeSort(sistema);
    int faltan = 0;
    size_t j = 0;
    for (const Factura& f : sin) {
        while (j < sistema.size() && menor(sistema[j], f)) j++;
        if (j < sistema.size() && mismaClave(sistema[j], f)) j++;
        else faltan++;
    }
    return faltan;
}

double medirConciliacion(function<int()> conciliar, int esperado) {
    double total = 0;
    for (int r = 0; r < 4; r++) {
        auto t0 = reloj::now();
        int faltan = conciliar();
        auto t1 = reloj::now();
        if (faltan != esperado) { printf("ERROR: resultado distinto (%d vs %d)\n", faltan, esperado); exit(1); }
        if (r > 0) total += chrono::duration<double, milli>(t1 - t0).count();
    }
    return total / 3.0;
}

int main() {
    const int tamanios[] = {5000, 20000, 80000};
    const char* nombreTam[] = {"5.000 (1 mes)", "20.000 (4 meses)", "80.000 (anio completo)"};

    printf("## Parte A: ordenar facturas por (NIT, nro_factura)\n\n");
    printf("Tiempos en ms. Escenarios: **fecha** = como lo entrega el SIN (orden por fecha), "
           "**casi** = ya ordenado por clave con 2%% de facturas fuera de lugar (agregadas despues), "
           "**ordenado** = archivo ya ordenado (se vuelve a correr la conciliacion).\n\n");
    printf("| Facturas | Escenario | Insercion | MergeSort | QuickSort (pivote ultimo) | std::stable_sort |\n");
    printf("|---|---|---|---|---|---|\n");

    for (int k = 0; k < 3; k++) {
        int n = tamanios[k];
        vector<Factura> porFecha = generarRCV(n);
        vector<Factura> ordenado = porFecha;
        stable_sort(ordenado.begin(), ordenado.end(), menor);
        vector<Factura> casi = ordenado;
        for (int i = 0; i < n / 50; i++) {  // 2% de facturas movidas a cualquier lugar
            int a = rng() % n, b = rng() % n;
            swap(casi[a], casi[b]);
        }
        struct { const char* nombre; vector<Factura>* datos; } escenarios[] = {
            {"fecha", &porFecha}, {"casi", &casi}, {"ordenado", &ordenado}};
        for (auto& e : escenarios) {
            char sI[32];
            // Insercion con datos en desorden y 80.000 filas tarda demasiado: se omite.
            if (n >= 80000 && e.datos == &porFecha) snprintf(sI, sizeof sI, "no se corre (>30 s)");
            else snprintf(sI, sizeof sI, "%.2f", medirOrden(insercion, *e.datos));
            double tM = medirOrden(mergeSort, *e.datos);
            double tQ = medirOrden(quickSort, *e.datos);
            double tS = medirOrden(stdStableSort, *e.datos);
            printf("| %s | %s | %s | %.2f | %.2f | %.2f |\n", nombreTam[k], e.nombre, sI, tM, tQ, tS);
            fflush(stdout);
        }
    }

    printf("\n## Parte B: proceso completo de conciliacion (SIN contra sistema)\n\n");
    printf("El sistema tiene el 1%% de las facturas sin registrar (faltantes). Las tres versiones "
           "deben encontrar exactamente las mismas faltantes.\n\n");
    printf("| Facturas | Hoy: lineal O(n^2) (ms) | MergeSort + binaria (ms) | MergeSort + recorrido conjunto (ms) | Mejora (hoy / mejor) | Faltantes encontradas |\n");
    printf("|---|---|---|---|---|---|\n");
    for (int k = 0; k < 3; k++) {
        int n = tamanios[k];
        vector<Factura> sin = generarRCV(n);
        vector<Factura> sistema;
        for (const Factura& f : sin)
            if (rng() % 100 != 0) sistema.push_back(f);  // 1% no fue registrado
        shuffle(sistema.begin(), sistema.end(), rng);  // el sistema exporta en otro orden
        int esperado = conciliarMezcla(sin, sistema);

        double tHoy = medirConciliacion([&] { return conciliarHoy(sin, sistema); }, esperado);
        double tBin = medirConciliacion([&] { return conciliarBinaria(sin, sistema); }, esperado);
        double tMez = medirConciliacion([&] { return conciliarMezcla(sin, sistema); }, esperado);
        printf("| %s | %.2f | %.2f | %.2f | ~%.0fx | %d |\n", nombreTam[k], tHoy, tBin, tMez,
               tHoy / min(tBin, tMez), esperado);
        fflush(stdout);
    }
    return 0;
}
