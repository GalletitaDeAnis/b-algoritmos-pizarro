// Practica clase 4: busqueda lineal contra busqueda binaria.
// Protocolo: 1 corrida de calentamiento (descartada), 3 repeticiones, promedio.
// Compilar: g++ -O2 -std=c++17 -o busqueda busqueda.cpp
#include <algorithm>
#include <chrono>
#include <cstdio>
#include <random>
#include <vector>

using namespace std;
using reloj = chrono::steady_clock;

// O(n): recorre uno por uno hasta encontrarlo.
int busquedaLineal(const vector<int>& a, int x) {
    for (size_t i = 0; i < a.size(); i++)
        if (a[i] == x) return (int)i;
    return -1;
}

// O(log n): parte el rango a la mitad en cada paso. Requiere el arreglo ORDENADO.
int busquedaBinaria(const vector<int>& a, int x) {
    int lo = 0, hi = (int)a.size() - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (a[mid] == x) return mid;
        if (a[mid] < x) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;
}

volatile long long sumidero = 0;  // evita que el compilador elimine las busquedas

// Mide el tiempo total (ms) de hacer todas las busquedas de 'objetivos'.
template <class F>
double medir(F buscar, const vector<int>& a, const vector<int>& objetivos) {
    auto t0 = reloj::now();
    long long s = 0;
    for (int x : objetivos) s += buscar(a, x);
    auto t1 = reloj::now();
    sumidero = sumidero + s;
    return chrono::duration<double, milli>(t1 - t0).count();
}

// Protocolo: calentar (descartar), repetir 3 veces, promediar.
template <class F>
double protocolo(F buscar, const vector<int>& a, const vector<int>& objetivos) {
    medir(buscar, a, objetivos);  // calentamiento
    double total = 0;
    for (int r = 0; r < 3; r++) total += medir(buscar, a, objetivos);
    return total / 3.0;
}

int main() {
    const int BUSQUEDAS = 1000;  // cuantas busquedas se hacen en cada medicion
    const int n0 = 100000;
    const int mult[] = {1, 4, 16, 64};
    mt19937 rng(42);

    printf("Busquedas por medicion: %d (la mitad existen, la mitad no)\n\n", BUSQUEDAS);
    printf("| N | Datos | Lineal (ms) | Binaria (ms) | Cociente lineal/binaria | Ratio lineal | Ratio binaria | Ordenar (ms) | Busquedas para amortizar el orden |\n");
    printf("|---|---|---|---|---|---|---|---|---|\n");

    double antLin = 0, antBin = 0;
    for (int k = 0; k < 4; k++) {
        int n = n0 * mult[k];
        // Datos: numeros pares 0,2,4,... (ordenados). Los impares no existen.
        vector<int> ordenado(n);
        for (int i = 0; i < n; i++) ordenado[i] = 2 * i;

        vector<int> objetivos(BUSQUEDAS);
        uniform_int_distribution<int> d(0, 2 * n - 1);
        for (int i = 0; i < BUSQUEDAS; i++) {
            int v = d(rng);
            objetivos[i] = (i % 2 == 0) ? (v & ~1) : (v | 1);  // par = existe, impar = no existe
        }

        double tLin = protocolo(busquedaLineal, ordenado, objetivos);
        double tBin = protocolo(busquedaBinaria, ordenado, objetivos);

        // Costo de ordenar primero (los mismos datos desordenados), mismo protocolo.
        vector<int> desordenado = ordenado;
        shuffle(desordenado.begin(), desordenado.end(), rng);
        double tOrd = 0;
        for (int r = 0; r < 4; r++) {
            vector<int> copia = desordenado;
            auto t0 = reloj::now();
            sort(copia.begin(), copia.end());
            auto t1 = reloj::now();
            if (r > 0) tOrd += chrono::duration<double, milli>(t1 - t0).count();  // r=0 es calentamiento
        }
        tOrd /= 3.0;

        // Cuantas busquedas hacen falta para que ordenar + binaria le gane a la lineal.
        double ahorroPorBusqueda = (tLin - tBin) / BUSQUEDAS;
        double amortiza = tOrd / ahorroPorBusqueda;

        char rl[16] = "-", rb[16] = "-";
        if (k > 0) {
            snprintf(rl, sizeof rl, "~%.1fx", tLin / antLin);
            snprintf(rb, sizeof rb, "~%.2fx", tBin / antBin);
        }
        const char* nombre[] = {"n", "4n", "16n", "64n"};
        printf("| %s | %d | %.3f | %.4f | ~%.0fx | %s | %s | %.2f | ~%.0f |\n",
               nombre[k], n, tLin, tBin, tLin / tBin, rl, rb, tOrd, amortiza);
        antLin = tLin;
        antBin = tBin;
    }
    return 0;
}
