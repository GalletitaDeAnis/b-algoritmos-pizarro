// Paolo Pizarro Arispe
// Parcial 2 - Variante A (Turnos de una clinica) - Port a C++ de examen-p2-A.cs
// Se atiende de la urgencia mas baja (1 = critico) a la mas alta (5 = leve); si empatan, primero el que llego antes.
// Compilar (PowerShell): g++ -O2 -std=c++17 -o parcial2.exe parcial2.cpp
#include <chrono>
#include <cstdio>
#include <queue>
#include <string>
#include <tuple>
#include <unordered_set>
#include <vector>

using namespace std;
using reloj = chrono::steady_clock;

const string NADIE = "(nadie)";  // equivale al null del C#

// ---------- SOLUCION INGENUA (lista simple): correcta pero lenta. Logica sin cambios ----------
struct Paciente {
    string nombre;
    int urgencia;
    int llegada;
};

class GestorIngenuo {
    vector<Paciente> lista;
    int llegadas = 0;

public:
    void Registrar(const string& nombre, int urgencia) { lista.push_back({nombre, urgencia, llegadas++}); }  // O(1)

    string AtenderSiguiente() {  // recorre TODA la lista para hallar al mejor + RemoveAt: O(n)
        if (lista.empty()) return NADIE;
        size_t mejor = 0;
        for (size_t i = 1; i < lista.size(); i++) {
            const Paciente& p = lista[i];
            const Paciente& m = lista[mejor];
            if (p.urgencia < m.urgencia || (p.urgencia == m.urgencia && p.llegada < m.llegada)) mejor = i;
        }
        string nombre = lista[mejor].nombre;
        lista.erase(lista.begin() + mejor);
        return nombre;
    }

    bool EstaEsperando(const string& nombre) const {  // recorre la lista: O(n)
        for (const Paciente& p : lista)
            if (p.nombre == nombre) return true;
        return false;
    }

    int Esperando() const { return (int)lista.size(); }
};

// ---------- MI SOLUCION: HEAP (cola de prioridad) + HASH (conjunto de los que esperan) ----------
class GestorMejorado {
    // Heap de MINIMO con la clave (urgencia, llegada, nombre): las tuplas se comparan campo por campo,
    // asi que sale la menor urgencia y, si empatan, la menor llegada (el que llego antes) = el desempate.
    // (los nombres son unicos y la llegada nunca se repite, asi que el nombre nunca decide)
    priority_queue<tuple<int, int, string>, vector<tuple<int, int, string>>, greater<tuple<int, int, string>>> fila;
    unordered_set<string> enEspera;  // hash con los nombres que siguen esperando
    int llegadas = 0;

public:
    void Registrar(const string& nombre, int urgencia) {  // O(log n) el heap + O(1) el hash
        fila.push({urgencia, llegadas++, nombre});
        enEspera.insert(nombre);
    }

    string AtenderSiguiente() {  // O(log n): ver la raiz O(1) y sacarla (baja) O(log n); borrar del hash O(1)
        if (fila.empty()) return NADIE;
        string nombre = get<2>(fila.top());
        fila.pop();
        enEspera.erase(nombre);
        return nombre;
    }

    bool EstaEsperando(const string& nombre) const { return enEspera.count(nombre) > 0; }  // O(1)

    int Esperando() const { return (int)fila.size(); }  // O(1)
};

// ---------- El guion de pruebas (igual al del .cs): (operacion, argumento, urgencia, esperado) ----------
struct Paso { string op, arg; int urg; string esperado; };
const vector<Paso> Guion = {
    {"registrar", "Ana", 3, ""}, {"registrar", "Beto", 1, ""}, {"registrar", "Carla", 3, ""}, {"registrar", "Diego", 2, ""},
    {"esperando", "", 0, "4"},
    {"esta", "Beto", 0, "True"}, {"esta", "Zoe", 0, "False"},
    {"atender", "", 0, "Beto"}, {"esta", "Beto", 0, "False"},
    {"atender", "", 0, "Diego"},
    {"registrar", "Elena", 1, ""},
    {"atender", "", 0, "Elena"}, {"atender", "", 0, "Ana"}, {"atender", "", 0, "Carla"},
    {"atender", "", 0, "(nadie)"}, {"esperando", "", 0, "0"},
};

template <class G>
string Ejecutar(G& g, const Paso& p) {
    if (p.op == "registrar") { g.Registrar(p.arg, p.urg); return ""; }
    if (p.op == "atender") return g.AtenderSiguiente();
    if (p.op == "esta") return g.EstaEsperando(p.arg) ? "True" : "False";
    return to_string(g.Esperando());
}

template <class G>
void Probar(const char* titulo) {
    G g;
    int bien = 0, total = 0;
    for (const Paso& p : Guion) {
        string obtenido = Ejecutar(g, p);
        if (p.op == "registrar") continue;
        total++;
        if (obtenido == p.esperado) bien++;
        else printf("   MAL en <%s %s>: esperado %s, obtuvo %s\n", p.op.c_str(), p.arg.c_str(), p.esperado.c_str(), obtenido.c_str());
    }
    printf("%s: %d de %d pasos correctos\n", titulo, bien, total);
}

// ---------- La CARGA para medir (igual al .cs): n pacientes llegan y despues se atiende a los n ----------
template <class G>
long long EjecutarCarga(G& g, int n) {
    for (int i = 0; i < n; i++) g.Registrar("P" + to_string(i), (int)((i * 7919LL) % 5) + 1);
    long long control = 0;
    for (int i = 0; i < n; i++) control += (long long)g.AtenderSiguiente().size();
    return control;
}

volatile long long sumidero;  // evita que el compilador elimine la carga

// Protocolo: 1 corrida de calentamiento (descartada) + 3 corridas medidas + promedio, en ms.
// Cada corrida usa un gestor NUEVO (la carga lo deja vacio).
template <class G>
double Medir(int n, long long& control) {
    { G g; sumidero = EjecutarCarga(g, n); }  // calentamiento
    double total = 0;
    for (int r = 0; r < 3; r++) {
        G g;
        auto t0 = reloj::now();
        control = EjecutarCarga(g, n);
        auto t1 = reloj::now();
        sumidero = control;
        total += chrono::duration<double, milli>(t1 - t0).count();
    }
    return total / 3.0;
}

int main() {
    Probar<GestorIngenuo>("GestorIngenuo ");
    Probar<GestorMejorado>("GestorMejorado");

    // Comprobacion extra: con una carga real, las dos atienden EXACTAMENTE en el mismo orden
    {
        GestorIngenuo a; GestorMejorado b;
        for (int i = 0; i < 5000; i++) {
            int u = (int)((i * 7919LL) % 5) + 1;
            a.Registrar("P" + to_string(i), u); b.Registrar("P" + to_string(i), u);
        }
        bool igual = true;
        for (int i = 0; i < 5001; i++) igual = igual && a.AtenderSiguiente() == b.AtenderSiguiente();
        printf("Mismo orden de atencion con 5000 pacientes: %s\n\n", igual ? "SI" : "NO");
    }

    const int n0 = 10000;
    printf("| n | Ingenuo (ms) | Veces mas | Mejorado (ms) | Veces mas | Ingenuo / Mejorado |\n");
    printf("|---|---|---|---|---|---|\n");
    double antI = 0, antM = 0;
    for (int k = 0, n = n0; k < 3; k++, n *= 2) {
        long long cI = 0, cM = 0;
        double tI = Medir<GestorIngenuo>(n, cI);
        double tM = Medir<GestorMejorado>(n, cM);
        if (cI != cM) { printf("ERROR: control distinto con n = %d\n", n); return 1; }
        char vI[16] = "-", vM[16] = "-";
        if (k > 0) {
            snprintf(vI, sizeof vI, "x%.2f", tI / antI);
            snprintf(vM, sizeof vM, "x%.2f", tM / antM);
        }
        printf("| %d | %.2f | %s | %.3f | %s | x%.0f |\n", n, tI, vI, tM, vM, tI / tM);
        fflush(stdout);
        antI = tI;
        antM = tM;
    }
    return 0;
}
