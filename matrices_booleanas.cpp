#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <chrono>

using namespace std;

//Funcion que retorne una matriz
vector<vector<int>> generarMatrizAleatoria (int filas, int columnas)
{
    //Declarar la matriz
    vector<vector<int>> matrizBooleana (filas, vector<int> (columnas));
    
    //Crear el mecanismo aleatorio
    
    random_device rd;            //Semilla
    
    mt19937 generador (rd());    //Motor utiliza la semilla
    
    uniform_int_distribution <> enteros (0,1); //Distribucion, genera los enteros 1 y 0
    
    for (int i = 0; i < filas; i++)
    {
        for (int j = 0; j < columnas; j++)
        {
            matrizBooleana.at(i).at(j) = enteros(generador); //Asignar valores aleatorios a la matriz
            if (j == columnas - 1 && matrizBooleana[i][j] == 0 && matrizBooleana[i][0] == 0) //Evitar casos donde la fila sean solo 0s
            {
                matrizBooleana[i][j] = 1;
            }
        }
    }
    
    return matrizBooleana;
}




//Apartado de funciones para generar MATRIZ BASICA a partir de matriz boolena




// Funcion que cuenta el numero de 1s en una fila
int contarUnos(const vector<int>& fila)
{
    int count = 0;
    for (int val : fila)
    {
        if (val == 1)
        {
            count++;
        }
    }
    return count;
}

// Funciion que verifica si hay determinante entre dos filas
bool hayDeterminante(const vector<int>& filaA, const vector<int>& filaB)
{
    bool uno_cero = false;
    bool cero_uno = false;

    for (size_t j = 0; j < filaA.size(); j++)
    {
        if (filaA[j] == 1 && filaB[j] == 0)
        {
            uno_cero = true;
        }
        if (filaA[j] == 0 && filaB[j] == 1)
        {
            cero_uno = true;
        }
        if (uno_cero && cero_uno)
        {
            return true; // ya hay determinante
        }
    }
    return false;
}

// Funcion que compara si dos filas son identicas
bool sonIdenticas(const vector<int>& a, const vector<int>& b)
{
    if (a.size() != b.size())
    {
        return false;
    }
    for (size_t i = 0; i < a.size(); i++)
    {
        if (a[i] != b[i])
        {
            return false;
        }
    }
    return true;
}

// Funcion principal, donde crea la matriz basica
vector<vector<int>> crearMatrizBasica(vector<vector<int>> matriz, int filas, int columnas)
{
    // ---------------- PRIMERA PASADA ---------
    vector<bool> eliminada(filas, false);

    for (int i = 0; i < filas; i++)
    {
        if (eliminada[i])
        {
            continue;
        }

        for (int j = i + 1; j < filas; j++)
        {
            if (eliminada[j])
            {
                continue;
            }

            bool det = hayDeterminante(matriz[i], matriz[j]);

            if (!det)
            {
                int unos_i = contarUnos(matriz[i]);
                int unos_j = contarUnos(matriz[j]);

                if (unos_i > unos_j)
                {
                    eliminada[i] = true;
                    break; // dejamos de comparar i, ya eliminada
                }
                else if (unos_j > unos_i)
                {
                    eliminada[j] = true;
                }
                // si tienen igual cantidad de 1s se mantiene ambas
            }
        }
    }

    // Guardar las filas que no fueron eliminadas en otra matriz (matriz candidata)
    vector<vector<int>> candidata;
    for (int i = 0; i < filas; i++)
    {
        if (!eliminada[i])
        {
            candidata.push_back(matriz[i]);
        }
    }

    // -------------- SEGUNDA PASADA ----------
    int f2 = candidata.size();
    vector<bool> eliminada2(f2, false);

    for (int i = 0; i < f2; i++)
    {
        if (eliminada2[i])
        {
            continue;
        }

        for (int j = i + 1; j < f2; j++)
        {
            if (eliminada2[j])
            {
                continue;
            }

            bool det = hayDeterminante(candidata[i], candidata[j]);

            if (!det)
            {
                int unos_i = contarUnos(candidata[i]);
                int unos_j = contarUnos(candidata[j]);

                if (unos_i > unos_j)
                {
                    eliminada2[i] = true;
                }
                else if (unos_j > unos_i)
                {
                    eliminada2[j] = true;
                }
                // si tienen igual cantidad de 1s se mantienen ambas
            }
        }
    }

    // Eliminar duplicados (dejar solo la primera fila identica)
    vector<vector<int>> matrizBasica;
    for (int i = 0; i < f2; i++)
    {
        if (eliminada2[i])
        {
            continue;
        }

        bool repetida = false;
        for (int k = 0; k < (int)matrizBasica.size(); k++)
        {
            if (sonIdenticas(candidata[i], matrizBasica[k]))
            {
                repetida = true;
                break;
            }
        }

        if (!repetida)
        {
            matrizBasica.push_back(candidata[i]);
        }
    }

    return matrizBasica;
}

//CFunción calcular DENSIDAD
float densidad (int filas, int columnas, vector<vector<int>> mb)
{
    int contador = 0;
    float densidad = 0;
    for (int i = 0; i < filas; i++)
    {
        for (int j = 0; j < columnas; j++)
        {
            if (mb[i][j] == 1)
            {
                contador += 1;
            }
        }
    }

    densidad = (float)contador / (filas * columnas);

    return densidad;
}




//CODIGO ALGORITMO YYC---------------------------------------------------------------------






// Convierte un testor (vector<int>) a string para imprimir
string a_cadena(const vector<int>& t)
{
    string s = "{";
    for (size_t i = 0; i < t.size(); ++i)
    {
        s += "x" + to_string(t[i] + 1); // +1 para notaciOn con la que empieza en 1, es decir en ves de que el primer elemento sea 0, le hacemos 1
        if (i + 1 < t.size())
        {
            s += ", ";
        }
    }
    s += "}";
    return s;
}

// -------------------------------------------------------------
// Verifica si un vector ya esta dentro de un vector de vectores
// Evita duplicados, cuando  se expande los testores con varias columnas que tienen 1 en la misma fila, a vece se produce el mismo testor por dos caminos distintos.
// ------------------------------------------------
bool contiene_vector(const vector<vector<int>>& vecs, const vector<int>& v)
{
    for (const auto& w : vecs)
    {
        if (w.size() != v.size())
        {
            continue;
        }
        bool igual = true;
        for (size_t i = 0; i < v.size(); ++i)
        {
            if (w[i] != v[i])
            {
                igual = false;
                break;
            }
        }
        if (igual)
        {
            return true;
        }
    }
    return false;
}

// Devuelve true si el testor T tiene al menos una columna con 1 en la fila (Condicional 2)
// Verifica si el testor T intersecta la fila de la matriz MB
// (Condicional 2)
bool intersecta_con_fila(const vector<vector<int>>& MB, const vector<int>& T, int fila)
{
    for (int col : T)
    {
        if (MB[fila][col] == 1)
        {
            return true;
        }
    }
    return false;
}
// -------------------------------------------------------------

// Verificación de Subpaso1 y Subpaso2 (filas con suma==1 >= |T|)
// MB: matriz basica
// T: testor candidato (lista de columnas)
// k: numero de filas a considerar
// -------------------------------------------------------------
bool verificar_subpasos(const vector<vector<int>>& MB, const vector<int>& T, int k)
{
    int numCols = (int)T.size();
    int numFilas = k;

    //Subpaso 1:Sumar horizontalmente y contar filas con suma ==1

    vector<int> suma_fila(numFilas, 0);
    for (int r = 0; r < numFilas; ++r)
    {
        int s = 0;
        for (int c = 0; c < numCols; ++c)
        {
            s += MB[r][ T[c] ];
        }
        suma_fila[r] = s;
    }

    int filas_sum_igual_1 = 0;
    for (int r = 0; r < numFilas; ++r)
    {
        if (suma_fila[r] == 1)
        {
            filas_sum_igual_1++;
        }
    }

    if (filas_sum_igual_1 < numCols)
    {
        return false;
    }
    //Subpaso 2: RESET, conservar solo filas con suma 1
    vector<vector<int>> S_res;
    for (int r = 0; r < numFilas; ++r)
    {
        if (suma_fila[r] == 1)
        {
            vector<int> filaVec(numCols, 0);
            for (int c = 0; c < numCols; ++c)
            {
                filaVec[c] = MB[r][ T[c] ];
            }
            S_res.push_back(filaVec);
        }
    }


    //Suma vertical: cada suma de columna en S_res debe ser >=1
    for (int c = 0; c < numCols; ++c)
    {
        int colSum = 0;
        for (size_t r = 0; r < S_res.size(); ++r)
        {
            colSum += S_res[r][c];
        }
        if (colSum < 1)
        {
            return false;//Falla subpaso 2
        }
    }

    return true;//pasa ambos subpasos
}

// -------------------------------------------------------------
// FUNCIÓN PRINCIPAL DEL ALGORITMO YYC 
//
// IMPORTANTE: esta función devuelve los TESTORES TÍPICOS
//
// MB: matriz básica
// RETORNA: vector<vector<int>> con los testores
// -------------------------------------------------------------
vector<vector<int>> ejecutar_yyc(const vector<vector<int>>& MB)
{
    int m = (int)MB.size();
    if (m == 0)
    {
        return {};
    }
    int n = (int)MB[0].size();
    //Testores iniciales: elementos con 1 en fila 0
    vector<vector<int>> candidatos;
    for (int j = 0; j < n; ++j)
    {
        if (MB[0][j] == 1)
        {
            vector<int> single = { j };
            candidatos.push_back(single);
        }
    }

    vector<vector<vector<int>>> testores_por_fila;
    testores_por_fila.push_back(candidatos);

    //Medicion del tiempo
    auto t_inicio_total = chrono::steady_clock::now();
    double tiempo_acumulado_seg = 0.0;

    for (int k = 1; k < m; ++k)
    {
        auto t_inicio_fila = chrono::steady_clock::now();

        vector<vector<int>> nuevos;
        vector<int> cols_uno;
        for (int j = 0; j < n; ++j)
        {
            if (MB[k][j] == 1)
            {
                cols_uno.push_back(j);
            }
        }

        // Recorrer candidatos actuales
        for (size_t idx = 0; idx < candidatos.size(); ++idx)
        {
            vector<int> T = candidatos[idx];

            //Condicional 2
            if (intersecta_con_fila(MB, T, k))
            {
                if (!contiene_vector(nuevos, T))
                {
                    nuevos.push_back(T);
                }
            }
            else
            {
                //Condicional 1
                for (int c : cols_uno)
                {
                    //Insertar ordenadamente
                    vector<int> Tprima = T;
                    bool ya = false;
                    for (int x : Tprima)
                    {
                        if (x == c)
                        {
                            ya = true;
                            break;
                        }
                    }
                    if (!ya)
                    {
                        bool inserted = false;
                        for (size_t p = 0; p < Tprima.size(); ++p)
                        {
                            if (c < Tprima[p])
                            {
                                Tprima.insert(Tprima.begin() + p, c);
                                inserted = true;
                                break;
                            }
                        }
                        if (!inserted)
                        {
                            Tprima.push_back(c);
                        }
                    }

                    //Verificación de subpasos
                    bool ok = verificar_subpasos(MB, Tprima, k + 1);
                    if (ok)
                    {
                        if (!contiene_vector(nuevos, Tprima))
                        {
                            nuevos.push_back(Tprima);
                        }
                    }
                }
            }
        }

        candidatos = nuevos;
        testores_por_fila.push_back(candidatos);

        auto t_fin_fila = chrono::steady_clock::now();
        tiempo_acumulado_seg = chrono::duration<double>(t_fin_fila - t_inicio_total).count();

        cout << "=== Despues de procesar fila " << (k + 1) << " ===\n";
        cout << "Candidatos (contador=" << candidatos.size() << "): ";
        for (size_t i = 0; i < candidatos.size(); ++i)
        {
            cout << a_cadena(candidatos[i]);
            if (i + 1 < candidatos.size())
            {
                cout << ", ";
            }
        }

        //Mostrar tiempo acumulado
        cout << "\n";
        cout << "Tiempo acumulado hasta esta fila: " << tiempo_acumulado_seg << " segundos\n\n";
    }

    return candidatos;
}


//==================================================================================
//==========================Algoritmo BT============================================

// Verifica si un conjunto T es testor para la matriz básica MB
bool esTestor_BT(const vector<vector<int>>& MB, const vector<int>& T) {
    int filas = MB.size();
    int columnas = MB[0].size();

    for (int i = 0; i < filas; i++) {
        bool cubre = false;
        for (int j = 0; j < T.size(); j++) {
            int col = T[j];
            if (MB[i][col] == 1) {
                cubre = true;
                break;
            }
        }
        if (!cubre) return false;
    }
    return true;
}

// Verifica minimalidad para el algoritmo BT
bool esMinimal_BT(const vector<vector<int>>& MB, const vector<int>& T) {
    if (T.size() <= 1) return true;

    for (int i = 0; i < T.size(); i++) {
        vector<int> subT;
        for (int j = 0; j < T.size(); j++) {
            if (i != j) subT.push_back(T[j]);
        }
        if (esTestor_BT(MB, subT)) {
            return false; // si una versión más pequeña también es testor, NO es minimal
        }
    }
    return true;
}

// Algoritmo BT recursivo
void BT_rec(const vector<vector<int>>& MB, int columnaActual, vector<int>& T,
            vector<vector<int>>& testores, int totalColumnas) 
{
    if (columnaActual == totalColumnas) return;

    // Intentar incluir la columna actual
    T.push_back(columnaActual);

    if (esTestor_BT(MB, T)) {
        if (esMinimal_BT(MB, T)) {
            testores.push_back(T);
        }
    } else {
        BT_rec(MB, columnaActual + 1, T, testores, totalColumnas);
    }

    T.pop_back();

    // Intentar sin incluir la columna actual
    BT_rec(MB, columnaActual + 1, T, testores, totalColumnas);
}

// Interfaz principal del algoritmo BT
vector<vector<int>> algoritmoBT(const vector<vector<int>>& MB) {
    vector<vector<int>> testores;
    vector<int> T;
    int totalColumnas = MB[0].size();

    BT_rec(MB, 0, T, testores, totalColumnas);
    return testores;
}







// ---------- Ejemplo de uso en main ----------
// NOTA: el usuario indicó que normalmente pasará la MB; aquí muestro un ejemplo.
// Cambia MB según desees probar.

int main ()
{
	//Declarar variables
	int filas, columnas;
	
	cout<<"_____________________ Matriz Booleana Aleatoria y Matriz Basica ____________________ "<<endl;
	//Pedir datos
	cout<< "Filas: ";
	cin >>filas;
	
	cout<<"Columnas: ";
	cin >>columnas;
	
	//Lamar a las funciones
	vector<vector<int>> matriz = generarMatrizAleatoria (filas, columnas);
	vector<vector<int>> matrizBasica = crearMatrizBasica (matriz, filas, columnas);
	
	//Imrimir matriz booleana
	cout<<"__________ Matriz booleana aleatoria __________"<<endl;
	cout<<endl;
	cout<<endl;
	for (int i = 0 ; i < filas; i++){
		cout<<"|";

		for (int j = 0; j<columnas; j++){
			cout<<" "<<matriz.at(i).at(j)<<" ";
		}
		cout<<"|"<<endl;
	}
	cout<<endl;
	cout<<endl;
	
	filas = matrizBasica.size();
    
	//Imprimir matriz basica
	cout<<"_______________ Matriz basica _______________"<<endl;
	cout <<endl;
	cout<<endl;
	for (int i = 0 ; i < filas; i++){
		cout<<"|";

		for (int j = 0; j<columnas; j++){
			cout<<" "<<matrizBasica[i][j]<<" ";
		}
		cout<<"|"<<endl;
	}

    cout<<endl;
    cout<< "Densidad Matriz Basica: "<<densidad (filas, columnas, matrizBasica)<<endl;
    cout<<endl<<endl;


    int opcion;
    cout<<"Porfavor elija una de las siguientes opciones:"<<endl;
    cout<<"1.  Testores tipicos con algoritmo YYC."<<endl;
    cout<<"2.  Testores tipicos con algoritmo BT."<<endl<<endl;
    cout<<"Opcion:  ";
    cin>>opcion;
    cout<<endl;

    switch (opcion)
    {
        case 1:
            int option;
            cout<< "Elija la opcion:"<<endl;
            cout<<"1. Utilizar la matriz basica A. "<<endl;
            cout<<"2. Utilizar la matriz booleana aleatoria previamente creada."<< endl;
            cout<<"Opcion:  ";
            cin>>option;
            cout<<endl<<endl;

            switch (option){
                case 1:
                {
                    cout <<"----------Matriz Basica A----------"<<endl<<endl;
                    vector<vector<int>> matrizA={{0, 0, 1, 0, 0, 1}, {1, 0, 0, 0, 1, 1}, {1, 1, 1, 1, 1, 0}, {0, 0, 0, 1, 0, 1}}; 
                    for (int i = 0 ; i < 4; i++){
		                cout<<"|";
		                    for (int j = 0; j<6; j++){
			                    cout<<" "<<matrizA[i][j]<<" ";
		                    }
		                cout<<"|"<<endl;
	                }
                    cout<<endl;
                    cout<<"Densidad Matriz Basica A: "<<densidad (4, 6, matrizA)<<endl;
                    cout<<"_______________ Testores tipicos YYC con matriz A _______________"<<endl;
                    vector<vector<int>> testoresTipicos= ejecutar_yyc (matrizA);
                    cout << "=== Resultado final matriz A (testores tipicos) ===\n";
                    for (const auto& t : testoresTipicos) {
                        cout << a_cadena(t) << "\n";
                    }
                    break;
                }
                case 2:
                {
                    cout<<"_______________ Testores tipicos YYC con matriz booleana _______________"<<endl;
                    cout<<endl;
                    vector<vector<int>> testoresTipicos= ejecutar_yyc (matrizBasica);
                    cout << "=== Resultado final (testores tipicos ) ===\n";
                    for (const auto& t : testoresTipicos) {
                        cout << a_cadena(t) << "\n";
                    }
                    break;
                }
                default:
                    cerr<<"Opción incorrecta"<<endl;
                    break;
            }       

            break;
        
        case 2: {
    cout << "\n=== Algoritmo BT ===\n";

    int opcion2;
    cout << "1. Ejecutar BT con matriz A por defecto\n";
    cout << "2. Ejecutar BT con matriz basica generada\n";
    cout << "Seleccione una opcion: ";
    cin >> opcion2;

    vector<vector<int>> MB_BT;

    if (opcion2 == 1) {
        // Matriz A fija (del proyecto original)
        MB_BT = {
            {0,0,1,0,0,1},
            {1,0,0,0,1,1},
            {1,1,1,1,1,0},
            {0,0,0,1,0,1}
        };
        cout << "\nUsando Matriz A (por defecto):\n";
    }
    else {
        // Usar MB generada por tu código previo
        MB_BT = matrizBasica;  
        cout << "\nUsando matriz basica generada:\n";
    }

    // Mostrar la MB usada
    cout << "\nMatriz Basica utilizada:\n";
    for (auto& fila : MB_BT) {
        for (int v : fila) cout << v << " ";
        cout << endl;
    }

    // Medición del tiempo (EN MILISEGUNDOS)
    auto inicio = chrono::high_resolution_clock::now();
    vector<vector<int>> resultadoBT = algoritmoBT(MB_BT);
    auto fin = chrono::high_resolution_clock::now();

    double tiempo_ms = chrono::duration<double, milli>(fin - inicio).count();

    cout << "\n=== Testores tipicos encontrados (BT) ===\n";
    for (auto& t : resultadoBT) {
        cout << "{ ";
        for (int i = 0; i < t.size(); i++) {
            cout << "x" << (t[i] + 1);
            if (i < t.size() - 1) cout << ", ";
        }
        cout << " }\n";
    }

    cout << "\nTiempo total de ejecucion (BT): " << tiempo_ms << " ms\n";

    break;
}

            
        default:
            break;
    }

    /*
    cout<<"_______________ Testores tipicos YYC _______________"<<endl;
    vector<vector<int>> testoresTipicos= ejecutar_yyc (matrizBasica);
    cout << "=== Resultado final (testores tipicos ) ===\n";
    for (const auto& t : testoresTipicos) {
        cout << a_cadena(t) << "\n";
    }
    */
	return 0;	
}
