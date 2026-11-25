// Paolo Pizarro Arispe
// Evaluacion integradora - Variante B (Compras de una escuela). Port a C++ de examen-int-B.cs.
// Compilar: g++ -O2 -std=c++17 -o integradora.exe integradora.cpp
#include <algorithm>
#include <chrono>
#include <cstdio>
#include <random>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;
using reloj = chrono::steady_clock;

struct Util {
    string codigo;
    int costoBs;
    int utilidad;
};

struct Resultado {
    int utilidad;
    vector<string> codigos;
};

// Ingenuo: prueba los 2^n subconjuntos, igual que el .cs. O(2^n * n).
class CompradorIngenuo {
    vector<string> elegidos;
public:
    Resultado Elegir(const vector<Util>& utiles, int presupuesto) {
        int n = utiles.size(), mejor = 0;
        long long mejorMascara = 0;
        for (long long mascara = 0; mascara < (1LL << n); mascara++) {
            int costo = 0, utilidad = 0;
            for (int i = 0; i < n; i++)
                if (mascara & (1LL << i)) { costo += utiles[i].costoBs; utilidad += utiles[i].utilidad; }
            if (costo <= presupuesto && utilidad > mejor) { mejor = utilidad; mejorMascara = mascara; }
        }
        elegidos.clear();
        for (int i = 0; i < n; i++)
            if (mejorMascara & (1LL << i)) elegidos.push_back(utiles[i].codigo);
        return {mejor, elegidos};
    }
    bool EstaEnLaCompra(const string& codigo) const {  // recorre la lista: O(k)
        return find(elegidos.begin(), elegidos.end(), codigo) != elegidos.end();
    }
};

// Mejorado: programacion dinamica (mochila 0/1) + hash para las consultas.
// mejor[i][p] = maxima utilidad usando los primeros i utiles con presupuesto p.
//   mejor[i][p] = max(mejor[i-1][p], mejor[i-1][p - costo_i] + utilidad_i)   si costo_i <= p
// Costo: O(n * P) tiempo y memoria (P = presupuesto). Reconstruccion: O(n).
class CompradorMejorado {
    unordered_set<string> enCompra;
public:
    Resultado Elegir(const vector<Util>& utiles, int presupuesto) {
        int n = utiles.size();
        long long sumaCostos = 0;
        for (const Util& x : utiles) sumaCostos += x.costoBs;
        // Si el presupuesto pasa la suma de todos los costos, con esa suma ya entran todos: la tabla no necesita mas columnas.
        int P = (int)max(0LL, min((long long)presupuesto, sumaCostos));
        vector<vector<int>> mejor(n + 1, vector<int>(P + 1, 0));
        for (int i = 1; i <= n; i++) {
            int c = utiles[i - 1].costoBs, u = utiles[i - 1].utilidad;
            for (int p = 0; p <= P; p++) {
                mejor[i][p] = mejor[i - 1][p];  // no lo compro
                if (c <= p && mejor[i - 1][p - c] + u > mejor[i][p])
                    mejor[i][p] = mejor[i - 1][p - c] + u;  // lo compro
            }
        }
        Resultado r{mejor[n][P], {}};
        enCompra.clear();
        int p = P;
        for (int i = n; i >= 1; i--)
            if (mejor[i][p] != mejor[i - 1][p]) {  // cambio el valor: el util i se compro
                r.codigos.push_back(utiles[i - 1].codigo);
                enCompra.insert(utiles[i - 1].codigo);
                p -= utiles[i - 1].costoBs;
            }
        return r;
    }
    bool EstaEnLaCompra(const string& codigo) const { return enCompra.count(codigo) > 0; }  // O(1) promedio
};

// ---------- Pruebas (los 6 casos del .cs + 2 de EstaEnLaCompra) ----------
struct Caso {
    const char* que;
    vector<Util> utiles;
    int presupuesto, esperada;
    vector<string> codigos;
};

vector<Caso> casos = {
    {"el voraz se equivoca", {{"U1", 20, 120}, {"U2", 40, 200}, {"U3", 60, 240}}, 100, 440, {"U2", "U3"}},
    {"cuatro utiles", {{"U1", 4, 6}, {"U2", 6, 8}, {"U3", 8, 10}, {"U4", 10, 12}}, 16, 20, {"U2", "U4"}},
    {"presupuesto cero", {{"U1", 5, 10}, {"U2", 7, 12}}, 0, 0, {}},
    {"sin utiles", {}, 30, 0, {}},
    {"nada cabe", {{"U1", 80, 90}, {"U2", 100, 100}}, 60, 0, {}},
    {"entran todos", {{"U1", 10, 20}, {"U2", 14, 24}, {"U3", 6, 8}}, 200, 52, {"U1", "U2", "U3"}},
};

template <class C>
void Probar(const char* titulo) {
    int bien = 0, total = 0;
    for (auto& caso : casos) {
        C comprador;
        Resultado r = comprador.Elegir(caso.utiles, caso.presupuesto);
        vector<string> a = r.codigos, b = caso.codigos;
        sort(a.begin(), a.end());
        sort(b.begin(), b.end());
        total++;
        if (r.utilidad == caso.esperada && a == b) bien++;
        else printf("   MAL en %s: esperado %d, obtuvo %d\n", caso.que, caso.esperada, r.utilidad);
    }
    C c;
    c.Elegir(casos[0].utiles, casos[0].presupuesto);
    total += 2;
    if (c.EstaEnLaCompra("U2")) bien++; else printf("   MAL en EstaEnLaCompra(U2)\n");
    if (!c.EstaEnLaCompra("U1")) bien++; else printf("   MAL en EstaEnLaCompra(U1)\n");
    printf("%s: %d de %d pruebas correctas\n", titulo, bien, total);
}

// ---------- Carga: n utiles al azar (semilla fija), presupuesto = un tercio del costo total ----------
// Mismo diseño que EjecutarCarga del .cs (costo 4..49, utilidad 10..99), con el generador de C++.
vector<Util> GenerarUtiles(int n, int& presupuesto) {
    mt19937 azar(n + 1000);
    vector<Util> utiles;
    int total = 0;
    for (int i = 0; i < n; i++) {
        int costo = 4 + azar() % 46, utilidad = 10 + azar() % 90;
        utiles.push_back({"K" + to_string(i), costo, utilidad});
        total += costo;
    }
    presupuesto = total / 3;
    return utiles;
}

template <class C>
int EjecutarCarga(int n) {
    int presupuesto;
    vector<Util> utiles = GenerarUtiles(n, presupuesto);
    C comprador;
    return comprador.Elegir(utiles, presupuesto).utilidad;
}

volatile int sumidero;
template <class F>
double Medir(F f) {  // 1 calentamiento + 3 corridas medidas + promedio, en ms
    sumidero = f();
    double total = 0;
    for (int r = 0; r < 3; r++) {
        auto t0 = reloj::now();
        sumidero = f();
        auto t1 = reloj::now();
        total += chrono::duration<double, milli>(t1 - t0).count();
    }
    return total / 3;
}

int main() {
    Probar<CompradorIngenuo>("CompradorIngenuo ");
    Probar<CompradorMejorado>("CompradorMejorado");

    printf("\n| n | Ingenuo (ms) | Veces mas | Mejorado (ms) | Veces mas | Utilidad ingenuo | Utilidad mejorado |\n");
    printf("|---|---|---|---|---|---|---|\n");
    int n0 = 20;
    double antI = 0, antM = 0;
    for (int n = n0; n <= n0 + 6; n += 2) {
        double tI = Medir([&] { return EjecutarCarga<CompradorIngenuo>(n); });
        double tM = Medir([&] { return EjecutarCarga<CompradorMejorado>(n); });
        int uI = EjecutarCarga<CompradorIngenuo>(n), uM = EjecutarCarga<CompradorMejorado>(n);
        char vI[16] = "-", vM[16] = "-";
        if (antI > 0) { snprintf(vI, 16, "x%.2f", tI / antI); snprintf(vM, 16, "x%.2f", tM / antM); }
        printf("| %d | %.2f | %s | %.4f | %s | %d | %d |%s\n", n, tI, vI, tM, vM, uI, uM, uI == uM ? "" : " DISTINTA");
        fflush(stdout);
        antI = tI;
        antM = tM;
    }

    // Solucion mejorada con 1000 utiles y presupuesto 10000
    int pres;
    vector<Util> grandes = GenerarUtiles(1000, pres);
    double t = Medir([&] { CompradorMejorado c; return c.Elegir(grandes, 10000).utilidad; });
    CompradorMejorado c;
    printf("\nMejorado con 1000 utiles y presupuesto 10000: %.2f ms (utilidad %d)\n", t, c.Elegir(grandes, 10000).utilidad);
    double tg = Medir([&] { CompradorMejorado c; return c.Elegir(grandes, 1000000000).utilidad; });
    printf("Mejorado con 1000 utiles y presupuesto 1000000000: %.2f ms (utilidad %d)\n", tg, c.Elegir(grandes, 1000000000).utilidad);
    return 0;
}
