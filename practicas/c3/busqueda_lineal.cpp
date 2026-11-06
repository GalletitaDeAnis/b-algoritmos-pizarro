#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>

using namespace std;

int busquedaLineal(const vector<int>& arr, int objetivo) {
    for (size_t i = 0; i < arr.size(); i++) {
        if (arr[i] == objetivo) return i;
    }
    return -1;
}

double medirTiempo(const vector<int>& arr, int objetivo) {
    auto inicio = chrono::high_resolution_clock::now();
    busquedaLineal(arr, objetivo);
    auto fin = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> duracion = fin - inicio;
    return duracion.count();
}

double ejecutarProtocolo(int tamaño) {
    vector<int> arr(tamaño, 1);
    int objetivo = -1;
    busquedaLineal(arr, objetivo);
    double tiempoTotal = 0;
    for (int i = 0; i < 5; i++) {
        tiempoTotal += medirTiempo(arr, objetivo);
    }
    return tiempoTotal / 5.0;
}

int main() {
    int n = 50000000; 
    int multiplicadores[] = {1, 4, 16};
    double promedioAnterior = 0;
    
    cout << "Ejecutando mediciones...\n\n";
    cout << "| N | Cantidad de Datos | Tiempo Promedio (ms) | Ratio |\n";
    cout << "|---|---|---|---|\n";

    for (int mult : multiplicadores) {
        int tamañoActual = n * mult;
        double promedio = ejecutarProtocolo(tamañoActual);
        string tamañoStr = (mult == 1) ? "n" : to_string(mult) + "n";
        
        cout << "| " << tamañoStr << " | " << tamañoActual << " | " 
             << fixed << setprecision(4) << promedio << " ms | ";
             
        if (promedioAnterior > 0) {
            cout << "~" << fixed << setprecision(2) << (promedio / promedioAnterior) << "x |\n";
        } else {
            cout << "- |\n";
        }
        promedioAnterior = promedio;
    }
    return 0;
}
