// Paolo Pizarro Arispe
// H3 - Voraz o dinamica en mi caso: que facturas cubre un pago a un proveedor.
// Decision que se repite: para cada factura pendiente del proveedor, ¿la incluyo en este pago o no?
// Resultado a optimizar: cubrir el monto pagado lo mas posible SIN pasarse (ideal: exacto, diferencia 0).
// Es el problema de la suma de subconjuntos (una mochila donde peso = valor).
//   - FUERZA BRUTA: probar los 2^n subconjuntos.                       Correcta, O(2^n).
//   - VORAZ: ordenar de mayor a menor y tomar cada factura si cabe.    Rapida, O(n log n), puede equivocarse.
//   - DINAMICA: tabla de montos alcanzables (anotar lo ya calculado).  Correcta, O(n * S), S = pago en centavos.
// Compilar (PowerShell): g++ -O2 -std=c++17 -o pagos.exe pagos.cpp
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <functional>
#include <random>
#include <vector>

using namespace std;
using reloj = chrono::steady_clock;

// Todos los montos en CENTAVOS (enteros), para que la tabla de la dinamica sea exacta.

// ---------- FUERZA BRUTA: incluir o no incluir cada factura, todas las combinaciones ----------
void bruta(const vector<int>& f, int i, long long suma, long long pago, long long& mejor) {
    if (i == (int)f.size()) {
        if (suma <= pago && suma > mejor) mejor = suma;
        return;
    }
    bruta(f, i + 1, suma + f[i], pago, mejor);  // la incluyo
    bruta(f, i + 1, suma, pago, mejor);         // no la incluyo
}
long long CubrirBruta(const vector<int>& facturas, long long pago) {
    long long mejor = 0;
    bruta(facturas, 0, 0, pago, mejor);
    return mejor;
}

// ---------- VORAZ: la mas grande primero, si cabe ----------
long long CubrirVoraz(vector<int> facturas, long long pago, vector<int>* elegidas = nullptr) {
    sort(facturas.begin(), facturas.end(), greater<int>());
    long long suma = 0;
    for (int x : facturas)
        if (suma + x <= pago) {  // decision voraz: lo mejor AHORA, sin mirar atras
            suma += x;
            if (elegidas) elegidas->push_back(x);
        }
    return suma;
}

// ---------- DINAMICA: alcanzable[s] = ¿puedo sumar exactamente s con las facturas vistas? ----------
// Recurrencia: alcanzable(i, s) = alcanzable(i-1, s)  O  alcanzable(i-1, s - f_i)
// quien[s] = la factura con la que se alcanzo s por primera vez (para reconstruir cuales son).
long long CubrirDinamica(const vector<int>& facturas, long long pago, vector<int>* elegidas = nullptr) {
    vector<int> quien(pago + 1, -1);  // -1 = todavia no alcanzable
    quien[0] = (int)facturas.size();  // el 0 se alcanza sin facturas
    for (int i = 0; i < (int)facturas.size(); i++) {
        int x = facturas[i];
        for (long long s = pago; s >= x; s--)  // de mayor a menor: cada factura se usa una sola vez
            if (quien[s] == -1 && quien[s - x] != -1) quien[s] = i;
    }
    long long mejor = pago;
    while (quien[mejor] == -1) mejor--;  // el mayor monto alcanzable <= pago
    if (elegidas)
        for (long long s = mejor; s > 0; s -= facturas[quien[s]]) elegidas->push_back(facturas[quien[s]]);
    return mejor;
}

// ---------- Datos con forma de mi caso ----------
// n facturas pendientes de un proveedor, de 50.00 a 2000.00 Bs. El pago es la suma de una parte de ellas
// (el proveedor recibio una transferencia por varias facturas juntas): siempre existe una respuesta exacta.
void Generar(int n, mt19937& azar, vector<int>& facturas, long long& pago) {
    facturas.assign(n, 0);
    pago = 0;
    for (int& x : facturas) {
        x = 5000 + (int)(azar() % 195001);
        if (azar() % 2) pago += x;
    }
    if (pago == 0) pago = facturas[0];
}

volatile long long sumidero;
template <class F>
double Medir(F f) {  // 1 calentamiento + 3 corridas + promedio, en ms
    sumidero = f();
    double total = 0;
    for (int r = 0; r < 3; r++) {
        auto t0 = reloj::now();
        sumidero = f();
        auto t1 = reloj::now();
        total += chrono::duration<double, milli>(t1 - t0).count();
    }
    return total / 3.0;
}

void Imprimir(const char* que, const vector<int>& v) {
    printf("  %-9s:", que);
    long long s = 0;
    for (int x : v) { printf(" %.2f", x / 100.0); s += x; }
    printf("  -> suma %.2f Bs\n", s / 100.0);
}

int main() {
    // ---- 1. El ejemplo chico donde el voraz falla ----
    printf("## 1. Ejemplo: pago de 1000.00 Bs, facturas pendientes 600.00, 500.00, 500.00 y 300.00\n\n");
    vector<int> ej = {60000, 50000, 50000, 30000};
    vector<int> eV, eD;
    long long v = CubrirVoraz(ej, 100000, &eV), d = CubrirDinamica(ej, 100000, &eD);
    Imprimir("voraz", eV);
    Imprimir("dinamica", eD);
    printf("  fuerza bruta: %.2f Bs | voraz deja sin cubrir %.2f Bs | dinamica: exacto\n\n",
           CubrirBruta(ej, 100000) / 100.0, (100000 - v) / 100.0);
    if (d != 100000) return 1;

    // ---- 2. ¿Cuantas veces acierta el voraz? 1000 pagos al azar con 15 facturas ----
    mt19937 azar(2026);
    int exactosV = 0, exactosD = 0, iguales = 0, casos = 1000;
    long long faltaV = 0;
    for (int c = 0; c < casos; c++) {
        vector<int> f;
        long long pago;
        Generar(15, azar, f, pago);
        long long b = CubrirBruta(f, pago), vv = CubrirVoraz(f, pago), dd = CubrirDinamica(f, pago);
        if (dd == b) iguales++;
        if (vv == pago) exactosV++;
        if (dd == pago) exactosD++;
        faltaV += pago - vv;
    }
    printf("## 2. %d pagos al azar, 15 facturas pendientes cada uno (siempre existe una combinacion exacta)\n\n", casos);
    printf("| Metodo | Pagos cubiertos exacto | Promedio sin cubrir |\n|---|---|---|\n");
    printf("| Fuerza bruta | %d de %d | 0.00 Bs |\n", casos, casos);
    printf("| Voraz | %d de %d | %.2f Bs |\n", exactosV, casos, faltaV / 100.0 / casos);
    printf("| Dinamica | %d de %d (igual a la bruta en %d de %d) | 0.00 Bs |\n\n", exactosD, casos, iguales, casos);

    // ---- 3. Tiempos: fuerza bruta contra dinamica contra voraz (n crece de a 4) ----
    printf("## 3. Tiempos con n facturas (ms; 1 calentamiento + 3 corridas + promedio)\n\n");
    printf("| n | Fuerza bruta (ms) | Veces mas | Dinamica (ms) | Veces mas | Voraz (ms) | Pago (Bs) |\n");
    printf("|---|---|---|---|---|---|---|\n");
    double antB = 0, antD = 0;
    for (int n = 12; n <= 28; n += 4) {
        vector<int> f;
        long long pago;
        mt19937 az(100 + n);
        Generar(n, az, f, pago);
        double tB = Medir([&] { return CubrirBruta(f, pago); });
        double tD = Medir([&] { return CubrirDinamica(f, pago); });
        double tV = Medir([&] { return CubrirVoraz(f, pago); });
        if (CubrirBruta(f, pago) != CubrirDinamica(f, pago)) { printf("ERROR\n"); return 1; }
        char vB[16] = "-", vD[16] = "-";
        if (antB > 0) { snprintf(vB, 16, "x%.1f", tB / antB); snprintf(vD, 16, "x%.2f", tD / antD); }
        printf("| %d | %.3f | %s | %.3f | %s | %.4f | %.2f |\n", n, tB, vB, tD, vD, tV, pago / 100.0);
        fflush(stdout);
        antB = tB;
        antD = tD;
    }

    // ---- 4. Dinamica y voraz con n que se duplica (la bruta ya no termina) ----
    printf("\n## 4. Dinamica contra voraz, n que se duplica (la fuerza bruta con n = 40 serian ~2^40 casos)\n\n");
    printf("| n | Dinamica (ms) | Veces mas | Voraz (ms) | Veces mas | Pago (Bs) | Voraz deja sin cubrir (Bs) |\n");
    printf("|---|---|---|---|---|---|---|\n");
    antD = 0;
    double antV = 0;
    for (int n = 20; n <= 160; n *= 2) {
        vector<int> f;
        long long pago;
        mt19937 az(500 + n);
        Generar(n, az, f, pago);
        double tD = Medir([&] { return CubrirDinamica(f, pago); });
        double tV = Medir([&] { return CubrirVoraz(f, pago); });
        long long dd = CubrirDinamica(f, pago), vv = CubrirVoraz(f, pago);
        char vD[16] = "-", vV[16] = "-";
        if (antD > 0) { snprintf(vD, 16, "x%.2f", tD / antD); snprintf(vV, 16, "x%.2f", tV / antV); }
        printf("| %d | %.2f | %s | %.4f | %s | %.2f | %.2f |\n", n, tD, vD, tV, vV, pago / 100.0, (dd - vv) / 100.0);
        fflush(stdout);
        antD = tD;
        antV = tV;
    }
    return 0;
}
