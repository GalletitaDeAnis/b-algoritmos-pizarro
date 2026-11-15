// Practica clase 5: el torneo de ordenamientos.
// Insercion (cuadratico, O(n^2)) contra MergeSort y QuickSort (O(n log n)).
// Protocolo: 1 corrida de calentamiento (descartada), 3 repeticiones, promedio.
// Todos ordenan EXACTAMENTE la misma copia de los mismos datos aleatorios.
// Compilar: g++ -O2 -std=c++17 -o torneo torneo.cpp
#include <algorithm>
#include <chrono>
#include <cstdio>
#include <random>
#include <vector>

using namespace std;
using reloj = chrono::steady_clock;

// ---------- Insercion: O(n^2) ----------
// Toma cada elemento y lo corre hacia la izquierda hasta su lugar.
void insercion(vector<int>& a) {
    for (size_t i = 1; i < a.size(); i++) {
        int x = a[i];
        size_t j = i;
        while (j > 0 && a[j - 1] > x) {
            a[j] = a[j - 1];
            j--;
        }
        a[j] = x;
    }
}

// ---------- MergeSort: O(n log n) siempre ----------
// Parte en dos mitades, ordena cada una y las mezcla.
void mergeRec(vector<int>& a, vector<int>& tmp, int lo, int hi) {  // ordena [lo, hi)
    if (hi - lo < 2) return;
    int mid = lo + (hi - lo) / 2;
    mergeRec(a, tmp, lo, mid);
    mergeRec(a, tmp, mid, hi);
    int i = lo, j = mid, k = lo;
    while (i < mid && j < hi) tmp[k++] = (a[j] < a[i]) ? a[j++] : a[i++];  // estable
    while (i < mid) tmp[k++] = a[i++];
    while (j < hi) tmp[k++] = a[j++];
    for (int t = lo; t < hi; t++) a[t] = tmp[t];
}
void mergeSort(vector<int>& a) {
    vector<int> tmp(a.size());
    mergeRec(a, tmp, 0, (int)a.size());
}

// ---------- QuickSort: O(n log n) promedio, O(n^2) peor caso ----------
// Elige un pivote, deja los menores a la izquierda y los mayores a la derecha.
// Version clasica de clase: pivote = ultimo elemento (particion de Lomuto).
void quickRec(vector<int>& a, int lo, int hi) {  // ordena [lo, hi]
    if (lo >= hi) return;
    int pivote = a[hi];
    int i = lo;
    for (int j = lo; j < hi; j++)
        if (a[j] < pivote) swap(a[i++], a[j]);
    swap(a[i], a[hi]);
    quickRec(a, lo, i - 1);
    quickRec(a, i + 1, hi);
}
void quickSort(vector<int>& a) { quickRec(a, 0, (int)a.size() - 1); }

// ---------- Medicion ----------
template <class F>
double protocolo(F ordenar, const vector<int>& datos) {
    double total = 0;
    for (int r = 0; r < 4; r++) {  // r = 0 es calentamiento
        vector<int> copia = datos;  // la copia NO se mide
        auto t0 = reloj::now();
        ordenar(copia);
        auto t1 = reloj::now();
        if (!is_sorted(copia.begin(), copia.end())) { printf("ERROR: no ordeno\n"); exit(1); }
        if (r > 0) total += chrono::duration<double, milli>(t1 - t0).count();
    }
    return total / 3.0;
}

int main() {
    const int n0 = 5000;
    const int LIMITE_INSERCION = 320000;  // por encima, insercion tarda demasiado
    mt19937 rng(42);

    printf("Datos: enteros aleatorios. Tiempo promedio en ms (calentar + 3 repeticiones).\n\n");
    printf("| N | Datos | Insercion (ms) | MergeSort (ms) | QuickSort (ms) | Ratio Insercion | Ratio Merge | Ratio Quick | Insercion / Merge |\n");
    printf("|---|---|---|---|---|---|---|---|---|\n");

    double antI = 0, antM = 0, antQ = 0;
    const char* nombre[] = {"n", "4n", "16n", "64n", "256n", "1024n"};
    for (int k = 0, n = n0; k < 6; k++, n *= 4) {
        vector<int> datos(n);
        uniform_int_distribution<int> d(0, 1000000000);
        for (int& x : datos) x = d(rng);

        double tI = (n <= LIMITE_INSERCION) ? protocolo(insercion, datos) : -1;
        double tM = protocolo(mergeSort, datos);
        double tQ = protocolo(quickSort, datos);

        char sI[32], rI[16] = "-", rM[16] = "-", rQ[16] = "-", cI[16] = "-";
        if (tI >= 0) snprintf(sI, sizeof sI, "%.2f", tI);
        else snprintf(sI, sizeof sI, "no se corre (~%.0f s estimado)", antI * 16 / 1000);
        if (k > 0) {
            if (tI >= 0) snprintf(rI, sizeof rI, "~%.1fx", tI / antI);
            snprintf(rM, sizeof rM, "~%.1fx", tM / antM);
            snprintf(rQ, sizeof rQ, "~%.1fx", tQ / antQ);
        }
        if (tI >= 0) snprintf(cI, sizeof cI, "~%.0fx", tI / tM);
        printf("| %s | %d | %s | %.2f | %.2f | %s | %s | %s | %s |\n",
               nombre[k], n, sI, tM, tQ, rI, rM, rQ, cI);
        fflush(stdout);
        antI = (tI >= 0) ? tI : antI * 16;
        antM = tM;
        antQ = tQ;
    }
    return 0;
}
