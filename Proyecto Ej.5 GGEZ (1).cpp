#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <iomanip>

using namespace std;

typedef vector<int> Row;
typedef vector<Row> Matrix;

// ==================== FUNCIONES BT (SIMPLIFICADAS) ====================

bool esTestor(const Matrix& A, const vector<int>& cols) {
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

bool esMinimal(const Matrix& A, const vector<int>& cols) {
    if (!esTestor(A, cols)) return false;
    for (int i = 0; i < cols.size(); i++) {
        vector<int> sub = cols;
        sub.erase(sub.begin() + i);
        if (esTestor(A, sub)) return false;
    }
    return true;
}

Matrix BT(const Matrix& A) {
    int n = A[0].size();
    Matrix tipicos;
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

// ==================== FUNCIONES YYC (SIMPLIFICADAS) ====================

bool contiene_vector(const Matrix& vecs, const vector<int>& v) {
    for (const auto& w : vecs) {
        if (w.size() != v.size()) continue;
        bool igual = true;
        for (size_t i = 0; i < v.size(); ++i) {
            if (w[i] != v[i]) {
                igual = false;
                break;
            }
        }
        if (igual) return true;
    }
    return false;
}

bool intersecta_con_fila(const Matrix& MB, const vector<int>& T, int fila) {
    for (int col : T) {
        if (MB[fila][col] == 1) return true;
    }
    return false;
}

bool verificar_subpasos(const Matrix& MB, const vector<int>& T, int k) {
    int numCols = T.size();
    int numFilas = k;
    vector<int> suma_fila(numFilas, 0);
    
    for (int r = 0; r < numFilas; ++r) {
        int s = 0;
        for (int c = 0; c < numCols; ++c) {
            s += MB[r][T[c]];
        }
        suma_fila[r] = s;
    }

    int filas_sum_igual_1 = 0;
    for (int r = 0; r < numFilas; ++r) {
        if (suma_fila[r] == 1) filas_sum_igual_1++;
    }
    if (filas_sum_igual_1 < numCols) return false;

    Matrix S_res;
    for (int r = 0; r < numFilas; ++r) {
        if (suma_fila[r] == 1) {
            vector<int> filaVec(numCols, 0);
            for (int c = 0; c < numCols; ++c) {
                filaVec[c] = MB[r][T[c]];
            }
            S_res.push_back(filaVec);
        }
    }

    for (int c = 0; c < numCols; ++c) {
        int colSum = 0;
        for (size_t r = 0; r < S_res.size(); ++r) {
            colSum += S_res[r][c];
        }
        if (colSum < 1) return false;
    }
    return true;
}

Matrix ejecutar_yyc(const Matrix& MB) {
    int m = MB.size();
    if (m == 0) return {};
    int n = MB[0].size();
    
    Matrix candidatos;
    for (int j = 0; j < n; ++j) {
        if (MB[0][j] == 1) {
            candidatos.push_back({j});
        }
    }

    for (int k = 1; k < m; ++k) {
        Matrix nuevos;
        vector<int> cols_uno;
        for (int j = 0; j < n; ++j) {
            if (MB[k][j] == 1) cols_uno.push_back(j);
        }

        for (size_t idx = 0; idx < candidatos.size(); ++idx) {
            vector<int> T = candidatos[idx];
            if (intersecta_con_fila(MB, T, k)) {
                if (!contiene_vector(nuevos, T)) nuevos.push_back(T);
            } else {
                for (int c : cols_uno) {
                    vector<int> Tprima = T;
                    bool ya = false;
                    for (int x : Tprima) {
                        if (x == c) {
                            ya = true;
                            break;
                        }
                    }
                    if (!ya) {
                        bool inserted = false;
                        for (size_t p = 0; p < Tprima.size(); ++p) {
                            if (c < Tprima[p]) {
                                Tprima.insert(Tprima.begin() + p, c);
                                inserted = true;
                                break;
                            }
                        }
                        if (!inserted) Tprima.push_back(c);
                    }
                    if (verificar_subpasos(MB, Tprima, k + 1)) {
                        if (!contiene_vector(nuevos, Tprima)) {
                            nuevos.push_back(Tprima);
                        }
                    }
                }
            }
        }
        candidatos = nuevos;
    }
    return candidatos;
}

// ==================== OPERADORES f, ?, ? ====================

Matrix operador_phi(const Matrix& A, const Matrix& B) {
    if (A.size() != B.size()) {
        cerr << "Error: f requiere mismas filas en A y B" << endl;
        return {};
    }
    
    Matrix resultado = A;
    for (size_t i = 0; i < A.size(); i++) {
        resultado[i].insert(resultado[i].end(), B[i].begin(), B[i].end());
    }
    return resultado;
}

Matrix operador_theta(const Matrix& A, const Matrix& B) {
    Matrix resultado;
    for (const auto& filaA : A) {
        for (const auto& filaB : B) {
            vector<int> nuevaFila = filaA;
            nuevaFila.insert(nuevaFila.end(), filaB.begin(), filaB.end());
            resultado.push_back(nuevaFila);
        }
    }
    return resultado;
}

Matrix operador_gamma(const Matrix& A, const Matrix& B) {
    Matrix resultado;
    int colsA = A[0].size();
    int colsB = B[0].size();
    
    for (const auto& filaA : A) {
        vector<int> nuevaFila = filaA;
        nuevaFila.insert(nuevaFila.end(), colsB, 0);
        resultado.push_back(nuevaFila);
    }
    
    for (const auto& filaB : B) {
        vector<int> nuevaFila(colsA, 0);
        nuevaFila.insert(nuevaFila.end(), filaB.begin(), filaB.end());
        resultado.push_back(nuevaFila);
    }
    
    return resultado;
}

Matrix phi_power(const Matrix& A, int N) {
    if (N <= 0) return {};
    Matrix resultado = A;
    for (int i = 1; i < N; i++) {
        resultado = operador_phi(resultado, A);
    }
    return resultado;
}

Matrix theta_power(const Matrix& A, int N) {
    if (N <= 0) return {};
    Matrix resultado = A;
    for (int i = 1; i < N; i++) {
        resultado = operador_theta(resultado, A);
    }
    return resultado;
}

Matrix gamma_power(const Matrix& A, int N) {
    if (N <= 0) return {};
    Matrix resultado = A;
    for (int i = 1; i < N; i++) {
        resultado = operador_gamma(resultado, A);
    }
    return resultado;
}

// ==================== FUNCIONES AUXILIARES ====================

float densidad(int filas, int columnas, const Matrix& mb) {
    int contador = 0;
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            if (mb[i][j] == 1) {
                contador += 1;
            }
        }
    }
    return (float)contador / (filas * columnas);
}

Matrix reordenar_por_unos(const Matrix& matriz) {
    Matrix resultado = matriz;
    sort(resultado.begin(), resultado.end(), 
        [](const vector<int>& a, const vector<int>& b) {
            int unosA = count(a.begin(), a.end(), 1);
            int unosB = count(b.begin(), b.end(), 1);
            return unosA < unosB;
        });
    return resultado;
}

pair<double, double> medir_tiempos(const Matrix& matriz) {
    double tiempo_yyc, tiempo_bt;
    
    auto inicio_yyc = chrono::high_resolution_clock::now();
    auto testores_yyc = ejecutar_yyc(matriz);
    auto fin_yyc = chrono::high_resolution_clock::now();
    tiempo_yyc = chrono::duration<double, milli>(fin_yyc - inicio_yyc).count();
    
    auto inicio_bt = chrono::high_resolution_clock::now();
    auto testores_bt = BT(matriz);
    auto fin_bt = chrono::high_resolution_clock::now();
    tiempo_bt = chrono::duration<double, milli>(fin_bt - inicio_bt).count();
    
    return {tiempo_yyc, tiempo_bt};
}

void imprimir_matriz_info(const Matrix& matriz, const string& nombre) {
    cout << nombre << " - Filas: " << matriz.size() 
         << ", Columnas: " << (matriz.empty() ? 0 : matriz[0].size())
         << ", Densidad: " << fixed << setprecision(3) 
         << densidad(matriz.size(), matriz[0].size(), matriz) << endl;
}

// ==================== MATRICES A Y B (DEL ARTÍCULO) ====================

Matrix crear_matriz_A() {
    return {
        {0,0,1,0,0,1},
        {1,0,0,0,1,1},
        {1,1,1,1,1,0},
        {0,0,0,1,0,1}
    };
}

Matrix crear_matriz_B() {
    return {
        {1,1,1,0,0},
        {1,1,0,0,1},
        {1,0,1,1,0},
        {1,0,1,0,1}
    };
}

// ==================== TABLA 5: OPERADOR f ====================

void generar_tabla_5() {
    cout << "\n" << string(70, '=') << endl;
    cout << "TABLA 5 - OPERADOR f SOBRE ?(A,B)" << endl;
    cout << string(70, '=') << endl;
    
    Matrix A = crear_matriz_A();
    Matrix B = crear_matriz_B();
    
    cout << "N\tFilas\tColumnas\tYYC (ms)\tBT (ms)\t\tYYC Ord\t\tBT Ord" << endl;
    cout << "------------------------------------------------------------------------" << endl;
    
    for (int N = 1; N <= 5; N++) {
        Matrix theta_AB = operador_theta(A, B);
        Matrix phi_matriz = phi_power(theta_AB, N);
        Matrix phi_ordenada = reordenar_por_unos(phi_matriz);
        
        auto tiempos_normal = medir_tiempos(phi_matriz);
        auto tiempos_ordenado = medir_tiempos(phi_ordenada);
        
        cout << N << "\t" << phi_matriz.size() << "\t" << phi_matriz[0].size() << "\t\t"
             << fixed << setprecision(2) << tiempos_normal.first << "\t\t"
             << tiempos_normal.second << "\t\t"
             << tiempos_ordenado.first << "\t\t"
             << tiempos_ordenado.second << endl;
    }
}

// ==================== TABLA 6: OPERADOR ? ====================

void generar_tabla_6() {
    cout << "\n" << string(70, '=') << endl;
    cout << "TABLA 6 - OPERADOR ? SOBRE ?(A,B)" << endl;
    cout << string(70, '=') << endl;
    
    Matrix A = crear_matriz_A();
    Matrix B = crear_matriz_B();
    
    cout << "N\tFilas\tColumnas\tYYC (ms)\tBT (ms)\t\tYYC Ord\t\tBT Ord" << endl;
    cout << "------------------------------------------------------------------------" << endl;
    
    for (int N = 1; N <= 4; N++) {
        Matrix theta_AB = operador_theta(A, B);
        Matrix gamma_matriz = gamma_power(theta_AB, N);
        Matrix gamma_ordenada = reordenar_por_unos(gamma_matriz);
        
        auto tiempos_normal = medir_tiempos(gamma_matriz);
        auto tiempos_ordenado = medir_tiempos(gamma_ordenada);
        
        cout << N << "\t" << gamma_matriz.size() << "\t" << gamma_matriz[0].size() << "\t\t"
             << fixed << setprecision(2) << tiempos_normal.first << "\t\t"
             << tiempos_normal.second << "\t\t"
             << tiempos_ordenado.first << "\t\t"
             << tiempos_ordenado.second << endl;
    }
}

// ==================== EJERCICIO 5.1: OPERADORES ====================

void ejercicio_5_1() {
    cout << "\n" << string(70, '=') << endl;
    cout << "EJERCICIO 5.1 - IMPLEMENTACIÓN DE OPERADORES f, ?, ?" << endl;
    cout << string(70, '=') << endl;
    
    Matrix A = crear_matriz_A();
    Matrix B = crear_matriz_B();
    
    cout << "Matrices base del artículo:" << endl;
    imprimir_matriz_info(A, "A");
    imprimir_matriz_info(B, "B");
    
    cout << "\nAplicando operadores básicos:" << endl;
    Matrix phi_AB = operador_phi(A, B);
    Matrix theta_AB = operador_theta(A, B);
    Matrix gamma_AB = operador_gamma(A, B);
    
    imprimir_matriz_info(phi_AB, "f(A,B)");
    imprimir_matriz_info(theta_AB, "?(A,B)");
    imprimir_matriz_info(gamma_AB, "?(A,B)");
    
    cout << "\nPotencias de operadores (N=3):" << endl;
    Matrix phi_3 = phi_power(B, 3);
    Matrix theta_3 = theta_power(operador_theta(A, B), 3);
    Matrix gamma_3 = gamma_power(A, 3);
    
    imprimir_matriz_info(phi_3, "f³(B)");
    imprimir_matriz_info(theta_3, "?³(?(A,B))");
    imprimir_matriz_info(gamma_3, "?³(A)");
}

// ==================== EJERCICIO 5.2: EXPERIMENTACIÓN ====================

void ejercicio_5_2() {
    cout << "\n" << string(70, '=') << endl;
    cout << "EJERCICIO 5.2 - EVALUACIÓN EXPERIMENTAL" << endl;
    cout << string(70, '=') << endl;
    
    generar_tabla_5();
    generar_tabla_6();
}

// ==================== FUNCIÓN PRINCIPAL ====================

int main() {
    cout << "EJERCICIO 5 - OPERADORES Y EVALUACIÓN DE ALGORITMOS" << endl;
    cout << "====================================================" << endl;
    
    ejercicio_5_1();
    ejercicio_5_2();
    
    cout << "\n" << string(70, '=') << endl;
    cout << "EJERCICIO 5 COMPLETADO EXITOSAMENTE" << endl;
    cout << string(70, '=') << endl;
    
    return 0;
}
