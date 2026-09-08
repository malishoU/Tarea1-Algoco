// INF-221 - Algoritmos y Complejidad
// Tarea 1 - 2026-2
//
// Programa principal para ejecutar los algoritmos de multiplicacion de matrices
// y registrar las mediciones de tiempo y memoria.
//
// Implementado para la Tarea 1 a partir de las especificaciones
// entregadas en el enunciado del curso.




#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/resource.h>
#include <vector>

using namespace std;



// Algoritmo de mult de matrices

vector<vector<int>> naiveMultiply(
    vector<vector<int>>& mat1,
    vector<vector<int>>& mat2
);

vector<vector<int>> strassenMultiply(
    vector<vector<int>>& mat1,
    vector<vector<int>>& mat2
);



// Leer matriz del archivo

vector<vector<int>> readMatrix(const string& filename) {

    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: no se pudo abrir " << filename << endl;
        exit(1);
    }

    vector<vector<int>> matrix;
    string line;

    while (getline(file, line)) {

        if (line.empty())
            continue;

        stringstream ss(line);
        vector<int> row;
        int value;

        while (ss >> value) {
            row.push_back(value);
        }

        if (!row.empty()) {
            matrix.push_back(row);
        }
    }

    return matrix;
}


// Escribir matriz
// --------------------------------------------------

void writeMatrix(
    const string& filename,
    const vector<vector<int>>& matrix
) {

    filesystem::create_directories(
        filesystem::path(filename).parent_path()
    );

    ofstream file(filename);

    for (const auto& row : matrix) {

        for (size_t j = 0; j < row.size(); j++) {

            if (j > 0)
                file << " ";

            file << row[j];
        }

        file << "\n";
    }
}



// Main


int main(int argc, char* argv[]) {

    if (argc != 4) {
        cerr << "Uso: " << argv[0]
             << " <algoritmo> <matriz1> <matriz2>"
             << endl;

        return 1;
    }

    string algorithm = argv[1];
    string file1 = argv[2];
    string file2 = argv[3];

    vector<vector<int>> mat1 = readMatrix(file1);
    vector<vector<int>> mat2 = readMatrix(file2);

    if (mat1.empty() || mat2.empty()) {
        cerr << "Error: matriz vacia." << endl;
        return 1;
    }

    if (mat1[0].size() != mat2.size()) {
        cerr << "Error: dimensiones incompatibles." << endl;
        return 1;
    }

    vector<vector<int>> result;

    auto start = chrono::steady_clock::now();

    if (algorithm == "naive") {

        result = naiveMultiply(mat1, mat2);

    } else if (algorithm == "strassen") {

        result = strassenMultiply(mat1, mat2);

    } else {

        cerr << "Algoritmo no reconocido: "
             << algorithm << endl;

        return 1;
    }

    auto end = chrono::steady_clock::now();

    chrono::duration<double> elapsed = end - start;

    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);

    long maxMemory = usage.ru_maxrss;


    // --------------------------------------------------
    // Extract information from input filename
    // Example: 16_densa_D0_a_1.txt
    // --------------------------------------------------

    string filename =
        filesystem::path(file1).stem().string();

    size_t lastUnderscore =
        filename.rfind('_');

    if (lastUnderscore != string::npos) {
        filename =
            filename.substr(0, lastUnderscore);
    }

    string outputFile =
        "data/matrix_output/" +
        filename +
        "_out.txt";

    writeMatrix(outputFile, result);


    
    // Guardar medidas
    

    filesystem::create_directories(
        "data/measurements"
    );

    string measurementFile =
        "data/measurements/matrix_measurements.csv";

    bool exists =
        filesystem::exists(measurementFile);

    ofstream measurements(
        measurementFile,
        ios::app
    );

    if (!exists) {
        measurements
            << "algorithm,n,type,domain,sample,"
            << "status,time_seconds,max_rss_kb,error\n";
    }


   

    stringstream nameStream(filename);

    string n;
    string type;
    string domain;
    string sample;

    getline(nameStream, n, '_');
    getline(nameStream, type, '_');
    getline(nameStream, domain, '_');
    getline(nameStream, sample, '_');


    measurements
        << algorithm << ","
        << n << ","
        << type << ","
        << domain << ","
        << sample << ","
        << "OK,"
        << elapsed.count() << ","
        << maxMemory << ","
        << "\n";


    cout << "Algoritmo: "
         << algorithm << endl;

    cout << "Tamano: "
         << mat1.size()
         << " x "
         << mat1[0].size()
         << endl;

    cout << "Tiempo: "
         << elapsed.count()
         << " segundos"
         << endl;

    cout << "Memoria maxima: "
         << maxMemory
         << " KB"
         << endl;

    cout << "Salida guardada en: "
         << outputFile
         << endl;

    cout << "Medicion guardada en: "
         << measurementFile
         << endl;

    return 0;
}