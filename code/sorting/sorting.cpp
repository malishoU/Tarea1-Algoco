// INF-221 - Algoritmos y Complejidad
// Tarea 1 - 2026-2
//
// Programa principal para ejecutar los algoritmos de ordenamiento
// y registrar las mediciones de tiempo y memoria.
//
// Implementado para la Tarea 1 a partir de las especificaciones
// entregadas en el enunciado del curso.


#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/resource.h>
#include <vector>

using namespace std;

// Sorting algorithms

vector<int> mergeSortArray(vector<int>& arr);
vector<int> quickSortArray(vector<int>& arr);
vector<int> patienceSortArray(vector<int>& arr);
vector<int> sortArray(vector<int>& arr);

// Split helper

vector<string> split(const string& texto, char delimitador) {

    vector<string> partes;
    string parte;

    stringstream ss(texto);

    while (getline(ss, parte, delimitador)) {
        partes.push_back(parte);
    }

    return partes;
}



// Main


int main(int argc, char* argv[]) {

    
    // Command-line arguments
    

    if (argc < 3) {

        cerr << "Uso: "
             << argv[0]
             << " <algoritmo> <archivo_entrada>"
             << endl;

        return 1;
    }


    string algoritmo = argv[1];
    string nombreArchivo = argv[2];


    
    // Validate algorithm
    

    if (
        algoritmo != "merge" &&
        algoritmo != "quick" &&
        algoritmo != "patience" &&
        algoritmo != "std"
    ) {

        cerr << "Error: algoritmo no reconocido." << endl;

        return 1;
    }


    
    // Read input file
    

    ifstream archivo(nombreArchivo);

    if (!archivo.is_open()) {

        cerr << "Error: no se pudo abrir el archivo: "
             << nombreArchivo
             << endl;

        return 1;
    }


    vector<int> arr;
    int numero;


    while (archivo >> numero) {
        arr.push_back(numero);
    }


    archivo.close();

    filesystem::path rutaEntrada(nombreArchivo);
    string nombreBase = rutaEntrada.stem().string();
    vector<string> partes = split(nombreBase, '_');

    if (partes.size() != 4) {

        cerr << "Error: nombre de archivo invalido: "
             << nombreBase
             << endl;

        return 1;
    }

    string n = partes[0];
    string tipo = partes[1];
    string dominio = partes[2];
    string muestra = partes[3];

    auto inicio = chrono::steady_clock::now();


    if (algoritmo == "merge") {
        mergeSortArray(arr);
    }
    else if (algoritmo == "quick") {

        quickSortArray(arr);

    }
    else if (algoritmo == "patience") {

        patienceSortArray(arr);

    }
    else {

        sortArray(arr);
    }


    auto fin = chrono::steady_clock::now();


    chrono::duration<double> duracion = fin - inicio;

    double tiempo = duracion.count();


    
    // Memory measurement
    //
    // On Linux:
    // ru_maxrss = maximum resident set size in KB.
    

    struct rusage uso;


    if (getrusage(RUSAGE_SELF, &uso) != 0) {

        cerr << "Error: no se pudo obtener el uso de memoria."
             << endl;

        return 1;
    }


    long memoriaKB = uso.ru_maxrss;


    
    // Validate result
    //
    // Validation is done after stopping the timer.
    

    if (!is_sorted(arr.begin(), arr.end())) {

        cerr << "Error: el arreglo no quedo ordenado."
             << endl;

        return 1;
    }


    
    // Write sorted array
    

    string nombreSalida =
        "data/array_output/"
        + nombreBase
        + "_out.txt";


    ofstream archivoSalida(nombreSalida);


    if (!archivoSalida.is_open()) {

        cerr << "Error: no se pudo crear el archivo de salida: "
             << nombreSalida
             << endl;

        return 1;
    }


    for (size_t i = 0; i < arr.size(); i++) {

        archivoSalida << arr[i];

        if (i + 1 < arr.size()) {
            archivoSalida << " ";
        }
    }


    archivoSalida.close();


    
    // Measurement file
    

    string archivoMediciones =
        "data/measurements/sorting_measurements.csv";


    bool existeArchivo =
        filesystem::exists(archivoMediciones);


    ofstream mediciones(
        archivoMediciones,
        ios::app
    );


    if (!mediciones.is_open()) {

        cerr << "Error: no se pudo abrir el archivo de mediciones."
             << endl;

        return 1;
    }


    
    // CSV header
    //
    // This allows sorting.cpp to also work independently
    // outside the Makefile.
    

    if (!existeArchivo) {

        mediciones
            << "algorithm,"
            << "n,"
            << "type,"
            << "domain,"
            << "sample,"
            << "status,"
            << "time_seconds,"
            << "max_rss_kb,"
            << "error"
            << endl;
    }


    
    // Successful measurement
    //
    // The error column is left empty.
    

    mediciones
        << algoritmo << ","
        << n << ","
        << tipo << ","
        << dominio << ","
        << muestra << ","
        << "OK,"
        << tiempo << ","
        << memoriaKB << ","
        << endl;


    mediciones.close();


    
    // Console output
    

    cout << "Algoritmo: "
         << algoritmo
         << endl;

    cout << "Elementos: "
         << arr.size()
         << endl;

    cout << "Tiempo: "
         << tiempo
         << " segundos"
         << endl;

    cout << "Memoria maxima: "
         << memoriaKB
         << " KB"
         << endl;

    cout << "Resultado: CORRECTO"
         << endl;

    cout << "Salida guardada en: "
         << nombreSalida
         << endl;

    cout << "Medicion guardada en: "
         << archivoMediciones
         << endl;


    return 0;
}