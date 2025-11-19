#include <iostream>
#include <vector>
#include <chrono>
#include <set>
#include <algorithm>
using namespace std;

typedef vector<int> Row;
typedef vector<Row> Matrix;

// ========== FUNCIONES DEL ALGORITMO BT (DEL EJERCICIO 3) ==========

// Verifica si un conjunto de columnas es testor
bool esTestor(const Matrix &A, const vector<int> &cols) {
    if (cols.empty()) return false;
    
    int filas = A.size();
    for (int i = 0; i < filas; i++) {
        bool cubre = false;
        for (int c : cols) {
            if (A[i][c] == 1) {
                cubre = true;
                break;
            }
        }
        if (!cubre) return false;
    }
    return true;
}

// Verifica minimalidad para ser testor típico
bool esMinimal(const Matrix &A, const vector<int> &cols) {
    if (!esTestor(A, cols)) return false;

    for (int i = 0; i < cols.size(); i++) {
        vector<int> sub = cols;
        sub.erase(sub.begin() + i);
        if (esTestor(A, sub)) return false; // No es minimal
    }
    return true;
}

// Algoritmo BT
vector<vector<int>> BT(const Matrix &A) {
    int n = A[0].size();
    vector<vector<int>> tipicos;

    // Bitmask para recorrer subconjuntos
    for (int mask = 1; mask < (1 << n); mask++) {
        vector<int> cols;
        for (int j = 0; j < n; j++) {
            if (mask & (1 << j))
                cols.push_back(j);
        }

        if (esMinimal(A, cols))
            tipicos.push_back(cols);
    }
    return tipicos;
}

// ========== FUNCIONES ESPECÍFICAS DEL EJERCICIO 4 ==========

// Crear matriz B a partir de testores típicos
Matrix crearMatrizB(const vector<vector<int>> &testores, int numColumnas) {
    Matrix B;
    
    for (auto &testor : testores) {
        vector<int> fila(numColumnas, 0);
        for (int col : testor) {
            fila[col] = 1;
        }
        B.push_back(fila);
    }
    return B;
}

// Verificar si B es matriz básica (sin filas repetidas)
bool esMatrizBasica(const Matrix &B) {
    set<vector<int>> filasUnicas;
    for (auto &fila : B) {
        if (filasUnicas.count(fila)) {
            return false; // Hay fila repetida
        }
        filasUnicas.insert(fila);
    }
    return true;
}

// Función para imprimir matriz
void imprimirMatriz(const Matrix &matriz, const string &nombre) {
    cout << "Matriz " << nombre << " (" << matriz.size() << " filas x " 
         << (matriz.empty() ? 0 : matriz[0].size()) << " columnas):" << endl;
    for (auto &fila : matriz) {
        cout << "|";
        for (int val : fila) {
            cout << " " << val << " ";
        }
        cout << "|" << endl;
    }
    cout << endl;
}

// Calcular densidad de una matriz
double calcularDensidad(const Matrix &matriz) {
    if (matriz.empty() || matriz[0].empty()) return 0.0;
    
    int totalElementos = matriz.size() * matriz[0].size();
    int unos = 0;
    
    for (auto &fila : matriz) {
        for (int val : fila) {
            if (val == 1) unos++;
        }
    }
    
    return static_cast<double>(unos) / totalElementos;
}

// ========== FUNCIÓN PRINCIPAL DEL EJERCICIO 4 ==========

void ejecutar_ejercicio_4() {
    cout << "\n=== EJERCICIO 4 ===" << endl;
    cout << "Analisis de matriz B construida desde testores tipicos de A\n" << endl;
    
    // Matriz A original del ejercicio 3
    Matrix A = {
        {0,0,1,0,0,1},
        {1,0,0,0,1,1},
        {1,1,1,1,1,0},
        {0,0,0,1,0,1}
    };

    cout << "--- 4.1 Creando matriz B a partir de testores de A ---" << endl;
    
    // Obtener testores típicos de A usando BT
    cout << "Calculando testores tipicos de A con BT..." << endl;
    auto inicioA = chrono::high_resolution_clock::now();
    auto testoresA = BT(A);
    auto finA = chrono::high_resolution_clock::now();
    double tiempoA = chrono::duration<double, milli>(finA - inicioA).count();
    
    cout << "\nTestores tipicos de A encontrados:" << endl;
    for (auto &t : testoresA) {
        cout << "{ ";
        for (int c : t) cout << "x" << c+1 << " ";
        cout << "}" << endl;
    }
    cout << "Tiempo de calculo: " << tiempoA << " ms" << endl;

    // Crear matriz B
    Matrix B = crearMatrizB(testoresA, 6);
    
    cout << "\n";
    imprimirMatriz(B, "B");

    // Verificar si B es matriz básica
    cout << "Verificando si B es matriz basica..." << endl;
    if (esMatrizBasica(B)) {
        cout << "? B es una matriz básica (no tiene filas repetidas)" << endl;
    } else {
        cout << "? B NO es una matriz basica" << endl;
    }

    // 4.2 Calcular testores típicos de B usando BT
    cout << "\n--- 4.2 Calculando testores tipicos de B ---" << endl;
    
    auto inicioB = chrono::high_resolution_clock::now();
    auto testoresB = BT(B);
    auto finB = chrono::high_resolution_clock::now();
    double tiempoB = chrono::duration<double, milli>(finB - inicioB).count();

    cout << "Testores tipicos de B:" << endl;
    if (testoresB.empty()) {
        cout << "No se encontraron testores típicos" << endl;
    } else {
        for (auto &t : testoresB) {
            cout << "{ ";
            for (int c : t) cout << "x" << c+1 << " ";
            cout << "}" << endl;
        }
    }
    cout << "Tiempo de ejecución: " << tiempoB << " ms" << endl;

    // 4.3 Comparación entre A y B
    cout << "\n--- 4.3 Comparacion entre A y B ---" << endl;
    cout << "Matriz A: " << A.size() << " filas × " << A[0].size() << " columnas" << endl;
    cout << "Matriz B: " << B.size() << " filas × " << B[0].size() << " columnas" << endl;
    cout << "Testores tipicos de A: " << testoresA.size() << endl;
    cout << "Testores tipicos de B: " << testoresB.size() << endl;
    cout << "Densidad de A: " << calcularDensidad(A) << endl;
    cout << "Densidad de B: " << calcularDensidad(B) << endl;
    
    
}

// ========== MAIN PARA EJECUTAR SOLO EL EJERCICIO 4 ==========

int main() {
    cout << "==========================================" << endl;
    cout << "    PROYECTO MATEMATICAS DISCRETAS" << endl;
    cout << "        EJERCICIO 4 - INDEPENDIENTE" << endl;
    cout << "==========================================" << endl;
    
    ejecutar_ejercicio_4();
    
    cout << "\nPresione Enter para salir...";
    cin.get();
    
    return 0;
}
