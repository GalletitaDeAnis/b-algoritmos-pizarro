// Paolo Pizarro Arispe
// Parcial 1 - Variante B (Registro de comprobantes) - Port a C++ de examen-p1-B.cs
// Problema: dada una lista de comprobantes de pago, ¿hay alguno repetido?
// Compilar (PowerShell): g++ -O2 -std=c++17 -o parcial1.exe parcial1.cpp
#include <algorithm>
#include <chrono>
#include <cstdio>
#include <random>
#include <vector>

using namespace std;
using reloj = chrono::steady_clock;

// Solucion ACTUAL (lenta, logica sin cambios): comparar cada comprobante con todos los que le siguen. O(n^2)
bool HayComprobantesRepetidosPorPares(const vector<int>& comprobantes) {
    for (size_t a = 0; a < comprobantes.size(); a++)
        for (size_t b = a + 1; b < comprobantes.size(); b++)
            if (comprobantes[a] == comprobantes[b]) return true;
    return false;
}

// MI CURA: ordenar una COPIA (no se modifica la lista recibida) y comparar vecinos.
// Si hay repetidos, al ordenar quedan juntos. sort O(n log n) + un recorrido O(n) = O(n log n)
bool HayComprobantesRepetidosCurado(const vector<int>& comprobantes) {
    vector<int> copia = comprobantes;
    sort(copia.begin(), copia.end());
    for (size_t i = 1; i < copia.size(); i++)
        if (copia[i] == copia[i - 1]) return true;
    return false;
}

// PEOR CASO: lista SIN repetidos (1000, 1001, ... mezclados), igual que en el .cs
vector<int> GenerarSinRepetidos(int n, unsigned semilla = 77) {
    vector<int> datos(n);
    for (int i = 0; i < n; i++) datos[i] = 1000 + i;
    mt19937 azar(semilla);
    for (int i = n - 1; i > 0; i--) {
        int k = uniform_int_distribution<int>(0, i)(azar);
        swap(datos[i], datos[k]);
    }
    return datos;
}

volatile bool sumidero;  // evita que el compilador elimine la llamada

// Protocolo: 1 calentamiento (descartado) + 3 corridas medidas + promedio, en ms
double medir(bool (*f)(const vector<int>&), const vector<int>& datos) {
    sumidero = f(datos);  // calentamiento
    double total = 0;
    for (int r = 0; r < 3; r++) {
        auto t0 = reloj::now();
        sumidero = f(datos);
        auto t1 = reloj::now();
        total += chrono::duration<double, milli>(t1 - t0).count();
    }
    return total / 3.0;
}

int main() {
    // Los 5 casos
    struct Caso { vector<int> datos; bool esperado; const char* que; };
    vector<Caso> casos = {
        {{1010, 1003, 1009, 1003, 1005}, true, "repetido en el medio"},
        {{1010, 1003, 1009, 1004, 1005}, false, "sin repetidos"},
        {{}, false, "lista vacia"},
        {{1008}, false, "un solo elemento"},
        {{1004, 1001, 1002, 1006, 1004}, true, "repetido en los extremos"},
    };
    bool todosOk = true;
    for (auto& c : casos) {
        vector<int> antes = c.datos;
        bool lenta = HayComprobantesRepetidosPorPares(c.datos);
        bool cura = HayComprobantesRepetidosCurado(c.datos);
        bool ok = lenta == c.esperado && cura == c.esperado && antes == c.datos;
        todosOk = todosOk && ok;
        printf("%-26s esperado %-5s | por pares %-5s | tu cura %-5s %s\n", c.que,
               c.esperado ? "true" : "false", lenta ? "true" : "false", cura ? "true" : "false",
               ok ? "OK" : "FALLA");
    }
    printf("Casos: %s\n\n", todosOk ? "los 5 pasan (y la lista recibida no se modifica)" : "HAY FALLAS");

    // Medicion en el PEOR CASO, 4 tamanos que se duplican
    const int n0 = 10000;
    printf("| n | Por pares (ms) | x fila anterior | Curado (ms) | x fila anterior | Por pares / Curado |\n");
    printf("|---|---|---|---|---|---|\n");
    double antP = 0, antC = 0;
    for (int k = 0, n = n0; k < 4; k++, n *= 2) {
        vector<int> datos = GenerarSinRepetidos(n);
        double tP = medir(HayComprobantesRepetidosPorPares, datos);
        double tC = medir(HayComprobantesRepetidosCurado, datos);
        char rP[16] = "-", rC[16] = "-";
        if (k > 0) {
            snprintf(rP, sizeof rP, "x%.2f", tP / antP);
            snprintf(rC, sizeof rC, "x%.2f", tC / antC);
        }
        printf("| %d | %.2f | %s | %.3f | %s | x%.0f |\n", n, tP, rP, tC, rC, tP / tC);
        fflush(stdout);
        antP = tP;
        antC = tC;
    }
    return 0;
}
